/***************************************************************************
*             __________               __   ___.
*   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
*   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
*   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
*   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
*                     \/            \/     \/    \/            \/
* $Id: viewer.c 12008 2007-01-14 13:48:09Z dave $
*
* Copyright (C) 2007 Timo Horstschäfer
*
*
* All files in this archive are subject to the GNU General Public License.
* See the file COPYING in the source tree root for full license agreement.
*
* This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
* KIND, either express or implied.
*
****************************************************************************/

#include "plugin.h"
#include "pluginlib_actions.h"
#include "viewer.h"

#define VIEWER_LINE_BUF 255
#define VIEWER_SCROLLBAR_WIDTH 4
#define MAX_SEARCHLEN 255

/** text structure */
struct viewer_txt 
{
    const char *p;             /**< Text pointer */
    int        len;            /**< Text size */
    int        pos;            /**< Current line number */

    int        lines;          /**< Number of lines */
    const char **line;         /**< Array of line pointers */

    int        sections;       /**< Number of sections */
    int        *section;       /**< Array of section lines */
    const char **section_name; /**< Name of each section */
};

static void viewer_default_callback(int button);

static struct viewer_txt *txt;

/** Display and character dimensions*/
static int                  cols, rows;
#ifdef HAVE_REMOTE_LCD
static int                  remote_cols, remote_rows;
#endif
#ifdef HAVE_LCD_BITMAP
static int row_height;
#endif

static char                 search[255];
static bool                 quit;
static int                  retval;

/* Customization (also default settings) */
void                        (*viewer_shortcut)(void) 
                                      = (void *)viewer_menu_search;
static void                 (*viewer_callback)(int button)
                                      = viewer_default_callback;
static int                  scroll    = 0;
static int                  backlight = 2;
static int                  shortcut  = 0;

inline int viewer(const struct plugin_api *newrb, const char *text)
{
    static long buffer[128];
    viewer_init(newrb);
    viewer_set_text(buffer, text);
    return viewer_run();
}

/** \brief Returns a pointer to the last character of a line.
 * @param s Pointer to the actual line.
 */
static char *viewer_endl(const char *s, int width)
{
    const char *space = NULL;

    while (true)
    {
        if (*s == '\n' || *s == '\0')
            return (char *)s;
        if (*s == ' ')
            space = s;
        if (!--width)
            break;
        s += rb->utf8seek(s, 1);
    }

    return (char *) ( (space) ? space : s );
}

/** \brief Draws text on the display.
 *
 * Handles newline characters and line wrapping.
 */
static void viewer_draw(const char *s)
{
    const char *p, *eol;
    int line, len;
    char buf[VIEWER_LINE_BUF];
	int left_margin=0;

    rb->lcd_clear_display();

    /* Some Rockbox function restore the backlight settings; reset it */
    viewer_set_backlight(backlight);

#ifdef HAVE_LCD_BITMAP
    if (txt->lines > rows)
    { 
        rb->gui_scrollbar_draw(rb->screens[SCREEN_MAIN], 0, 0,
                                VIEWER_SCROLLBAR_WIDTH-1, LCD_HEIGHT,
                                txt->lines+rows-1, txt->pos, txt->pos+rows,
                                VERTICAL);
        left_margin=VIEWER_SCROLLBAR_WIDTH;
    }
#endif

    /* draw main display */
    for (p=s, line=0; line<rows; line++)
    {
        eol = viewer_endl(p, cols);

        len = eol-p;
        // if (*eol != '\n' && *eol != '\0') //removed to fix missing last character at eol
            len++;

        rb->strlcpy(buf, p, len);
        buf[len] = '\0';
#ifdef HAVE_LCD_BITMAP
        rb->lcd_putsxy(left_margin, line*row_height, buf);
#else
		rb->lcd_puts(0,line,buf);
#endif
        if (*eol)
            p = eol + rb->utf8seek(eol, 1);
        else 
            break;
    }

    rb->lcd_update();

#ifdef HAVE_REMOTE_LCD
    rb->lcd_remote_clear_display();

    /* draw remote */
    for (p=s, line=0; line<remote_rows; line++)
    {
        eol = viewer_endl(p, remote_cols);

        len = eol-p;
        if (*eol != '\n' && *eol != '\0')
            len++;

        rb->strlcpy(buf, p, len);
        buf[len] = '\0';

        rb->lcd_remote_puts(0, line, buf);

        if (*eol)
            p = eol + rb->utf8seek(eol, 1);
        else 
            break;
    }

    rb->lcd_remote_update();
#endif
}

