/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id: dict.c 14250 2007-08-08 23:32:35Z peter $
 *
 * Copyright (C) 2005 Tomas Salfischberger, 2006 Timo Horstschäfer
 *
 * All files in this archive are subject to the GNU General Public License.
 * See the file COPYING in the source tree root for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/

#include "plugin.h"
#include "lib/pluginlib_actions.h"
#include "lib/configfile.h"
#include "lib/playback_control.h"
#include "lib/viewer.h"

#include <ctype.h>

#define CONFIG_VERSION  3
#define CACHE_VERSION   2

//! Global dict variables
static struct dict_s {
    char path[255]; /**< Path of current dictionary */
    char name[255]; /**< Basename of current dictionary */

    int DataLen;

    int fCache;
    int fIndex;
    int fDict;
    //! Current description file (there may be more than one)
    int fDict_n; 

    //! Alphabet offset table
    int32_t charoft[26];
} Dict;

#define DESC_BUFFER_ADD 512

#define OFT_DIR ROCKBOX_DIR

#define CONFIG_FILENAME "dict2.cfg"
#define CONFIG_ITEMS ( sizeof(Conf_data) / sizeof (struct configdata) )
struct conf_s {
    int max_list; /**< Max number of articles listed */
    int viewer_scroll; /**< Number of lines to scroll */
    int viewer_backlight;
    int viewer_shortcut;
} Conf;

struct configdata Conf_data[] = {
    { TYPE_INT, 1, 50, { .int_p = &Conf.max_list }, "max_list", NULL },
    { TYPE_INT, 0, 4, { .int_p = &Conf.viewer_scroll }, "scroll", NULL },
    { TYPE_INT, 0, 2, { .int_p = &Conf.viewer_backlight }, "backlight", NULL },
    { TYPE_INT, 0, 2, { .int_p = &Conf.viewer_shortcut }, "shortcut", NULL },
};

#define MAX_LIST_DEFAULT    50

//! Malloc variables
static struct malloc_s {
    void *buf;
    size_t bufsize;
    size_t bufpos;
} Malloc;

struct cache_h {
    char magic[9];
    uint16_t version;
    int32_t charoft[26];
};

static struct cache_h Cache_h = {
    "DICT_OFT",
    CACHE_VERSION,
    { 0 },
};

//! Length of word_str in stardict's DICT format
#define WORDLEN 256

//! Rockbox filesize limit
#define MAX_FILESIZE 2147483647 

enum dict_action
{
    DICT_USB_CONNECTED = -4,
    DICT_NOT_FOUND = -2,
    DICT_QUIT = -3,
    DICT_ERROR = -1,
    DICT_OK,
};

#define VIEWER_NEW (VIEWER_CUSTOM)

/** \brief Data structure in .idx files
 *
 * Structure of .idx files:
 * \code
 *      char[]      name;   // variable length, zero-terminated, UTF-8
 *      uint32_t    offset; // beginning of the article in the .dict file, Big Endian
 *      uint32_t    size;   // article size, Big Endian
 * \endcode
 */
struct WordData_s
{
    uint32_t offset;
    uint32_t size;
};

struct WordData
{
    uint64_t offset;
    uint32_t size;
};

//! Internal structure for the result
struct DictEntry
{
    char name[WORDLEN];
    int32_t index;
    struct WordData data;
};

#ifndef betoh64

#ifdef ROCKBOX_LITTLE_ENDIAN
static inline uint64_t swap64(uint64_t value)
{
    uint64_t hi = swap32(value >> 32);
    uint64_t lo = swap32(value & 0xffffffff);
    return (lo << 32) | hi;
}
#define betoh64(x) swap64(x)
#else
#define betoh64(x) (x)
#endif

#endif

void str_toupper(const char *src, char *dst)
{
    while (*src)
        *dst++ = toupper(*src++);
    *dst = '\0';
}

//! Display an error message
#define dict_error(...)  \
    DEBUGF(__VA_ARGS__); DEBUGF("\n"); \
    LOGF(__VA_ARGS__);   \
    rb->splashf(HZ, __VA_ARGS__);
    

//! Initialize the plugin buffer
void dict_buf_init(void)
{
    Malloc.buf = rb->plugin_get_buffer((size_t *)&Malloc.bufsize);
    Malloc.bufpos = 0;
}

/** \brief Stack-like malloc
 *
 * This malloc implementation only keeps track of how much memory is allocated
 */
void *dict_malloc(size_t size)
{
    void *p;
    if (Malloc.bufpos+size <= Malloc.bufsize)
    {
        p = Malloc.buf + Malloc.bufpos;
        if ((int)p%4) { DEBUGF("\n%d %% 4 = %d\n", (int)p, (int)p%4); }
        Malloc.bufpos += size;
        return (void *)Malloc.buf + Malloc.bufpos - size;
    } 
    else
    {
        dict_error("Out of Memory");
        return NULL;
    }
}

size_t dict_malloc_align(void)
{
    size_t align = 4 - (((size_t)Malloc.buf + Malloc.bufpos) % 4);
    Malloc.bufpos += align;
    return align;
}

/** \brief Stack-like free
 *
 * @param size Nnumber of bytes to free, 0 will free the whole buffer
 */
void dict_free(size_t size)
{
    if (size == 0 || size > Malloc.bufpos)
        Malloc.bufpos = 0;
    else
        Malloc.bufpos -= size;
}

//! Extract the path and name of the given filename
void dict_get_name(const char *name)
{
    char *slash, *dot;
    size_t n;

    slash = rb->strrchr(name, '/');
    dot = rb->strrchr(slash+1, '.');
    
    /* get path */
    n = slash+1-name;
    rb->strlcpy(Dict.path, name, n);
    Dict.path[n] = '\0';

    /* get filename without extension */
    n = dot-slash;
    rb->strlcpy(Dict.name, slash+1, n);
    Dict.name[n] = '\0';

    /* get index type */
    if (rb->strcmp(dot, ".lidx") == 0)
        Dict.DataLen = sizeof(struct WordData);
    else
        Dict.DataLen = sizeof(struct WordData_s);
}

//! Generates the cache file to the current dictionary
enum dict_action dict_create_cache(void)
{
    uint32_t offset;
    int len;
    char word[WORDLEN];
    enum dict_action ret = DICT_OK;
    int32_t count;
    char a_c, a_cur = 'a'-1;

    int button;
    const struct button_mapping *plugin_contexts[] = { pla_main_ctx };

    rb->splash(0, "Creating offset table... may take some minutes");

    rb->memset(Cache_h.charoft, 0, sizeof(Cache_h.charoft));
    /* write dummy header */
    rb->write(Dict.fCache, &Cache_h, sizeof(struct cache_h));

    offset = count = 0;
    while (ret == DICT_OK)
    {
        /* get name length */
        len = 0;
        while (rb->read(Dict.fIndex, &word[len], 1) == 1)
        {
            if (word[len++] == '\0' || len >= WORDLEN)
                break;
        }
        if (!len)
            break;

        /* character offset table */
        a_c = tolower(word[0]); 
        if (a_c > a_cur && a_c >= 'a' && a_c <= 'z')
        {
            LOGF("'%s' (%ld)", word, count);
            do
                Cache_h.charoft[++a_cur-'a'] = count;
            while (a_cur < a_c);
        }

        /* write current position to file */
        rb->write(Dict.fCache, &offset, sizeof(uint32_t));

        rb->lseek(Dict.fIndex, Dict.DataLen, SEEK_CUR);
        offset += Dict.DataLen + len;
        count++;

        /* don't take over all control */
        button = pluginlib_getaction(TIMEOUT_NOBLOCK, plugin_contexts, ARRAYLEN(plugin_contexts));
        switch (button)
        {
            case PLA_EXIT:
                ret = DICT_ERROR;
                break;
            default:
                if (rb->default_event_handler(button) == SYS_USB_CONNECTED)
                    ret = DICT_USB_CONNECTED;
                break;
        }
        rb->yield();
    }

    /* Finish table */
    if (a_cur < 'z')
    {
        do
            Cache_h.charoft[++a_cur-'a'] = count;
        while (a_cur < 'z');
    }

    /* write real header */
    rb->lseek(Dict.fCache, 0, SEEK_SET);
    rb->write(Dict.fCache, &Cache_h, sizeof(struct cache_h));

    return ret;
}