static void viewer_default_callback(int button)
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
        case PLA_CANCEL:
            viewer_menu();
            break;
#ifdef VIEWER_HAS_SHORTCUT
        default:
            /* Could need some better way to access the REC-button */
            if (rb->button_status() & BUTTON_VIEWER_SHORTCUT)
                viewer_shortcut();
            break;
#endif
    }
}

void viewer_init(const struct plugin_api *newrb)
{
    rb = newrb;

#ifdef HAVE_LCD_BITMAP
    rb->lcd_getstringsize("o", &cols, &row_height);
# ifdef HAVE_REMOTE_LCD
    remote_cols = LCD_REMOTE_WIDTH / cols;
    remote_rows = LCD_REMOTE_HEIGHT / row_height;
# endif
    cols = (LCD_WIDTH-VIEWER_SCROLLBAR_WIDTH) / cols;
    rows = LCD_HEIGHT / row_height;
#else
    cols = 11;
    rows = 2;
#endif

    viewer_callback = viewer_default_callback;
    scroll = 0;
}

int viewer_run(void)
{
    int button;
    const struct button_mapping *viewer_contexts[] = { pla_main_ctx };

    viewer_redraw();

    quit = false;
    retval = VIEWER_EXIT;

    while (!quit)
    {
        rb->yield();
        button = pluginlib_getaction(HZ, viewer_contexts, ARRAYLEN(viewer_contexts));

        if (rb->default_event_handler(button) == SYS_USB_CONNECTED)
            viewer_exit(VIEWER_ATTACHED_USB);

        viewer_callback(button);
        viewer_redraw();
    }

    /* restore backlight setting */
    rb->backlight_set_timeout(rb->global_settings->backlight_timeout);

    return retval;
}

int viewer_line(const char *p)
{
    int line;

    for (line=0; line<txt->lines; line++)
    {
        if (p < txt->line[line])
            break;
    }

    return line ? line-1 : line;
}

int viewer_set_text(void *buffer, const char *text)
{
    size_t buffer_used;
    const char *p;

    txt = buffer;
    buffer_used = sizeof(struct viewer_txt);

    txt->p = text;
    txt->len = rb->strlen(txt->p);
    txt->pos = 0;

    /* Create an array with the pointers to the beginning of each line */
    txt->lines = 0;
    txt->line = buffer + buffer_used;
    for (p = txt->p; *p;)
    {
        txt->line[txt->lines++] = p;
        p = viewer_endl(p, cols);
        if (*p)
            p += rb->utf8seek(p, 1);
    }
    buffer_used += txt->lines * sizeof txt->line;
    return buffer_used;
}

void viewer_set_callback(void (*newcallback)(int button))
{
    viewer_callback = newcallback;
}

void viewer_set_scroll(int newscroll)
{
    scroll = newscroll < 0 ? 0 : newscroll;
}

void viewer_set_pos(int line)
{
    if (line < 0)
        txt->pos = txt->lines-1 - (line%txt->lines);
    else if (line >= txt->lines)
        txt->pos = txt->lines-1;
    else
        txt->pos = line;
}

void viewer_set_backlight(int newbacklight)
{
    backlight = (newbacklight > 2) ? 2 : newbacklight;
    switch (backlight)
    {
        case 0:
            rb->backlight_set_timeout(0);
            break;
        case 1:
            rb->backlight_set_timeout(1);
            break;
        default:
            rb->backlight_set_timeout(rb->global_settings->backlight_timeout);
            break;
    }
}

void viewer_set_shortcut(int shortcutindex)
{
    switch (shortcutindex)
    {
        default:
        case 0:
            shortcutindex = 0;
            viewer_shortcut = (void *)viewer_menu_search;
            break;
        case 1:
            viewer_shortcut = (void *)viewer_menu_backlight;
            break;
        case 2:
            viewer_shortcut = (void *)viewer_menu_scroll;
            break;
    }
    shortcut = shortcutindex;
}