//! Closes all file descriptors
void dict_close(void)
{
    rb->close(Dict.fCache);
    rb->close(Dict.fIndex);
    rb->close(Dict.fDict);

    Conf.viewer_scroll    = viewer_get_scroll();
    Conf.viewer_backlight = viewer_get_backlight();
    Conf.viewer_shortcut  = viewer_get_shortcut();
    configfile_save(CONFIG_FILENAME, Conf_data,
                    CONFIG_ITEMS, CONFIG_VERSION);
}

enum dict_action dict_open(void)
{
    char fn[FAT_FILENAME_BYTES];

    /* index file */
    if (Dict.DataLen == sizeof(struct WordData))
        rb->snprintf(fn, sizeof(fn), "%s/%s.lidx", Dict.path, Dict.name); // slash added
    else
        rb->snprintf(fn, sizeof(fn), "%s/%s.idx", Dict.path, Dict.name); // slash added
    Dict.fIndex = rb->open(fn, O_RDONLY);
    if (Dict.fIndex < 0)
    {
        dict_error("Failed to open index file: %s", fn);
        dict_close();
        return DICT_ERROR;
    }

    /* cache file */
    rb->snprintf(fn, sizeof(fn), "%s/%s.oft", OFT_DIR, Dict.name); // slash added
    Dict.fCache = rb->open(fn, O_RDONLY);
    if (Dict.fCache >= 0)
    {
        /* check the cache file */
        struct cache_h header;

        rb->read(Dict.fCache, &header, sizeof(struct cache_h));
        if (rb->strcmp(header.magic, Cache_h.magic) == 0 &&
            header.version == Cache_h.version)
        {
            rb->memcpy(Dict.charoft, header.charoft, sizeof(Dict.charoft));
            return PLUGIN_OK;
        }

        dict_error("Cache file outdated");

        rb->close(Dict.fCache);
        rb->remove(fn);
    }

    /* incorrect cache file, create a new one */
    Dict.fCache = rb->creat(fn, 0666);
    if (Dict.fCache >= 0)
    {
        if (dict_create_cache() == DICT_OK)
        {
            rb->memcpy(Dict.charoft, Cache_h.charoft, sizeof(Dict.charoft));

            /* reopen read-only */
            rb->close(Dict.fCache);
            Dict.fCache = rb->open(fn, O_RDONLY);
            return DICT_OK;
        }
        else
        {
            rb->close(Dict.fCache);
            rb->remove(fn);
        }
    }

    dict_error("Failed to open cache file: %s", fn);
    return DICT_ERROR;
}

/** \brief Initializes everything related to dict.
 *
 * @param filename Filename of the .idx file for the dictionary
 */
enum dict_action dict_init(const char *file)
{
    /* malloc */
    dict_buf_init();

    /* variables */
    Conf.viewer_scroll    = viewer_get_scroll();
    Conf.viewer_backlight = viewer_get_backlight();
    Conf.viewer_shortcut  = viewer_get_shortcut();
    Conf.max_list   = MAX_LIST_DEFAULT;
    Dict.fDict = Dict.fDict_n = -1;

    /* configuration data */
    configfile_load(CONFIG_FILENAME, Conf_data,
                    CONFIG_ITEMS, CONFIG_VERSION);

    /* dict name */
    dict_get_name(file);

    /* files */
    return dict_open();
}

/** \brief Returns offset for an entry in the idx file
 *
 * This function allows random access to the idx file
 */
uint32_t dict_cache_offset(int32_t index)
{
    uint32_t offset;

    /* read offset */
    rb->lseek(Dict.fCache, sizeof(uint32_t)*index + sizeof(struct cache_h),
              SEEK_SET);
    rb->read(Dict.fCache, &offset, sizeof(uint32_t));

    return offset;
}

//! Compare function, that represents the order in the .idx file
int dict_index_strcmp(const char *str1, const char *str2)
{
    int a;

    a = rb->strcasecmp(str1, str2);
    if (!a)
    {
        return rb->strcmp(str1, str2);
    }
    
    return a;
}

/** \brief Reads up to n bytes from current fIndex position until a zero occurs
 *
 * \return Number of bytes read.
 */
int dict_index_gets(char *buf, int n)
{
    int i;
    for (i=0; i<n; i++)
    {
        if (!rb->read(Dict.fIndex, buf, 1))
            break;
        if (! *buf++)
        {
            i++;
            break;
        }
    }
    return i;
}

/** \brief Reads up to n bytes from an entry in the idx file until a zero occurs
 *
 * @param index The index in idx t read from.
 * \return Number of bytes read.
 */
int dict_index_gets_from(int32_t index, char *buf, int n)
{
    rb->lseek(Dict.fIndex, dict_cache_offset(index), SEEK_SET);
    return dict_index_gets(buf, n);
}

/** \brief Extracts all data about an entry in idx
 *
 * \return length of the article name
 */
int dict_index_entry(int32_t index, struct DictEntry *out)
{
    int size;
    char buf[sizeof(struct WordData)];

    out->index = index;

    size = dict_index_gets_from(index, out->name, WORDLEN);

    rb->read(Dict.fIndex, buf, Dict.DataLen);

    /* convert to host endianess */
    if (Dict.DataLen == sizeof(struct WordData))
    {
        out->data.offset = betoh64( ((struct WordData *)buf)->offset );
        out->data.size   = betoh32( ((struct WordData *)buf)->size );
    }
    else
    {
        out->data.offset = betoh32( ((struct WordData_s *)buf)->offset );
        out->data.size   = betoh32( ((struct WordData_s *)buf)->size );
    }

    return size;
}

/** \brief Binary search function
 *
 * @return If nothing was found, the last probe is returned negative.
 */
int32_t dict_index_binary_search(const char *search,
                                 int(*cmpf)(const char *, const char *))
{
    char word[WORDLEN];
    int32_t high, low, probe = 0;
    int cmp;

    int count = 0;

    int i = tolower(search[0]) - 'a';
    if (i < 0)
    {
        low = -1;
        high = Dict.charoft[0];
    }
    else if (i >= 25)
    {
        low = Dict.charoft[25];
        high = (rb->filesize(Dict.fCache) - sizeof (struct cache_h)) /
                sizeof(uint32_t);
    }
    else 
    {
        low = Dict.charoft[i] - 1;
        high = Dict.charoft[i+1] + 1;
    }

    while (high - low > 1)
    {
        count++;
        probe = (high + low) / 2;

        dict_index_gets_from(probe, word, WORDLEN);
        LOGF("%ld: '%s'", probe, word);

        /* jump according to the found word. */
        cmp = cmpf(search, word);
        if (cmp < 0)
            high = probe;
        else if (cmp > 0)
            low = probe;
        else
        {
            probe = -probe;
            break;
        }
    }

    LOGF("probes: %d", count);
    return -probe;
}

/** \brief Shows a selection menu for all articles starting with the given word
 *
 * The selections screen is only displayed, if there's more than one result.
 */
int32_t dict_index_find_startingWith(const char *search)
{
    int32_t index;
    char buf[WORDLEN];
    int len, new_len, cmp, needmem;
    int buffer_size;
    int n = 0, sel = 0;

    char **items;
    buffer_size = dict_malloc_align() + Conf.max_list * sizeof(char *);
    items = (char **) dict_malloc(buffer_size);

    /* make uppercase to find lowest value */
    str_toupper(search, buf);
    index = dict_index_binary_search(buf, dict_index_strcmp);
    if (index < 0)
        index = -index;

    rb->lseek(Dict.fIndex, dict_cache_offset(index), SEEK_SET);

    /* find all words that start with the word */
    len = rb->strlen(search);
    while (n<Conf.max_list)
    {
        needmem = new_len = dict_index_gets(buf, WORDLEN);
        if (!new_len)
            break;
        if (needmem%4 > 0)
            needmem += 4 - needmem%4;

        /* seek to next article name */
        rb->lseek(Dict.fIndex, Dict.DataLen, SEEK_CUR);

        cmp = rb->strncasecmp(search, buf, len);
        if (cmp > 0)
        {
            index++;
            continue;
        }
        else if (cmp < 0)
            break;

        items[n] = (char *)dict_malloc(needmem);
        if (items[n] == NULL)
            break;
        buffer_size += needmem;

        rb->strlcpy(items[n], buf, new_len);
        DEBUGF("%s\n", buf);

        n++;
    }

    /* show the selection list */
    if (n > 1)
    {
        struct menu_callback_with_desc menu_ = {NULL,"Search Results", Icon_NOICON};
        struct menu_item_ex menu = {
            MT_RETURN_ID|MENU_HAS_DESC|MENU_ITEM_COUNT(n),
            {.strings = (const char **)items},
            {.callback_and_desc = &menu_}
        };

        sel = rb->do_menu(&menu, &sel, NULL, true);
    }

    dict_free(buffer_size);

    if (n == 0)
        return DICT_NOT_FOUND;

    switch (sel)
    {
        case MENU_ATTACHED_USB:
            return DICT_USB_CONNECTED;
        case MENU_SELECTED_EXIT: case GO_TO_PREVIOUS: case GO_TO_ROOT:
            return DICT_QUIT;
        default:
            return index + sel;
    }
}