int viewer_get_scroll(void)
{
    return scroll;
}

int viewer_get_pos(void)
{
    return txt->pos;
}

int viewer_get_backlight(void)
{
    return backlight;
}

int viewer_get_shortcut(void)
{
    return shortcut;
}

void viewer_up(void)
{
    int n = scroll ? scroll : rows;
    txt->pos = (txt->pos < n) ? 0 : txt->pos - n;
}

void viewer_down(void)
{
    int n = scroll ? scroll : rows;

#ifdef HAVE_REMOTE_LCD
    if (txt->lines < remote_rows)
#else
    if (txt->lines < rows)
#endif
        return;

    if (txt->pos+n >= txt->lines)
        txt->pos = txt->lines-1;
    else
        txt->pos += n;
}

void viewer_redraw(void)
{
    viewer_draw(txt->line[txt->pos]);
}

bool viewer_search(void)
{
    int line;
    const char *p;
    size_t n;

    if (!search[0])
        return false;
   
    n = rb->strlen(search);
    line = (txt->pos == txt->lines-1) ? 0 : txt->pos+1;
    p = txt->line[line];
    while (true)
    {
        if (!*p)
            p = txt->p;

        /* just a slow linear search */
        if (rb->strncasecmp(search, p, n) == 0)
            break;

        if (p == txt->line[txt->pos])
            return false;

        p += rb->utf8seek(p, 1);
    }

    viewer_set_pos(viewer_line(p));
    return true;
}

void viewer_exit(int newretval)
{
    retval = newretval;
    quit = true;
}

/* Viewer menu */
bool viewer_menu_search(void)
{
    rb->kbd_input(search, sizeof(search));
    if (viewer_search())
        return true;
    else
    {
        rb->splashf(HZ, "\"%s\" not found", search);
        return false;
    }
}

bool viewer_menu_scroll(void)
{
    static const struct opt_items scroll_menu[] = {
        { "Full page", -1 },
        { "1 line", -1 },
        { "2 lines", -1 },
        { "3 lines", -1 },
        { "4 lines", -1 },
        { "5 lines", -1 },
    };

    return rb->set_option("Scrolling", &scroll, INT, scroll_menu, 6, NULL);
}

bool viewer_menu_backlight(void)
{
    static const struct opt_items backlight_menu[] = {
        { "Off", -1 },
        { "On", -1 },
        { "use Rockox setting", -1 },
    };

    return rb->set_option("Backlight", &backlight, INT, backlight_menu, 3,
                          viewer_set_backlight);
}

bool viewer_menu_shortcut(void)
{
    static const struct opt_items shortcut_menu[] = {
        { "Search...", -1 },
        { "Backlight", -1 },
        { "Scrolling", -1 },
    };

    return rb->set_option("REC shortcut", &shortcut, INT, shortcut_menu, 3, 
                          viewer_set_shortcut);
}

MENUITEM_FUNCTION(search_item, MENU_FUNC_CHECK_RETVAL, "Search...",
                  viewer_menu_search, NULL, NULL, Icon_NOICON);
MENUITEM_FUNCTION(scroll_item, 0, "Scrolling",
                  viewer_menu_scroll, NULL, NULL, Icon_NOICON);
MENUITEM_FUNCTION(backlight_item, 0, "Backlight",
                  viewer_menu_backlight, NULL, NULL, Icon_NOICON);
MENUITEM_FUNCTION(shortcut_item, 0, "REC shortcut",
                  viewer_menu_shortcut, NULL, NULL, Icon_NOICON);

#ifdef VIEWER_HAS_SHORTCUT
#define VIEWER_SC_ITEM ,&shortcut_item
#else
#define VIEWER_SC_ITEM
#endif
MAKE_MENU(viewer_control_menu, "Viewer Control", NULL, Icon_NOICON,
            &search_item, &scroll_item, &backlight_item VIEWER_SC_ITEM);

bool viewer_menu(void)
{
    switch(rb->do_menu(&viewer_control_menu, NULL, NULL, false))
    {
        case MENU_ATTACHED_USB:
        case true:
            return true;
        default:
            return false;
    }
}