//! Wrapper to open the correct .desc file depending on the description offset
enum dict_action dict_desc_open(int n)
{
    char fn[FAT_FILENAME_BYTES];

    if (n == Dict.fDict_n)
        return DICT_OK;

    if (Dict.fDict > 0)
        rb->close(Dict.fDict);

    if (n < 1)
        rb->snprintf(fn, sizeof(fn), "%s/%s.dict", Dict.path, Dict.name);  // slash added
    else
        rb->snprintf(fn, sizeof(fn), "%s/%s.dict.%d", Dict.path, Dict.name, n);  // slash added

    Dict.fDict = rb->open(fn, O_RDONLY);
    if (Dict.fDict < 0)
    {
        dict_error("Failed to open description file: %s", fn);
        return DICT_ERROR;
    }

    Dict.fDict_n = n;

    return DICT_OK;
}

/** \brief Reads the article description
 *
 * @param article The article to be read.
 */
char *dict_desc_read(struct WordData *artl, char *buf)
{
    uint64_t offset = artl->offset;
    int len, n = offset/MAX_FILESIZE;

    if (dict_desc_open(n) != DICT_OK)
        return NULL;

    offset %= MAX_FILESIZE;

    rb->lseek(Dict.fDict, offset, SEEK_SET);
    len = rb->read(Dict.fDict, buf, artl->size);

    /* check if article is splitted between two files */
    if (offset > MAX_FILESIZE - artl->size)
    {
        rb->close(Dict.fDict);
        if (dict_desc_open(++n) != DICT_OK)
            return NULL;

        rb->read(Dict.fDict, buf+len, artl->size-len);
    }
    buf[artl->size] = '\0';

    /* And print it to debug. */
    DEBUGF("Description: %s\n", buf);

    return buf;
}

void dict_menu(void)
{
    bool quit = false;
    int selection = 0;

    MENUITEM_STRINGLIST(menu, "DICT Menu", NULL,
        "Viewer control",
        "Playback control",
        "Max listed articles");

    rb->button_clear_queue();
    rb->sleep(HZ/5);
    while (!quit) {
        selection = rb->do_menu(&menu, &selection, NULL, false);
        switch (selection)
        {
            case 0:
                quit = viewer_menu();
                break;
            case 1:
                quit = playback_control(NULL);
                break;
            case 2:
                quit = rb->set_int("Max listed articles", "", UNIT_INT,
                                   &Conf.max_list, NULL, 10, 10, 50, NULL);
                break;
            default:
                quit = true;
                break;
        }
    }
}

void dict_viewer_callback(int button)
{
    switch (button)
    {
        case PLA_UP: case PLA_UP_REPEAT:
            viewer_up();
            break;
        case PLA_DOWN: case PLA_DOWN_REPEAT:
            viewer_down();
            break;
        case PLA_SELECT:
            viewer_search();
            break;
        case PLA_EXIT:
            viewer_exit(VIEWER_EXIT);
            break;
	case PLA_SELECT_REPEAT:
	    viewer_exit(VIEWER_NEW);
	    break;
        case PLA_CANCEL:
            dict_menu();
            break;
#ifdef VIEWER_HAS_SHORTCUT
        default:
            if (rb->button_status() & BUTTON_VIEWER_SHORTCUT)
                viewer_shortcut();
            break;
#endif

    }
}

//! Parses #redirect from MediaWiki
int parse_mw_redirect(struct DictEntry *a, const char *desc)
{
    char *p;
    int len = 0;
    int32_t index;
    static const char redirect[] = "#redirect";

    if ( rb->strncasecmp(desc, redirect, sizeof(redirect)-1) ) 
        return 0;

    /* get destination article */
    p = rb->strchr(desc+sizeof(redirect)-1, '[') + 2;
    while (p[len] != ']' && p[len] != '#')
        len++;

    rb->strlcpy(a->name, p, len);
    a->name[len] = '\0';

    /* replace '_' with ' ' */
    for (p=a->name; *p; p++)
    {
        if (*p == '_')
            *p = ' ';
    }


    index = dict_index_binary_search(a->name, dict_index_strcmp);
    if (index < 0)
    {
        dict_error("Illegal redirect: %s", a->name);
        return 0;
    }
    dict_index_entry(index, a);

    return 1;
}

//! Dict main loop
enum dict_action dict_main(void)
{
    struct DictEntry result;
    char search[WORDLEN], old[WORDLEN];
    char *desc;
    int n, desc_size = 0, header_len = 0;
    bool new_search = false;

    search[0] = '\0';

    viewer_init(rb);
    viewer_set_scroll(Conf.viewer_scroll);
    viewer_set_backlight(Conf.viewer_backlight);
    viewer_set_shortcut(Conf.viewer_shortcut);
    viewer_set_callback(dict_viewer_callback);

    while (true)
    {
#ifndef HAVE_FLASH_STORAGE
        /* keep the disk running */
        rb->storage_spindown(255);
#ifndef SIMULATOR
//        rb->ata_wakeup();
#endif
#endif /*HAVE_FLASH_STORAGE*/

        rb->strcpy(old, search);
        rb->kbd_input(search, WORDLEN);
        /* exit if the search string is empty or the user didn't change it */
        if (!rb->strlen(search) ||
            (!new_search && !rb->strncmp(old, search, sizeof(old)) ))
        {
            return DICT_OK;
        }
        new_search = false;

        result.index = dict_index_find_startingWith(search);
        switch (result.index)
        {
            case DICT_NOT_FOUND:
                dict_error("No results for \"%s\"", search);
                continue;
            case DICT_QUIT:
                continue;
            case DICT_USB_CONNECTED:
                return DICT_USB_CONNECTED;
        }

        /* get result data */
        dict_index_entry(result.index, &result); 
        do {
            if (desc_size > 0)
                dict_free(desc_size);

            /* make title */
            header_len = rb->strlen(result.name) + 2;
            /* increase the buffer to put a title in front of the article */
            desc_size = result.data.size + header_len + 1;
            desc = (char *)dict_malloc(desc_size);

            /* set desc pointer to beginning of the description */
            if (!dict_desc_read(&result.data, desc+header_len)) 
                return DICT_ERROR;

        } while (parse_mw_redirect(&result, desc+header_len));

        DEBUGF("\
result:\n\
    name:   %s\n\
    index:  %ld\n\
    offset: %llu (%llx)\n\
    size:   %lu\n",
                result.name, result.index, result.data.offset,
                result.data.offset, result.data.size);

#ifndef HAVE_FLASH_STORAGE
        rb->storage_spindown(rb->global_settings->disk_spindown);
#endif

        rb->snprintf(desc, header_len, "%s\n", result.name);
        desc[header_len-1] = '\n';
        desc[desc_size-1] = '\0';

        desc_size += dict_malloc_align();
	n = viewer_set_text(dict_malloc(0), desc);
        dict_malloc(n);
        desc_size += n;

        while (!new_search)
        {
            switch(viewer_run())
            {
                case VIEWER_NEW:
                    new_search = true;
                    continue;
                case VIEWER_ATTACHED_USB:
                    return DICT_USB_CONNECTED;
                default:
                    return DICT_OK;
            }
        }

        dict_free(desc_size);
    }
}

enum plugin_status plugin_start(const void* file)
{
    enum plugin_status ret;

    switch (dict_init(file))
    {
        case DICT_ERROR:
            return PLUGIN_ERROR;
        case DICT_USB_CONNECTED:
            return PLUGIN_USB_CONNECTED;
        default:
            break;
    }

    switch (dict_main())
    {
        case DICT_OK:
            ret = PLUGIN_OK;
            break;
        case DICT_USB_CONNECTED:
            ret = PLUGIN_USB_CONNECTED;
            break;
        default:
            ret = PLUGIN_ERROR;
            break;
    }

#ifndef HAVE_FLASH_STORAGE
    /* reset disk setting */
    rb->storage_spindown(rb->global_settings->disk_spindown);
#endif

    dict_close();

    return ret;
}
