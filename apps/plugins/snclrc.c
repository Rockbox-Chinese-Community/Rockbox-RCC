#include <ctype.h>
#include "plugin.h"
#include "lib/read_image.h"

PLUGIN_HEADER
//BEGIN SNC_LANG
#define STR_(x) snc_lang_strings[x]
#define MAX_LANGUAGE_SIZE 2048
enum Lang_ID{
  SNC_L_YES
, SNC_L_NO
//Menu
, SNC_L_INSERT_LINE
, SNC_L_DELETE_LINE
, SNC_L_JOIN_LINES
, SNC_L_EDIT_LINES
, SNC_L_EDIT_LINE
, SNC_L_DISPLAY_TIME
, SNC_L_DICTIONARY
, SNC_L_TIMEOFFSET
, SNC_L_REPEAT1
, SNC_L_TRANSLATION
, SNC_L_DELETE_FILE
, SNC_L_CLEAN_BLANKS
, SNC_L_AUTOCUE
, SNC_L_FILE_LIST
, SNC_L_COLORS
, SNC_L_AA
, SNC_L_SCROLLING
, SNC_L_CONF_BUTTON
, SNC_L_SETTINGS
, SNC_L_EDIT
//MENUFUNCTIONS
, SNC_L_LOAD
, SNC_L_SAVE_AS
//Splash
, SNC_L_RESET_PREF
, SNC_L_OPEN_ERROR
, SNC_L_2UTF8
, SNC_L_LINE_TOO_LONG
, SNC_L_TOO_MANY_LINES
, SNC_L_SAVING
, SNC_L_DEL_BUFFER_FULL
, SNC_L_DELETING
, SNC_L_COLORS_CLEARED
, SNC_L_CREATING_INDEX
, SNC_L_SEARCHING
, SNC_L_WORD_NOT_FOUND
, SNC_L_NUM_BLANKS_DELETED
, SNC_L_AUDIO_PLAYING
//functions
, SNC_L_BACKLIGHT
, SNC_L_KARAOKE
, SNC_L_PEAKMETER
, SNC_L_READID3
, SNC_L_READSNC
/* Album art submenu */
, SNC_L_LRCIIAA
, SNC_L_AANLRC
, SNC_L_AABLRC
, SNC_L_AAIILRC
, SNC_L_AASIZE
/* Custom items */
, SNC_L_ID3INFO
, SNC_L_PLAYLIST
, SNC_L_FILES
// misc
, SNC_L_DELETE_FILES
, SNC_L_ADD
, SNC_L_AUTO_INSERT
, SNC_L_CLEAR_LIST
, SNC_L_RESET
, SNC_L_INVERT
, SNC_L_FOREGROUND
, SNC_L_BACKGROUND
, SNC_L_INACTIVE
, SNC_L_HIGHLIGHT
, SNC_L_WARNING
, SNC_L_ELAPSED_TIME
, SNC_L_REMAINING_TIME
, SNC_L_BLACK
, SNC_L_DARK_GREY
, SNC_L_LIGHT_GREY
, SNC_L_WHITE
, SNC_L_CHANGE_DICT
, SNC_L_KEYBOARD
, SNC_L_NEXT
, SNC_L_DELETE_QUESTION
, SNC_L_MIN_BLANK_LENGTH
, SNC_L_SAVE_QUESTION
, SNC_L_CLEAR
, SNC_L_SAVE
, SNC_L_REMOVE
, SNC_L_LINE_SCROLL_START_DELAY
, SNC_L_LINE_SCROLL_DELAY
, SNC_L_SCROLL_LIMIT
, SNC_L_CUE_INTERVAL
, SNC_L_LANGUAGE
, SNC_L_TRSYSFONT
, SNC_L_DISPLAY_TR
, SNC_L_EDIT_SETTINGS
, SNC_L_LAST_INDEX_IN_ARRAY
};

unsigned char *snc_lang_strings[SNC_L_LAST_INDEX_IN_ARRAY];
const unsigned char snc_lang_builtin[] =
  "Yes\0"
  "No\0"
  //Menu
  "Insert (A-B/blank)\0"
  "Delete (A-B/current)\0"
  "Join Lines\0"
  "Edit+\0"
  "Edit\0"
  "Display Time\0"
  "Dictionary\0"
  "Time Offset\0"
  "Repeat 1\0"
  "Translation\0"
  "Delete File\0"
  "Clean Blanks\0"
  "Auto Cue\0"
  "File-List\0"
  "Colors\0"
  "Album Art\0"
  "Scrolling\0"
  "Configure Button\0"
  "Settings\0"
  "EDIT\0"
  //MENUFUNCTIONS
  "Load\0"
  "Save As\0"
  //Splash
  "Reset preferences\0"
  "Can't open '%s'!\0"
  "Converting to UTF8...\0"
  "Lyrics line too long!\0"
  "Too many Lyrics lines!\0"
  "Saving...\0"
  "Delete Buffer is Full!\0"
  "Deleting ...\0"
  "Colors Cleared\0"
  "Creating Index...\0"
  "Searching...\0"
  "Word '%s' not found!\0"
  "Number of Blanks Deleted: %d\0"
  "No Audio Playing!\0"
  //functions
  "Backlight\0"
  "Karaoke\0"
  "Peakmeter\0"
  "Read ID3\0"
  "Read snc\0"
  /* Album art submenu */
  "- Lyrics || AA\0"
  " AA next to Lyrics\0"
  "v AA below Lyrics\0"
  "+ AA || Lyrics\0"
  "AA Size\0"
  /* Custom items */
  "ID3 Info\0"
  "Playlist\0"
  "Files\0"
  // misc
  "Delete Files\0"
  "Add\0"
  "Auto Insert\0"
  "Clear List\0"
  "Reset\0"
  "Invert\0"
  "Foreground\0"
  "Background\0"
  "Inactive\0"
  "Highlight\0"
  "Warning\0"
  "Elapsed Time\0"
  "Remaining Time\0"
  "Black\0"
  "Dark Grey\0"
  "Light Grey\0"
  "White\0"
  "Change Dictionary\0"
  "Keyboard\0"
  "NEXT\0"
  "Delete '%s'?\0"
  "Min Blank Length\0"
  "Save?\0"
  "Clear\0"
  "Save\0"
  "Remove\0"
  "Line-Scroll-Start-Delay\0"
  "Line-Scroll-Delay\0"
  "Scroll-limit\0"
  "Cue Interval\0"
  "Language\0"
  "Systemfont (tr)\0"
  "Display Translation\0"
  "Display Settings\0"
;
//copied from language.c
/* These defines must match the initial bytes in the binary lang file */
/* See tools/genlang (TODO: Use common include for both) */
#define LANGUAGE_COOKIE   0x1a
#define LANGUAGE_VERSION  0x06

#define HEADER_SIZE 4
#define SUBHEADER_SIZE 6

static unsigned char language_buffer[MAX_LANGUAGE_SIZE];
static unsigned char lang_options = 0;

static void rb_lang_init(const unsigned char *builtin, unsigned char **dest, int count)
{
    while(count--) {
        *dest++ = (unsigned char *)builtin;
        /* advance pointer to next string */
        builtin += rb->strlen((char *)builtin) + 1;
    }
}

static int snc_lang_load(const char *filename)
{
    const unsigned char *builtin = snc_lang_builtin;
    unsigned char **dest = snc_lang_strings;
    unsigned char *buffer = language_buffer;
    unsigned int user_num = 0;
    int max_lang_size = MAX_LANGUAGE_SIZE;
    unsigned int max_id = SNC_L_LAST_INDEX_IN_ARRAY;

    int lang_size;
    int fd = rb->open(filename, O_RDONLY);
    int retcode=0;
    unsigned char lang_header[HEADER_SIZE];
    unsigned char sub_header[SUBHEADER_SIZE];
    unsigned int id, num_strings, foffset;

    if(fd < 0)
        return 1;
    rb->read(fd, lang_header, HEADER_SIZE);
    if((lang_header[0] == LANGUAGE_COOKIE) &&
       (lang_header[1] == LANGUAGE_VERSION) &&
       (lang_header[2] == 0xFF)) {
        /* jump to the proper entry in the table of subheaders */
        rb->lseek(fd, user_num * SUBHEADER_SIZE, SEEK_CUR);
        rb->read(fd, sub_header, SUBHEADER_SIZE);
        /* read in information about the requested lang */
        num_strings = (sub_header[0]<<8) | sub_header[1];
        lang_size = (sub_header[2]<<8) | sub_header[3];
        foffset = (sub_header[4]<<8) | sub_header[5];
        if(lang_size <= max_lang_size) {
            /* initialize with builtin */
            rb_lang_init(builtin, dest, num_strings);
            rb->lseek(fd, foffset, SEEK_SET);
            rb->read(fd, buffer, lang_size);

            while(lang_size>3) {
                id = ((buffer[0]<<8) | buffer[1]); /* get two-byte id */
                buffer += 2;                       /* pass the id */
                if(id < max_id) {
#if 0
                    DEBUGF("%2x New: %30s ", id, buffer);
                    DEBUGF("Replaces: %s\n", dest[id]);
#endif
                    dest[id] = buffer; /* point to this string */
                }
                while(*buffer) {               /* pass the string */
                    lang_size--;
                    buffer++;
                 }
                lang_size-=3; /* the id and the terminating zero */
                buffer++;     /* pass the terminating zero-byte */
            }
        }
        else {
            DEBUGF("Language %s too large: %d\n", filename, lang_size);
            retcode = 2;
        }
    }
    else {
        DEBUGF("Illegal language file: %x %x %d\n", lang_header[0], lang_header[1], lang_header[2]);
        retcode = 3;
    }
    rb->close(fd);
    lang_options = retcode ? 0 : lang_header[3];
    return retcode;
}

//END SNC_LANG
/******************************************/
/* Keymapping */
#define CONTEXT_SNC        CONTEXT_WPS
#define SNC_SELECT_OR_MODE ACTION_WPS_BROWSE
#define SNC_PLAY_PAUSE     ACTION_WPS_PLAY
#define SNC_VISUAL_REW     ACTION_WPS_SEEKBACK
#define SNC_VISUAL_FF      ACTION_WPS_SEEKFWD
#define SNC_STOP_SEEK      ACTION_WPS_STOPSEEK
#define SNC_NEXT           ACTION_WPS_SKIPNEXT
#define SNC_PREV           ACTION_WPS_SKIPPREV
#define SNC_QUIT           ACTION_WPS_STOP
#define SNC_DEC            ACTION_WPS_VOLDOWN
#define SNC_INC            ACTION_WPS_VOLUP
#define SNC_OPEN_MENU      ACTION_WPS_CONTEXT
#define SNC_AB_MENU        ACTION_WPS_QUICKSCREEN

#ifdef CUSTOM_PATCH
  #define SNC_SAVE         ACTION_WPS_FN
  #define SNC_TSTAMP_HK    ACTION_WPS_HOTKEY
  #define SNC_AB           ACTION_WPS_MENU
  #define SNC_PLIST        ACTION_WPS_VIEW_PLAYLIST
#else
  #define SNC_SAVE         ACTION_WPS_REC
  #define SNC_TSTAMP_HK    ACTION_WPS_MENU
  #define SNC_AB           ACTION_WPS_ABRESET
#endif

#ifdef HAVE_SCROLLWHEEL /* iPods and Sansa e200 */
  #define SNC_LIST_UP      ACTION_WPS_VOLDOWN
  #define SNC_LIST_DOWN    ACTION_WPS_VOLUP
#else
  #define SNC_LIST_UP      ACTION_WPS_VOLUP
  #define SNC_LIST_DOWN    ACTION_WPS_VOLDOWN
#endif
/******************************************/
#define APP_NAME        "snclrc"
/******************************************/
/* icons */
static unsigned char volume_icons[][7]={
  {0x00,0x1c,0x1c,0x3e,0x7f,0x00,0x00}, /* Speaker */
  {0x01,0x1e,0x1c,0x3e,0x7f,0x20,0x40}, /* Speaker mute */
};
#define ICON_WIDTH    9
#define ICON_HEIGHT   8
#define UTF8_ICON     bitmap_icons_8x8[0]
#define VBR_ICON      bitmap_icons_8x8[1]
#define NOTE_ICON     bitmap_icons_7x8[0]
#define PAUSE_ICON    bitmap_icons_7x8[5]
#define AA_ICON_OFFSET  5
#define TR_ICON       bitmap_icons_7x8[9]
#define ARTIST_ICON   bitmap_icons_7x8[10]
#define NEXT_ICON     bitmap_icons_7x8[11]
#define RIGHT_ICON    bitmap_icons_3x8[0]
#define LEFT_ICON     bitmap_icons_3x8[1]
#define BUSY_ICON     bitmap_icons_7x8[12]

static const unsigned char bitmap_icons_3x8[][3]={
  {0x7c,0x38,0x10}, /* right */
  {0x10,0x38,0x7c}, /* left */
};

static const unsigned char bitmap_icons_8x8[][8]={
  {0x07,0x08,0x0e,0x21,0xd7,0xb1,0x4e,0x05}, /* utf8 */
  {0x0e,0xe4,0x53,0xa0,0x0f,0x15,0x0a,0x00}, /* vbr */
};

static const unsigned char bitmap_icons_7x8[][7] =
{
  {0x00,0x60,0x7f,0x03,0x33,0x3f,0x00}, /* Musical note */
  {0x44,0x4e,0x5f,0x44,0x44,0x44,0x38}, /* Repeat playmode */
  {0x44,0x4e,0x5f,0x44,0x38,0x02,0x7f}, /* Repeat-one playmode */
  {0x3e,0x41,0x51,0x41,0x45,0x41,0x3e}, /* Shuffle playmode (dice) */
  {0x7f,0x04,0x4e,0x5f,0x44,0x38,0x7f}, /* Repeat-AB playmode */
  {0x00,0x7f,0x7f,0x00,0x7f,0x7f,0x00}, /* Pause */
  {0x1c,0x14,0x18,0x00,0x2a,0x2a,0x20}, /* Album Art west */
  {0x00,0x04,0x75,0x55,0x65,0x04,0x00}, /* Album Art south */
  {0x00,0x3c,0x24,0x2c,0x38,0x00,0x00}, /* Album Art center */
  {0x01,0x05,0x29,0x20,0xb8,0x80,0x80}, /* Translation */
  {0x38,0x7c,0xd6,0x9e,0xd6,0x7c,0x38}, /* Artist */
  {0x44,0x28,0x54,0x28,0x10,0x7c,0x00}, /* Next */
  {0xc3,0xa5,0xd8,0xfd,0xe5,0xc3,0x00}, /* Hourglass */
};
static const unsigned char volume_values[]={0x00,0x08,0x14,0x2a,0x55,0x5f};
/******************************************/
#define BUFFERSIZE 512
#define MAX_ROWS 8 /* max 7 lines  [0]line1[1]line2[2]...line7[7] */

#define MAX_SECTIONS 300  /* section: time lyrics */
#define NOT_INIT -1
#define MAX_ULONG (unsigned long) -1
#define STATUS_ROW 0
#define SET_TIME_LATENCY 100
#define UPDATE_LATENCY 200
#define FF_REW_TOLERANCE 1000
#define MODIFIED_TIMETAG 2
#define MODIFIED_OFFSET_TIMETAG 4
#define MODIFIED_LYRICS  8
#define END_MARK "_-^-_"
#define COUNT_DOWN_MARK '*'
#define MARK '*'
#define TIME_FORMAT "%d:%02d"
#define RET_OK  ACTION_STD_CANCEL
#define RET_NOK ACTION_REDRAW
#define TIMEBAR_LYRICS_SPACE 3
#define NL "\r\n"
#define BREAK_LENGTH 5000
#define MIN_WIDTH_PEAKMETER 10
#define MAX_BUTTON_FUNC 2
#define SNC_MAX_PATH 60

/* status x-positions */
#define STATUS_VOL_POS       0
#define STATUS_BUSY_POS     11
#define STATUS_AA_POS       22
#define STATUS_UTF8_POS     33
#define STATUS_VBR_POS      44
#define STATUS_TR_POS       55
#define STATUS_MODIFIED_POS 66
#define STATUS_PAUSE_POS    77
#define STATUS_DELBOX_POS   88
#define STATUS_REPEAT_MODE_POS   LCD_WIDTH-8

#ifdef HAVE_REMOTE_LCD
  #define MAX_TYPE 3
  #define RC 2
#else
  #define MAX_TYPE 2
#endif

#define SCROLL 0
#define EDIT 1

#define SNC_CONFIG_VERSION  8 /* increment this every time the struct Prefs is restructered */
/* only for the binary settings */
#define PREF_BACKLIGHT      1
#define PREF_TRANSLATION    2
#define PREF_PEAKMETER      4
#define PREF_KARAOKE        16
#define PREF_READID3        32
#define PREF_READSNC        64
#define PREF_TIMEVISIBLE   128
#define PREF_TRSYSFONT     256
#define PREF_TRVISIBLE     512
#define PREF_SAVE 0xFFFF

#define SCROLL_MARKER_WIDTH 3
#define SCROLL_MARKER_X LCD_WIDTH-SCROLL_MARKER_WIDTH
#define AUTOCUE_ID 1
#define MAX_FILES2DELETE 20
/* makros */
#define SET_ROWS(snc,val) {int ii=0; for(;ii<MAX_TYPE;ii++) snc->x[ii].rows=val;}
#define SET_WIDTH(snc,val) {int ii=0; \
                            for(;ii<MAX_TYPE;ii++) \
                              snc->x[ii].width[0] = snc->x[ii].total_width = val;}
#define SET_LYRICS(snc,addr,row) {int ii=0; for(;ii<MAX_TYPE;ii++) \
                                  snc->x[ii].lyrics[row]=addr;}
#define COPY_SNC(dest,src) {int ii=0; for(;ii<MAX_TYPE;ii++) \
                snc_copy_(&(dest->x[ii]), &(src->x[ii]));}

#define CALC_MS(time) (int)(time%1000)/10
#define CALC_SS(time) (int)(time/1000)%60
#define CALC_MM(time) (int)time/60000
#define LCD_CLEAR_AREA(screen,x,y,w,h) \
  rb->screen_clear_area(rb->screens[screen],x,y,w,h);
#define AUDIO_ELAPSED() \
  (rb->audio_current_track()!=NULL ? rb->audio_current_track()->elapsed : 0)
#define IS_ID3_VALID(id3entry) (id3entry!=NULL && id3entry->path[0]=='/')
/* spaces at the beginning are skipped, so it's not necessary to check for a space */
#define IS_BLANK(id) (sncs[id].x[SCROLL].lyrics[0][0] == 0)
#define NO_LYRICS() (num_snc < 2)
#define IS_AUTOCUE() (sncs[0].time_in_ms==AUTOCUE_ID)
#define SET_SCROLL_ELEMENT(e, icon_, text_, y_, yoffset_)\
  e->text = &text_; e->y = y_; e->icon = icon_; e->icon_y_offset = yoffset_;
#define SNCLRC_DATA_DIR PLUGIN_APPS_DIR"/"APP_NAME
#define SETTINGS_FILE SNCLRC_DATA_DIR"/"APP_NAME".dat"
#define TIME_BAR_ROW 2

enum e_abfile_state{AB_NO,AB_YES,AB_UNKNOWN};
enum e_supported_formats{EXT_LRC,EXT_LRC8,EXT_SNC,EXT_TXT,
                         EXT_AB,EXT_TR,EXT_MP3,EXT_CUE};
enum e_repeat_modes{RMODE_CLEAR = 0, RMODE_OFF, RMODE_ALL, RMODE_ONE, RMODE_SHUFFLE, RMODE_AB};
enum e_load_translation{TR_FAILED=-1, TR_OFF=0, TR_OK=2};
enum e_snc_type{TYPE_STD=0, TYPE_TR=1};
enum e_aa_pos {NO_AA=0, BMP_WEST, BMP_SOUTH, BMP_CENTER};

/* Prototypes */
static bool load_file(enum e_supported_formats);
static int file_list_menu(void);
static void update(void);
static int add2file_list(bool remove);
static int switch_backlight(void);
static int switch_karaoke(void);
static int switch_peakmeter(void);
static int switch_readid3(void);
static int switch_readsnc(void);
static int switch_tr_use_sysfont(void);
static int set_time_offset(void);
static int switch_repeat1(void);
static int load_translation(void);
static int set_auto_cue(void);
static int file_list_menu(void);
static int switch_aa_0(void);
static int switch_aa_w(void);
static int switch_aa_s(void);
static int switch_aa_c(void);
static int set_aa_size(void);
static int lookup_word(void);

/* structs */
struct ButtonFunc{
  int num;
  int id[MAX_BUTTON_FUNC];
  int (*func[MAX_BUTTON_FUNC])(void);
};

static struct Preferences {
  int version;
  int save;
  int fontheight;
  int load_bmp;
  int sxs_bmp;
  /* scrolling */
  int scroll_limit_y;
  int vscroll_delay;
  int vscroll_speed;

  bool backlight;
  int load_translation;
  bool peakmeter;
  long cue_interval;
  bool karaoke;
  bool readid3;
  bool readsnc;
  bool time_visible;
  struct ButtonFunc button;
  /* colors */
  bool invert_colors;
  unsigned bg_color;
  unsigned fg_color;
  unsigned ia_color;
  unsigned hl_color;
  unsigned wrn_color;
  unsigned tb_elapsed_color;
  unsigned tb_remaining_color;
  unsigned tr_color;
  char lang_file[SNC_MAX_PATH];
  bool tr_use_sysfont;
  bool tr_visible;
}prefs;

static unsigned* color_items[] =
  {&prefs.fg_color, &prefs.bg_color, &prefs.ia_color, &prefs.hl_color, &prefs.wrn_color,
   &prefs.tb_elapsed_color, &prefs.tb_remaining_color, &prefs.tr_color};

static struct SNCText{
  int rows;
  unsigned char* lyrics[MAX_ROWS];
  int total_width;
  int width[MAX_ROWS];
} g_art, g_tit, g_next_at;

static struct SNCSection{
  short type;
  unsigned long time_in_ms;
  struct SNCText x[MAX_TYPE];
}sncs[MAX_SECTIONS];

struct ScrollElement{
  int y;
  const unsigned char* icon;
  int icon_y_offset;
  struct SNCText* text;
};

static struct{
  int num;
  bool auto_insert;
  char files[MAX_FILES2DELETE][MAX_PATH];
} file_list;

static struct FuncItem{
  enum Lang_ID lang_id;
  int(*func) (void);
  bool selected;
  bool allow2nd;
  void* value;
} func_items[]={ /* Position == ID -> append new functions at the end! */
  /* Album art submenu */
  {SNC_L_LRCIIAA, switch_aa_0, false, true, NULL},
  {SNC_L_AANLRC, switch_aa_w, false, true, NULL},
  {SNC_L_AABLRC, switch_aa_s, false, true, NULL},
  {SNC_L_AAIILRC, switch_aa_c, false, true, NULL},
  {SNC_L_AASIZE, set_aa_size, false, false, NULL},
  /* misc */
  {SNC_L_DICTIONARY, lookup_word, false, false, NULL},
  {SNC_L_TRANSLATION, load_translation, false, false, NULL},
  {SNC_L_TIMEOFFSET, set_time_offset, false, false, NULL},
  {SNC_L_REPEAT1, switch_repeat1, false, false, NULL},
  /* Settings submenu*/
  {SNC_L_KARAOKE, switch_karaoke, false, false, &prefs.karaoke},
  {SNC_L_BACKLIGHT, switch_backlight, false, false, &prefs.backlight},
  {SNC_L_PEAKMETER, switch_peakmeter, false, false, &prefs.peakmeter},
  {SNC_L_READID3, switch_readid3, false, false, &prefs.readid3},
  {SNC_L_READSNC, switch_readsnc, false, false, &prefs.readsnc},
  {SNC_L_TRSYSFONT, switch_tr_use_sysfont, false, false, &prefs.tr_use_sysfont},
};

#define SETTINGS_OFFSET 9
#define NUM_SETTINGS_ITEMS 6
#define NUM_AA_ITEMS 5

static unsigned char artist_title_buffer[BUFFERSIZE];
static unsigned char lyrics_buffer[MAX_SECTIONS*BUFFERSIZE];
static int lyrics_buffer_used;
static struct mp3entry* id3;
static unsigned char buf[BUFFERSIZE]; /* temp. buffer */
static unsigned char lyrics_filename[MAX_PATH];
static bool auto_scroll;
static int num_snc;
static int current_snc; /* 0..num_snc-1 */
static int current_snc_edit; /* 0..num_snc-1 */
static int end_snc_edit;

static int start_snc;
static int stop_snc;
static long time_offset;
static int indentwidth, sysfont_height;
static bool utf8, utf8_tmp;
static int modified;
static int g_current_y0 = LCD_HEIGHT;
static bool g_karaoke;

static const unsigned char* FORMATS[]=
  {"lrc","lrc8","snc","txt","ab","tr","mp3","cue"};
static enum e_supported_formats g_ext;
static int scroll_y0;
static int bmp_width;
static int bmp_height;
static char language[3];
static fb_data* snc_backdrop;
static enum e_abfile_state abfile_state;

static bool store_line_(struct SNCText*,const unsigned char*, int);
static void v_scroll(struct SNCText**, int*);

static int snc_div(int divisor, int dividend)
{
  return dividend != 0 ? divisor/dividend : divisor;
}

static void default_backlight(void){
    rb->backlight_set_timeout(rb->global_settings->backlight_timeout);
#if CONFIG_CHARGING
    rb->backlight_set_timeout_plugged(
      rb->global_settings->backlight_timeout_plugged);
#endif
}

static void force_backlight_on(void){
    rb->backlight_set_timeout(0);
#if CONFIG_CHARGING
    rb->backlight_set_timeout_plugged(0);
#endif
}

static void invert_colors(bool invert_bd){
  int i = 0;
  /* don't invert warning and timebar */
  const int num_colors = sizeof(color_items)/sizeof(unsigned*);
  while(i < num_colors){
    if(color_items[i] != &prefs.wrn_color
    && color_items[i] != &prefs.tb_elapsed_color
    && color_items[i] != &prefs.tb_remaining_color
    )
    *(color_items[i]) = ~(*(color_items[i])) & 0xFFFFFF;
    i++;
  }
  if(invert_bd && snc_backdrop != NULL){
    const int num_pixel = LCD_HEIGHT * LCD_WIDTH;
    for(i=0;i<num_pixel;i++){
      snc_backdrop[i] = ~(snc_backdrop[i]) & 0xFFFFFF;
    }
  }
}

static void reset_colors(void){
#ifdef HAVE_LCD_COLOR
  prefs.ia_color = LCD_DARKGRAY;
  prefs.fg_color = LCD_WHITE;
  prefs.bg_color = LCD_BLACK;
  prefs.hl_color = LCD_RGBPACK(255,255,0);
  prefs.wrn_color = LCD_RGBPACK(255,0,0);

  prefs.tb_elapsed_color = LCD_DARKGRAY;
  prefs.tb_remaining_color = LCD_WHITE;
#elif LCD_DEPTH > 1
  prefs.ia_color = LCD_LIGHTGRAY;
  prefs.fg_color = LCD_BLACK;
  prefs.bg_color = LCD_WHITE;
  prefs.hl_color = LCD_DARKGRAY;
  prefs.wrn_color = prefs.hl_color;

  prefs.tb_elapsed_color = LCD_BLACK;
  prefs.tb_remaining_color = LCD_LIGHTGRAY;
#endif
  prefs.tr_color = prefs.fg_color;
}

static void save_settings(const char* filename)
{
  int settings_fd = rb->creat(filename,0666); /* create the settings file */
  if(prefs.invert_colors)
    invert_colors(false); /* don't store inverted colors! */
  rb->write(settings_fd, &prefs, sizeof(struct Preferences));
  rb->write(settings_fd, &(file_list.auto_insert), sizeof(bool));
  rb->close(settings_fd);
  if(prefs.invert_colors)
    invert_colors(false); /* restore colors */
}

static bool load_settings(const char* filename)
{
  struct Preferences tmp_prefs;
  tmp_prefs.version = 0;
  if(prefs.invert_colors)
    invert_colors(true); /* restore colors */
  /* load settings */
  int settings_fd=rb->open(filename, O_RDONLY);
  if (settings_fd >= 0)
  {
    rb->read(settings_fd, &tmp_prefs, sizeof(struct Preferences));
    if(tmp_prefs.version==SNC_CONFIG_VERSION){
      rb->memcpy(&prefs, &tmp_prefs, sizeof(struct Preferences));
      if(filename != SETTINGS_FILE)
        rb->read(settings_fd, &(file_list.auto_insert), sizeof(bool));
    }
    rb->close(settings_fd);
  }
  if(tmp_prefs.version!=SNC_CONFIG_VERSION)
  {
    if(filename!=SETTINGS_FILE)
      return false;
    rb->splash(HZ,STR_(SNC_L_RESET_PREF));
    /* init default settings */
    prefs.backlight = true;
    prefs.button.id[0] = SETTINGS_OFFSET;
    prefs.button.num = 1;
    prefs.cue_interval = 0;
    prefs.fontheight = 0;
    prefs.karaoke = true;
    prefs.load_bmp=0;
    prefs.load_translation = TR_OFF;
    prefs.peakmeter = false;
    prefs.readid3 = false;
    prefs.readsnc = false;
    prefs.sxs_bmp=MIN(LCD_HEIGHT,LCD_WIDTH)>>1;
    prefs.time_visible = true;
    prefs.tr_visible = true;
    prefs.version = SNC_CONFIG_VERSION;
    prefs.vscroll_delay = 6;
    prefs.vscroll_speed = 2;
    prefs.invert_colors = false;
    reset_colors();
    prefs.lang_file[0] = 0;
    prefs.tr_use_sysfont = true;
    prefs.save=PREF_SAVE;
  }
  int i;
  /* assign functions */
  for(i=0;i<prefs.button.num;i++)
    prefs.button.func[i] = func_items[prefs.button.id[i]].func;

  if(prefs.backlight) force_backlight_on();

  int fontheight = rb->font_get(FONT_UI)->height;
  scroll_y0=TIME_BAR_ROW*fontheight+(sysfont_height<<1)+TIMEBAR_LYRICS_SPACE;
  if(fontheight != prefs.fontheight) { /* fontheight was changed! */
    prefs.fontheight = fontheight;
    prefs.scroll_limit_y = scroll_y0; /* scroll_limit_y depends on fontheight */
    prefs.save = PREF_SAVE; /* save the new fontheight */
  }
  if(prefs.invert_colors)
    invert_colors(true);

#if LCD_DEPTH > 1
  rb->lcd_set_background(prefs.bg_color);
  rb->lcd_set_foreground(prefs.fg_color);
#endif
  if(prefs.lang_file[0] == '/' && snc_lang_load(prefs.lang_file))
  {
    prefs.lang_file[0] = 0;
  }
  return true;
}

static int std_list_cb(int action, struct gui_synclist *lists){
  (void) lists;
  return action != ACTION_STD_OK ? action : ACTION_STD_CANCEL;
}
static const char* std_list_name(int item, void * data, char * buf, size_t len){
  (void) buf;
  (void) len;
  return ((char**) data )[item];
}
static int do_list(const char* title, int icon, int count, void* data,
    int (*action_cb)(int, struct gui_synclist *), int selected, int num_func)
{
  enum themable_icons get_list_icon(int item, void * data){
    (void) data;
    return item < num_func ? icon : Icon_Questionmark;
  }

  struct simplelist_info list;
  rb->simplelist_info_init(&list, (char*) title, count, data);
  list.get_icon = get_list_icon;
  list.get_name = std_list_name;
  list.action_callback = action_cb != NULL ? action_cb : std_list_cb;
  list.selection = selected;
  rb->simplelist_show_list(&list);
  return list.selection;
}

static int list_dir(const char* title, const char* filter, int icon,
  void (*cb)(int, const char**))
{
  /* list dictionaries */
  DIR* dir = rb->opendir(SNCLRC_DATA_DIR);
  if (!dir) {
    rb->splashf(HZ,STR_(SNC_L_OPEN_ERROR),SNCLRC_DATA_DIR);
    return RET_NOK;
  }
  #define MAX_ITEMS 20
  char* items[MAX_ITEMS];
  size_t size;
  items[0]=rb->plugin_get_buffer(&size);
  struct dirent *entry;
  unsigned char* ext;
  int i=0;
  while((entry=rb->readdir(dir))!=0 && i<MAX_ITEMS){
    ext=rb->strrchr(entry->d_name,'.');
    if(ext == NULL) continue; /* directory */
    ext++;
    if(rb->strncasecmp(ext,filter,3)==0){
      rb->strcpy(items[i], entry->d_name);
      items[i+1]=items[i]+rb->strlen(entry->d_name)+1;
      i++;
    }
  }
  rb->closedir(dir);
  if(i==0) return RET_NOK;

  i = do_list(title, icon, i, items, NULL, 0, i);
  if(i>=0)
  {
    cb(i, (const char**)items);
    return RET_OK;
  }
  return RET_NOK;
}

static unsigned long bytes2int(unsigned long b0, unsigned long b1,
		     unsigned long b2, unsigned long b3)
{
 return (((long)(b0 & 0xFF) << 24) | /* 3*8 */
         ((long)(b1 & 0xFF) << 16) | /* 2*8 */
         ((long)(b2 & 0xFF) << 8) |  /* 1*8 */
         ((long)(b3 & 0xFF)));       /* 0*8 */
}

static int lrccmp(const char *str1, const char *str2){
  return str1==str2 ? 0 : rb->strcasecmp(str1,str2);
}

static void iso2utf8(const unsigned char *iso, unsigned char *utf8_buf,
    unsigned int len){
  !utf8 ? rb->iso_decode(iso, utf8_buf, -1, len) : rb->memcpy(utf8_buf,iso,len);
}
/* no time */
static void snc_copy_(struct SNCText* dest,const struct SNCText* src){
  dest->rows=src->rows;
  dest->total_width = src->total_width;
  int r=0;
  for(;r<src->rows;r++){
    dest->lyrics[r]=src->lyrics[r];
    dest->width[r] = src->width[r];
  }
}
/* copy with time [offset] */
static void snc_copy(struct SNCSection* dest,const struct SNCSection* src,int offset){
  dest->time_in_ms=src->time_in_ms+offset;
  dest->type = src->type;
  COPY_SNC(dest,src);
}

static char* cat_time(char* str,unsigned long time)
{
  return (time!=MAX_ULONG) ?
    &str[rb->snprintf(str,6,TIME_FORMAT,(int)(CALC_MM(time)),(int)(CALC_SS(time)))]:
    rb->strcpy(str,"?:??")+4;
}

static void exec_func(int delay, void (*func)(void)){
  #define MAX_FUNCS 2
  /* 2: update_status_display, print_next_playing */
  static struct {
    int t;
    void (*exec)(void);
  } funcs[MAX_FUNCS]={{-1,NULL},{-1,NULL}};
  static int size=0;
  static bool busy=false;
  if(busy) return; /* a func is beeing executed, don't mess up */
  int i=0;
  int free=-1;
  if(func!=NULL){ /* add func to queue */
    if(size>0){
      for(;i<MAX_FUNCS;i++){ /* is the function already in the list? */
        if(free<0 && funcs[i].t<0) free=i; /* free place */
        if(funcs[i].exec==func){
          size--; /* -+ => 0 */
          break;
        }
      }
      if(i==MAX_FUNCS){ /* is a new func -> assign to a free place */
        i = free>=0 ? free : 0; /* overwrite first if queue is too small */
      }
    }
    funcs[i].t= delay << (g_karaoke ? 2 : 0);
    funcs[i].exec=func;
    size++;
    //DEBUGF("func %p added, num func in queue: %d\n",func,size);
  }
  if(size>0){ /* there are functions in the queue waiting */
    for(i=0;i<MAX_FUNCS;i++){
      if(funcs[i].t==0){
        busy=true;
        size--;
        funcs[i].exec();
        //DEBUGF("func %p executed, num func in queue: %d\n",funcs[i].exec,size);
        funcs[i].exec=NULL;
        busy=false;
      }
      funcs[i].t--;
    }
  }
}

static void store_artist_title(struct SNCText* at, const char* txt){
  static int used = 0;
  at->rows = 0;
  if(txt == NULL){
    at->lyrics[0] = 0;
    return;
  }

  int len = rb->strlen(txt)+1;
  if(at == &g_art || len+used >= BUFFERSIZE) used = 0;
  char* buffer = artist_title_buffer + used;
  rb->memcpy(buffer, txt, len);
  at->lyrics[0] = buffer;
  store_line_(at,buffer,LCD_WIDTH-ICON_WIDTH);
  used += len;
}

static int print_text_line(struct SNCText* snc, int r,
  void (*lcd_puts)(int, int, const unsigned char*),int x, int y,
  int min_x){
  char c=0;
  if(r+1<snc->rows){
    c=*(snc->lyrics[r+1]); /* backup */
    *(snc->lyrics[r+1])=0;
  }
  if(x < 0)
    x =(LCD_WIDTH-snc->width[r])>>1;
  if(x < min_x)
    x = min_x;
  lcd_puts(x,y,snc->lyrics[r]);
  /* restore */
  if(r+1<snc->rows) *(snc->lyrics[r+1])=c;
  return x;
}

static void print_artist_title_line(struct ScrollElement* e, int r){
  int x = print_text_line(e->text, r, rb->lcd_putsxy, -1, e->y, ICON_WIDTH);

  if(r == 0)
    rb->lcd_mono_bitmap(e->icon, x-ICON_WIDTH, e->y + e->icon_y_offset, 7, ICON_HEIGHT);
  else
    rb->lcd_mono_bitmap(LEFT_ICON, 0,
      e->y + e->icon_y_offset, SCROLL_MARKER_WIDTH, ICON_HEIGHT);

  if(r+1 < e->text->rows){
    rb->lcd_mono_bitmap(RIGHT_ICON, SCROLL_MARKER_X,
      e->y + e->icon_y_offset, SCROLL_MARKER_WIDTH, ICON_HEIGHT);
  }
}

static int print_lyrics_line(struct SNCText* snc, int r, short snc_type,
  void (*lcd_puts)(int, int, const unsigned char*),
  int x, int y, bool set_tr_color){
  if(snc_type == TYPE_TR){
    if(prefs.tr_use_sysfont)
      rb->lcd_setfont(FONT_SYSFIXED);
#if LCD_DEPTH > 1
    if(set_tr_color)
      rb->lcd_set_foreground(prefs.tr_color);
#else
  (void) set_tr_color;
#endif
  }
  print_text_line(snc, r, lcd_puts, x, y, 0);
  if(snc_type == TYPE_TR){
#if LCD_DEPTH > 1
    if(set_tr_color)
      rb->lcd_set_foreground(prefs.fg_color);
#endif
    rb->lcd_setfont(FONT_UI);
  }
  return snc_type == TYPE_TR && prefs.tr_use_sysfont ?
    sysfont_height:prefs.fontheight;
}

static void print_next_playing(void){
  if(!auto_scroll) return;
  if(g_next_at.rows==0){ /* not assigned yet */
    const struct mp3entry* nextid3 = rb->audio_next_track();
    if(IS_ID3_VALID(nextid3)){
      unsigned char* buffer=buf;
      const unsigned char* art=nextid3->artist;
      const unsigned char* tit=nextid3->title;
      if(art!=NULL && tit!=NULL){
        if(g_art.lyrics[0] == NULL || rb->strcmp(art, g_art.lyrics[0])!=0)
          rb->snprintf(buf,BUFFERSIZE,"%s - %s", tit, art);
        else
          rb->strcpy(buf, tit); /* only title */
      }
      else /* no artist and no title -> display filename */
        buffer=rb->strrchr(nextid3->path,'/')+1;
      store_artist_title(&g_next_at, buffer);
    }
  }
  if(g_next_at.rows>0){
    struct SNCText* text = &g_next_at;
    v_scroll(&text,NULL);
  }
}
static void draw_volume_icon(void){
  int vol = rb->global_settings->volume;
  unsigned char* volume_icon=volume_icons[1]; /* mute */
  if(vol>-60){
    volume_icon=volume_icons[0];
    if(vol>-15){
      volume_icons[0][6]=volume_values[5];
#if LCD_DEPTH > 1
      rb->lcd_set_foreground(prefs.wrn_color);
#endif
    }
    else if(vol>-25) volume_icons[0][6]=volume_values[4];
    else if(vol>-30) volume_icons[0][6]=volume_values[3];
    else if(vol>-40) volume_icons[0][6]=volume_values[2];
    else if(vol>-50) volume_icons[0][6]=volume_values[1];
    else volume_icons[0][6]=volume_values[0];  /* >20 */
  }
  rb->lcd_mono_bitmap(volume_icon,STATUS_VOL_POS,0,7,ICON_HEIGHT);
#if LCD_DEPTH > 1
  rb->lcd_set_foreground(prefs.fg_color);
#endif
}

static void update_status_display(void)
{
  rb->lcd_setfont(FONT_SYSFIXED);
  LCD_CLEAR_AREA(SCREEN_MAIN,0,0,LCD_WIDTH,sysfont_height);
  if(prefs.load_translation)
    rb->lcd_mono_bitmap(TR_ICON,STATUS_TR_POS,0,7,ICON_HEIGHT);
  if(modified){
#if LCD_DEPTH > 1
    rb->lcd_set_foreground(prefs.wrn_color);
#endif
    const unsigned char modified_icon[]={0x4f,0x6f}; /* modified*/
    rb->lcd_mono_bitmap(modified_icon,STATUS_MODIFIED_POS,0,2,ICON_HEIGHT);
    if(time_offset!=0){
      unsigned char sign_icon[]={0x08,0x08,0x3e,0x08,0x08}; /* + */
      if(time_offset<0) sign_icon[2]= 0x08; /* - */
      rb->lcd_mono_bitmap(sign_icon,STATUS_MODIFIED_POS+3,0,5,ICON_HEIGHT);
    }
#if LCD_DEPTH > 1
    rb->lcd_set_foreground(prefs.fg_color);
#endif
  }
#if LCD_DEPTH > 1
  if(id3->vbr && id3->codectype==AFMT_MPA_L3){ /* sync problems with vbr mp3 */
    rb->lcd_set_foreground(prefs.wrn_color);
    rb->lcd_mono_bitmap(VBR_ICON,STATUS_VBR_POS,0,8,ICON_HEIGHT);
    rb->lcd_set_foreground(prefs.fg_color);
  }
#endif
  if(rb->audio_status() & AUDIO_STATUS_PAUSE)
    rb->lcd_mono_bitmap(PAUSE_ICON,STATUS_PAUSE_POS,0,7,ICON_HEIGHT);
  if(prefs.load_bmp)
    rb->lcd_mono_bitmap(bitmap_icons_7x8[prefs.load_bmp+AA_ICON_OFFSET]
      ,STATUS_AA_POS,0,7,ICON_HEIGHT);

  if(utf8) rb->lcd_mono_bitmap(UTF8_ICON,STATUS_UTF8_POS,0,8,ICON_HEIGHT);

  if(file_list.num>0){
    rb->snprintf(buf,BUFFERSIZE,"%d", file_list.num);
    rb->lcd_putsxy(STATUS_DELBOX_POS,0,buf);
  }

  draw_volume_icon();

#ifdef CUSTOM_PLUGIN_PATCH
  rb->snprintf(buf,BUFFERSIZE,"%d/%d",rb->playlist_get_display_index(),
      rb->playlist_amount());
  rb->lcd_putsxy(LCD_WIDTH-rb->strlen(buf)*6-12,0,buf);
#endif

  rb->lcd_mono_bitmap(bitmap_icons_7x8[rb->global_settings->repeat_mode],
                      STATUS_REPEAT_MODE_POS,0,7,ICON_HEIGHT);

  rb->lcd_setfont(FONT_UI);
  rb->lcd_update_rect(0,0,LCD_WIDTH,sysfont_height);

  if(IS_ID3_VALID(rb->audio_next_track()))
    print_next_playing(); /* show next playing immediately if available */
  exec_func(10,print_next_playing);  /* update next playing */
}

static void update_status_busy(int action){
  static int last_action = ACTION_NONE;
  if(last_action == action || ACTION_UNKNOWN == action
     || SNC_INC == action || SNC_DEC == action)
    return;
  last_action = action;
  if(action != ACTION_NONE){
    rb->lcd_mono_bitmap(BUSY_ICON,STATUS_BUSY_POS,0,7,ICON_HEIGHT);
    rb->lcd_update_rect(STATUS_BUSY_POS, 0, 7, ICON_HEIGHT);
  }
  else
    exec_func(1, update_status_display);
}

static void draw_ab(int x, int y, int snc){
  if(sncs[snc].time_in_ms==(unsigned) NOT_INIT) return;
  rb->lcd_set_drawmode(DRMODE_BG|DRMODE_INVERSEVID);
  //rb->lcd_set_drawmode(DRMODE_COMPLEMENT);
  rb->lcd_mono_bitmap( snc==start_snc ? RIGHT_ICON : LEFT_ICON, x, y-3,
    SCROLL_MARKER_WIDTH, ICON_HEIGHT);
  rb->lcd_set_drawmode(DRMODE_SOLID);
}

static void draw_bar(int x1, int x2, int y, int x_current){
#if LCD_DEPTH > 1
  //remaining
  rb->lcd_set_foreground(prefs.tb_remaining_color);
  rb->lcd_fillrect(x_current,y,1+x2-x_current,3);
  //frame
  rb->lcd_set_foreground(prefs.tb_elapsed_color);
  rb->lcd_hline(x1,x2,y-1);
  rb->lcd_vline(x1-1,y,y+1);
  rb->lcd_vline(x2+1,y,y+1);
  rb->lcd_drawpixel(x1,y);
  rb->lcd_drawpixel(x2,y);
  //elapsed
  if(x_current>x1){
    rb->lcd_fillrect(x1,y,x_current-x1,3);
    rb->lcd_set_foreground(prefs.tb_remaining_color);
    rb->lcd_hline(x1+1,x_current-2,y);

    rb->lcd_set_foreground(prefs.bg_color);
    rb->lcd_vline(x_current,y-1,y+2);
  }
  rb->lcd_set_foreground(prefs.fg_color);
#else
  rb->lcd_fillrect(x_current,y,1+x2-x_current,3);
  //frame
  rb->lcd_hline(x1,x2,y-1);
  rb->lcd_vline(x1-1,y,y+1);
  rb->lcd_vline(x2+1,y,y+1);
  rb->lcd_drawpixel(x1,y);
  rb->lcd_drawpixel(x2,y);
  //elapsed
  if(x_current>x1){
    rb->lcd_hline(x1+1,x_current-2,y);
    rb->lcd_vline(x_current,y-1,y+2);
  }
#endif
}

static long update_time_display(long time_in_ms, bool force){
  if(time_in_ms < 0) return 0;
  else if(time_in_ms > (long) id3->length) return id3->length;

  if(!force){
    static unsigned int old;
    unsigned int time = time_in_ms/1000;
    if(old==time) return time_in_ms; /* update every second */
    old=time;
  }

  int y=auto_scroll ? TIME_BAR_ROW*prefs.fontheight+sysfont_height:
    LCD_HEIGHT - sysfont_height;

  LCD_CLEAR_AREA(SCREEN_MAIN,0,y,LCD_WIDTH,sysfont_height);
  cat_time(buf,id3->length); /* getstringsize from length: can be >9 min */
  rb->lcd_setfont(FONT_SYSFIXED);
  int x1;
  rb->lcd_getstringsize(buf, &x1, NULL);
  x1 += rb->font_get(FONT_SYSFIXED)->maxwidth;

  int x2 = LCD_WIDTH-x1;
  rb->lcd_putsxy(x2 + rb->font_get(FONT_SYSFIXED)->maxwidth,y,buf);
  cat_time(buf,time_in_ms); /* update elapsed time */
  rb->lcd_putsxy(0,y,buf);
  rb->lcd_setfont(FONT_UI);

  /* timebar */
  const int width=x2-x1;
  y+=(sysfont_height>>1)-1;

  draw_bar(x1,x2,y,x1+snc_div(width*time_in_ms,id3->length));

  /* dots */
  int i=1;
  for(;i<num_snc-1;i++){
    rb->lcd_drawpixel(snc_div(width * sncs[i].time_in_ms, id3->length) + x1,
      !IS_BLANK(i) ? y+4 : y-3);
  }
  if(!auto_scroll){
    rb->lcd_vline(x1+snc_div(width*sncs[current_snc_edit].time_in_ms, id3->length),
      y-3,y+4); /* | */
  }
  /* ab */
  if(start_snc!=NOT_INIT)
    draw_ab(x1+snc_div(width*sncs[start_snc].time_in_ms, id3->length),y,start_snc);
  if(stop_snc!=NOT_INIT)
    draw_ab(x1+snc_div(width*sncs[stop_snc].time_in_ms, id3->length-1),y,stop_snc);

  rb->lcd_update_rect(0,y-4,LCD_WIDTH,sysfont_height+1);
  return time_in_ms;
}

static void browse_snc(void)
{
  const int max_height = LCD_HEIGHT - sysfont_height;
  LCD_CLEAR_AREA(SCREEN_MAIN,0,sysfont_height,LCD_WIDTH,max_height-sysfont_height);
  int id, y;
  if(end_snc_edit == MAX_SECTIONS){
    /* calculate end_snc_edit */
    id = num_snc-1;
    y = sysfont_height;
    do{
      if(sncs[id].type == TYPE_TR && prefs.tr_visible == false)
        continue;
      y += sncs[id].x[EDIT].rows *
      (sncs[id].type==TYPE_TR && prefs.tr_use_sysfont ? sysfont_height : prefs.fontheight);
    }while(y <= max_height && --id >= 0);
    end_snc_edit = id+1;
    if(sncs[end_snc_edit].type == TYPE_TR && prefs.tr_visible == false)
      end_snc_edit++;
  }
  int r, mark_y, mark_height = prefs.fontheight;

  id = current_snc_edit <= end_snc_edit ?
    current_snc_edit-1:end_snc_edit;
  if(sncs[id].type == TYPE_TR && prefs.tr_visible == false)
    id--;
  y = mark_y = sysfont_height;
  if(id<1){
    rb->lcd_putsxy(0, sysfont_height,g_tit.lyrics[0]);
    id=1;
    y+=prefs.fontheight;
  }

  struct SNCText* snc;
  char* end;
  int mark_pos;
  mark_pos = indentwidth - rb->font_get(FONT_UI)->maxwidth + 1;
  while(id<num_snc){
    snc=&(sncs[id].x[EDIT]);
    if(sncs[id].type == TYPE_TR && prefs.tr_visible == false){
      id++; /* skip translation */
      continue;
    }
    r=0;
    if(id==current_snc_edit) mark_y=y;
    while(r<snc->rows)
    {
      end = buf;
      if(id<stop_snc && id>=start_snc){
        rb->lcd_putsxy(mark_pos,y,"|");
        *end = 0;
      }
      if(r==0 && sncs[id].type==TYPE_STD){
        if(prefs.time_visible)
          end = cat_time(buf,sncs[id].time_in_ms);
        else if(*end != 0)
          *end = '.';
        if(id==start_snc) *end='>';
        else if(id==stop_snc) *end='<';
        else if(id==current_snc) *end=MARK;
        *(end+1)=0;
        rb->lcd_putsxy(0,y,buf);
      }
      y+=print_lyrics_line(snc,r,sncs[id].type,rb->lcd_putsxy,indentwidth,y,true);
      if(id==current_snc_edit) mark_height=y-mark_y;
      if(y + prefs.fontheight > max_height)
        goto END_BROWSE_SNC; /* quit the nested loop */
      r++;
    }
    id++;
  }
  END_BROWSE_SNC:
  /* invert current_snc_edit */
  rb->lcd_set_drawmode(DRMODE_COMPLEMENT);
  rb->lcd_fillrect(0, mark_y, LCD_WIDTH, mark_height);
  rb->lcd_set_drawmode(DRMODE_SOLID);
  update_time_display(AUDIO_ELAPSED(), true);
  rb->lcd_update();
}

static void v_scroll(struct SNCText** c, int* r){
  #define MAX_SCROLL_ELEMENTS 3
  if(r!=NULL &&
    (g_art.rows < 2 && g_tit.rows < 2 &&  g_next_at.rows < 2))
    return;

  struct SNCText* current = *c;
  int row = r!=NULL ? *r : -1;
  const int min_r = r!=NULL ? 1 : 0;

  struct ScrollElement elements[MAX_SCROLL_ELEMENTS];
  struct ScrollElement* e = NULL;
  int num_elements = 0;
  /* fill array with elements */
  if(g_art.rows > min_r){
    e = &elements[num_elements];
    SET_SCROLL_ELEMENT(e, ARTIST_ICON, g_art, sysfont_height,0);
    num_elements++;
  }
  if(g_tit.rows > min_r){
    e = &elements[num_elements];
    SET_SCROLL_ELEMENT(e, NOTE_ICON, g_tit, sysfont_height+prefs.fontheight,0);
    num_elements++;
  }
  if(g_next_at.rows > min_r){
    e = &elements[num_elements];
    SET_SCROLL_ELEMENT(e, NEXT_ICON, g_next_at,
      LCD_HEIGHT-prefs.fontheight, (prefs.fontheight-ICON_HEIGHT)>>1
    );
    num_elements++;;
  }
  if(num_elements == 0) return;
  /* get current pos from array */
  int i = 0;
  while(i<num_elements){
    if(elements[i].text == current){
      e = &elements[i];
      break;
    }
    i++;
  }
  if(i==num_elements){
    /* current has only one line -> change to the first with more than 1 line */
    i = 0;
    current = elements[0].text;
  }
   /* next */
  if(row+1 >= current->rows && num_elements>1){
    LCD_CLEAR_AREA(SCREEN_MAIN,0,e->y,LCD_WIDTH, prefs.fontheight);
    print_artist_title_line(e, 0);
    rb->lcd_update_rect(0,e->y,LCD_WIDTH, prefs.fontheight);

    e = &elements[(i + 1)%num_elements];
    current = e->text;
    row = 0;
  }
  else{
    row = (row + 1)%current->rows;
  }
  LCD_CLEAR_AREA(SCREEN_MAIN,0,e->y,LCD_WIDTH, prefs.fontheight);
  print_artist_title_line(e, row);
  rb->lcd_update_rect(0,e->y,LCD_WIDTH, prefs.fontheight);

  if(r!=NULL){
    *c = current;
    *r = row;
  }
}

static void v_scroll_artist_title(bool reset){
  if(!auto_scroll || prefs.vscroll_speed == 0) return;
  static int current_row=0;
  static struct SNCText* current = &g_art;
  static int wait = -1;
  if(reset){
    current_row = 0;
    wait = prefs.vscroll_delay;
    current = &g_art;
    return;
  }
  if(wait < 0) wait = prefs.vscroll_delay;
  if(wait == 0){
    v_scroll(&current, &current_row);
    wait = current_row==0 ? prefs.vscroll_delay : prefs.vscroll_speed;
    wait <<= (g_karaoke ? 2 : 0);
  }
  wait--;
}

static void base_display(void)
{
  rb->lcd_clear_display();
  update_status_display();
  struct SNCText* text = &g_art;
  v_scroll(&text, NULL);
  text = &g_tit;
  v_scroll(&text, NULL);

#ifdef HAVE_REMOTE_LCD
  rb->lcd_remote_clear_display();
  rb->snprintf(buf,BUFFERSIZE,"%s/%s", g_art.lyrics[0], g_tit.lyrics[0]);
  rb->lcd_remote_puts_scroll(0,0,buf);
  rb->lcd_remote_update();
#endif
  update_time_display(AUDIO_ELAPSED(), true);
}
/*
  Modes:
  0: draw bitmap from buffer
  1: draw bitmap from file
  2: no loading/drawing; only check and set size
*/
static void load_bmp(int mode){
  if(mode == 0 && bmp_width <= 0) return;
#ifdef HAVE_ALBUMART
  #define BITMAP_BUFFERSIZE 97200 /* max 180 x 180 colour image */
  static unsigned char bitmap_buffer[BITMAP_BUFFERSIZE];
  static struct bitmap bm;
  static int bmp_size = 0;
  const int height = LCD_HEIGHT - (scroll_y0 + prefs.fontheight + 2);
  if(mode > 0){
    if(!rb->search_albumart_files(id3,"",buf,BUFFERSIZE)){
      rb->strcpy(buf, SNCLRC_DATA_DIR"/"APP_NAME".jpg");
    }
    bm.data = bitmap_buffer;
    if(prefs.load_bmp == BMP_CENTER || NO_LYRICS())
    {
      bm.width = LCD_WIDTH;
      bm.height = height;
    }
    else
      bm.width = bm.height = prefs.sxs_bmp;
    if(prefs.peakmeter && LCD_WIDTH - bm.width < (MIN_WIDTH_PEAKMETER << 1)){
      bm.width = LCD_WIDTH - (MIN_WIDTH_PEAKMETER << 2);
    }
    if(mode == 1){
      bmp_size=read_image_file(buf, &bm, BITMAP_BUFFERSIZE,
        FORMAT_NATIVE|FORMAT_RESIZE|FORMAT_KEEP_ASPECT, NULL);
    }
  }
  if(bmp_size>0 || mode == 2){
    bmp_height = prefs.load_bmp==BMP_SOUTH ? bm.height + 2 : NOT_INIT; /* hack */
    bmp_width = bm.width+3;
    if(mode == 2) return;
    int x,y;
    if(prefs.load_bmp==BMP_WEST && !NO_LYRICS()){
      x = 0;
      y = scroll_y0 + ((height - bm.height)/3);
    }
    else{
      x=(LCD_WIDTH-bm.width)>>1;
      y = prefs.load_bmp==BMP_SOUTH && !NO_LYRICS()? LCD_HEIGHT -
         (prefs.fontheight + bmp_height) : scroll_y0;
    }
#if LCD_DEPTH > 1
    rb->lcd_bitmap((fb_data*) bm.data, x , y, bm.width, bm.height);
#endif
  }
  else
  {
    bmp_width=NOT_INIT; /* cannot load bitmap */
    bmp_height=NOT_INIT;
  }
#endif //HAVE_ALBUMART
}

static void peak_meter(void){
  int y=scroll_y0;
  int width = bmp_width != NOT_INIT ?
      ((LCD_WIDTH-bmp_width)/5)<<1 : LCD_WIDTH >> 3;
  if(width < MIN_WIDTH_PEAKMETER)
    width = MIN_WIDTH_PEAKMETER;
  const int max_height=bmp_height == NOT_INIT ?
    LCD_HEIGHT-y-prefs.fontheight-3 : bmp_height;
  if(prefs.load_bmp==BMP_SOUTH && !NO_LYRICS())
    y = LCD_HEIGHT - (prefs.fontheight + bmp_height);

  int left_peak,right_peak;
#if (CONFIG_CODEC == MAS3587F) || (CONFIG_CODEC == MAS3539F)
  left_peak = rb->mas_codec_readreg(0xC);
  right_peak = rb->mas_codec_readreg(0xD);
#elif (CONFIG_CODEC == SWCODEC)
  rb->pcm_calculate_peaks(&left_peak, &right_peak);
#endif
  /* clear */
  LCD_CLEAR_AREA(SCREEN_MAIN,0,y,width,max_height);
  int xr=LCD_WIDTH-width;
  LCD_CLEAR_AREA(SCREEN_MAIN,xr,y,width,max_height);
  /* normalize */
  left_peak=(max_height*left_peak)>>14; /* scale: 4x */
  right_peak=(max_height*right_peak)>>14;

  /* draw bars */
  const int bar_height=max_height>>3; /* max: 8 bars */
  const int num_bars_l=left_peak/bar_height;
  const int num_bars_r=right_peak/bar_height;
  int w2_l=width-2;
  int w2_r=width-2;
  int y2=y+7*bar_height; /* +max_height-bar_height; */
  const int bar_frame_height=bar_height-1;
  const int bar_content_height=bar_height-3;
  rb->lcd_set_drawmode(DRMODE_COMPLEMENT);
  int i=0;
  while(i<8){
    if(i<num_bars_l){
      rb->lcd_fillrect(1,y2+1,w2_l,bar_content_height);
      w2_l-=(width/num_bars_l)+1;
    }
    if(i<num_bars_r){
      rb->lcd_fillrect(xr+width-w2_r-1,y2+1,w2_r,bar_content_height);
      w2_r-=(width/num_bars_r)+1;
    }
    rb->lcd_drawrect(0,y2,width,bar_frame_height);
    rb->lcd_drawrect(xr,y2,width,bar_frame_height);
    y2-=bar_height;
    i++;
  }
  rb->lcd_set_drawmode(DRMODE_SOLID);
  rb->lcd_update_rect(0,y,width,max_height);
  rb->lcd_update_rect(xr,y,width,max_height);
}

static void scroll_snc(void)
{
  static int current_y0 = LCD_HEIGHT;
  static int reload_bmp = 0;
  int r;
  const int x = bmp_width != NOT_INIT && prefs.load_bmp == BMP_WEST ?
                bmp_width : NOT_INIT; /* NOT_INIT -> center */
  const int max_y = LCD_HEIGHT - (prefs.fontheight + prefs.fontheight +
                    (bmp_height == NOT_INIT ? 0 : bmp_height));

  LCD_CLEAR_AREA(SCREEN_MAIN,
      x >= 0 ? x:0, scroll_y0,
      LCD_WIDTH,
      reload_bmp ?
      LCD_HEIGHT - prefs.fontheight - scroll_y0 :
      max_y + prefs.fontheight - scroll_y0
  );

  if(reload_bmp > 0){
    if(reload_bmp == 2)
      prefs.peakmeter = true;
    reload_bmp = 0;
    load_bmp(0);
  }

  /* show the vertical scroll limit */
#if LCD_DEPTH > 1
  rb->lcd_set_foreground(prefs.ia_color);
#endif
  rb->lcd_mono_bitmap(LEFT_ICON, SCROLL_MARKER_X,
    prefs.scroll_limit_y + ((prefs.fontheight-ICON_HEIGHT)>>1),
    SCROLL_MARKER_WIDTH, ICON_HEIGHT);
#if LCD_DEPTH > 1
  rb->lcd_set_foreground(prefs.fg_color);
#endif

  int id = current_snc;
  struct SNCText* snc = &(sncs[id].x[SCROLL]);
  int tr_height = sncs[id+1].type == TYPE_TR ?
    sncs[id+1].x[SCROLL].rows * (prefs.tr_use_sysfont ? sysfont_height : prefs.fontheight)
    :
    0;
  /* position the current lyrics line */
  if(current_y0 > prefs.scroll_limit_y)
    current_y0 = prefs.scroll_limit_y;
  if((max_y < current_y0 + (snc->rows * prefs.fontheight) + tr_height) /* height exceeded */
     || id < 2 /* id=0: blank; id=1: 1st */
     || (IS_BLANK(id) &&
          sncs[id+1].time_in_ms >= sncs[id].time_in_ms + BREAK_LENGTH)
     || (id>0 && IS_BLANK(id-1) && (scroll_y0 + prefs.fontheight == current_y0)))
    current_y0 = scroll_y0;

  g_current_y0 = current_y0;
  /* printing the lines on screen */
  int y=current_y0;
  int new_current_y0 = current_y0;
  bool current_tr;
  while(id<num_snc){
    for(r=0;r<snc->rows;r++){
      current_tr = sncs[id].type==TYPE_TR && id==current_snc+1;
      if(y>max_y){
        if(id!=current_snc && !current_tr)
          goto END_PRINT_LINES; /* quit the nested loop */
        else if(y>max_y+3 && reload_bmp==0){ /* 3 pixel tolerance: bmp_height = bm.height + 2 */
          if(prefs.peakmeter){
            reload_bmp = 2;
            prefs.peakmeter = false; /* turn off peakmeter to avoid overlapping */
          }
          else
            reload_bmp = 1;
        }
      }
      y+=print_lyrics_line(snc,r,sncs[id].type,rb->lcd_putsxy,x,y,current_tr);
    }
    if(sncs[id+1].type==TYPE_STD){
#if LCD_DEPTH > 1
      rb->lcd_set_foreground(prefs.ia_color);
#endif
      if(sncs[id].time_in_ms==sncs[current_snc].time_in_ms){
        new_current_y0 = y;
      }
    }
    snc=&(sncs[++id].x[SCROLL]);
  }
  END_PRINT_LINES:
  /* show previous lines */
  y = current_y0;
  id = current_snc;
  while(id>1){
    snc=&(sncs[--id].x[SCROLL]);
    for(r=snc->rows-1; r>=0; r--){
      y-= sncs[id].type == TYPE_TR && prefs.tr_use_sysfont ?
        sysfont_height:prefs.fontheight;
      if(y<scroll_y0) goto END_PRINT_PREVIOUS_LINES;
      print_lyrics_line(snc,r,sncs[id].type,rb->lcd_putsxy,x,y,false);
    }
  }
  END_PRINT_PREVIOUS_LINES:
  current_y0 = new_current_y0;
#if LCD_DEPTH > 1
  rb->lcd_set_foreground(prefs.fg_color); /* restore foreground color */
#endif
#ifdef HAVE_REMOTE_LCD
  snc=&(sncs[current_snc].x[RC]);
  LCD_CLEAR_AREA(SCREEN_REMOTE,0,prefs.fontheight,
    LCD_REMOTE_WIDTH,LCD_REMOTE_HEIGHT-prefs.fontheight);
  const int rc_max_line=LCD_REMOTE_HEIGHT/prefs.fontheight;
  for(r=0;r<snc->rows && r<rc_max_line;r++){
    print_lyrics_line(snc,r,TYPE_STD,rb->lcd_remote_putsxy,0,
      r*prefs.fontheight + prefs.fontheight, false);
  }
#endif
  FOR_NB_SCREENS(r) rb->screens[r]->update();
}

static void update_display(void)
{
#if LCD_DEPTH > 1
  if(rb->lcd_get_backdrop()!=snc_backdrop)
    rb->lcd_set_backdrop(snc_backdrop);
#endif
  if(!auto_scroll && num_snc>0){
    update_status_display();
    browse_snc();
  }
  else{
    base_display();
    if(!NO_LYRICS()){
      if(prefs.load_bmp)
        load_bmp(0);
      scroll_snc();
    }
    else{
#ifdef HAVE_REMOTE_LCD
      LCD_CLEAR_AREA(SCREEN_REMOTE,0,prefs.fontheight,
        LCD_REMOTE_WIDTH,LCD_REMOTE_HEIGHT-sysfont_height);
      rb->lcd_remote_putsxy(0,prefs.fontheight,"Lyrics?");
#endif
      load_bmp(0);
      int i;
      FOR_NB_SCREENS(i) rb->screens[i]->update();
    }
  }
}

static void set_repeat_mode(int mode){ /* 0=off 1=repeat all 2=repeat one 3=shuffle 4=ab */
  if(rb->global_settings->repeat_mode+1==mode) return;
  static int orig_repeat_mode = NOT_INIT;
  if(orig_repeat_mode == NOT_INIT)
    orig_repeat_mode = rb->global_settings->repeat_mode;

  rb->global_settings->repeat_mode =
    mode==RMODE_CLEAR ? orig_repeat_mode : (mode-1);

  rb->audio_flush_and_reload_tracks();
  exec_func(5,update_status_display);
}
static int open_file(enum e_supported_formats type,int flags)
{
  char* filename = lyrics_filename;
  if(type==EXT_AB || type==EXT_TR) filename=buf;
  rb->strcpy(filename,id3->path);
  char* ext=rb->strrchr(filename,'.')+1;
  rb->strcpy(ext,FORMATS[type]);
  int fd=rb->open_utf8(filename, flags);
  if(fd >= 0){
    utf8 |= rb->lseek(fd,0,SEEK_SET) > 0;
    switch(type){
      case EXT_LRC8: utf8=true; break;
      case EXT_LRC:{
        unsigned short ucs=0;
        const int idlen=sizeof(ucs);
        rb->read(fd,&ucs,idlen);
        if(ucs != 0xFEFF && ucs != 0xFFFE) rb->lseek(fd,-idlen,SEEK_CUR);
        else{
          rb->splash(HZ,STR_(SNC_L_2UTF8));
          int len=rb->read(fd,lyrics_buffer,rb->filesize(fd)-idlen);
          rb->close(fd);
          size_t size;
          unsigned char* buffer=(unsigned char*) rb->plugin_get_buffer(&size);
          unsigned char* end=rb->utf16LEdecode(lyrics_buffer,buffer,len>>1);
          rb->remove(filename);
          rb->strcat(filename,"8");
          fd=rb->open(filename, O_WRONLY|O_CREAT|O_TRUNC);
          rb->write(fd, buffer,end-buffer);
          rb->close(fd);
          fd=-1; /* loading order: lrc, lrc8 -> lrc fails,
                    try to open the newly converted lrc8 */
        }
        break;
      }
      default:;
    }
  }
  return fd;
}

static int clear_ab(void){
  start_snc=NOT_INIT;
  stop_snc=NOT_INIT;
  set_repeat_mode(RMODE_CLEAR);/* reset repeat ab */
  return RET_OK;
}

static int get_snc_index(unsigned long time){
  if(time == MAX_ULONG)
    return NOT_INIT;
  if(time < (unsigned) num_snc) /* make it compatible to the old stored a-b */
    return time;
  int i=num_snc-2;
  while((sncs[i].time_in_ms < time || sncs[--i].time_in_ms >= time) && i>1);
  return i+1;
}

static bool ab_file(bool load){ /* check: load=false */
  int fd = open_file(EXT_AB,O_RDONLY);
  if(fd<0){
    abfile_state=AB_NO;
    return false;
  }
  abfile_state=AB_YES;
  if(load){
    int len = rb->read_line(fd, buf, BUFFERSIZE);
    if(len > 0){
      start_snc=get_snc_index(rb->atoi(buf));
      rb->read_line(fd, buf, BUFFERSIZE); /* no test needed */
      stop_snc=get_snc_index(rb->atoi(buf));
    }
  }
  rb->close(fd);
  return true;
}

static void set_ab_marker(void)
{
  if(NO_LYRICS()) return;
  int snc=auto_scroll?current_snc:current_snc_edit;
  if(sncs[snc].type == TYPE_TR)
    snc--;
  if(snc==start_snc) clear_ab();
  else if(snc==stop_snc) stop_snc=NOT_INIT;
  else{
    if(start_snc==NOT_INIT) start_snc=snc;
    else if(stop_snc==NOT_INIT) stop_snc=snc;
    else if(snc<stop_snc-2) start_snc=snc;  /* set already initialized a to new position */
    else stop_snc=snc;   /* or b if new position is after old b or 2 before */

    if(stop_snc!=NOT_INIT && stop_snc<start_snc) /* swap a and b */
    {
      stop_snc=start_snc;
      start_snc=snc;
    }
    /* don't want to call audio_flush_and_reload_tracks() everytime a is set or cleared */
    /* just set repeat ab mode when a or b is set to the last snc */
    if(start_snc==num_snc-1 || stop_snc==num_snc-1) set_repeat_mode(RMODE_AB);
  }
}

static void ab_menu(void){
  char* ab_items[4];
  int i=0;
  int offset=0;
  if(start_snc!=NOT_INIT){
    ab_items[i++] = STR_(SNC_L_CLEAR);
    ab_items[i++] = STR_(SNC_L_SAVE);
  }
  else offset=2;

  if(abfile_state==AB_UNKNOWN)
    abfile_state = ab_file(false);
  if(abfile_state==AB_YES){
    ab_items[i++] = STR_(SNC_L_LOAD);
    ab_items[i++] = STR_(SNC_L_REMOVE);
  }
  if(i==0) return;

  i = do_list("A-B", Icon_Menu_functioncall, i, ab_items, NULL, 0, i) + offset;
  switch(i){
    case 0: clear_ab(); break;
    case 1: /* save */
    {
      int fd = open_file(EXT_AB, O_WRONLY|O_CREAT|O_TRUNC);
      if(fd<0){
        abfile_state=AB_NO;
        break;
      }
      rb->fdprintf(fd,"%lu\n%lu\n",
        sncs[start_snc].time_in_ms,
        stop_snc == NOT_INIT ? MAX_ULONG : sncs[stop_snc].time_in_ms);
      rb->close(fd);
      abfile_state=AB_YES;
      break;
    }
    case 2: /* load */
      ab_file(true);
      break;
    case 3: /* remove */
      rb->strcpy(buf,lyrics_filename);
      char* ext=rb->strrchr(buf,'.')+1;
      rb->strcpy(ext,FORMATS[EXT_AB]);
      rb->remove(buf);
      clear_ab();
      abfile_state=AB_NO;
      break;
  }
}

static void set_volume(int vol)
{
  if(vol < rb->sound_min(SOUND_VOLUME) || vol > rb->sound_max(SOUND_VOLUME))
    return;
  rb->sound_set(SOUND_VOLUME, vol);
  rb->global_settings->volume = vol;
  rb->snprintf(buf, BUFFERSIZE, "%d", vol);
  LCD_CLEAR_AREA(SCREEN_MAIN,0,0,LCD_WIDTH,ICON_HEIGHT);
  /* draw icon */
  draw_volume_icon();
  /* draw bar */
  #define VOL_BAR_POS 30  /* > 3x6 */
  draw_bar(VOL_BAR_POS,LCD_WIDTH-VOL_BAR_POS,3,
      VOL_BAR_POS + ((LCD_WIDTH-2*VOL_BAR_POS))*
      (vol - rb->sound_min(SOUND_VOLUME)) /
      (rb->sound_max(SOUND_VOLUME) - rb->sound_min(SOUND_VOLUME)));
  /* show value*/
  int width;
  rb->lcd_setfont(FONT_SYSFIXED);
  rb->lcd_getstringsize(buf,&width,NULL);
  rb->lcd_putsxy(LCD_WIDTH-width-2,0,buf);
  rb->lcd_setfont(FONT_UI);

  rb->lcd_update_rect(0,0,LCD_WIDTH,ICON_HEIGHT);
  exec_func(10,update_status_display);
}

static bool tt2int(unsigned char** buffer, int* number, char end_char){
  unsigned char* start=*buffer;
  bool ok;
  while((ok=**buffer!=0)==true && *(*buffer)++!=end_char);
  size_t size=*buffer-start-1;
  if(ok &= size<=3){
    char xx[size+1];
    xx[size]=0;
    rb->memcpy(xx,start,size);
    *number=rb->atoi(xx);
  }
  else *buffer=start;
  return ok;
}

static bool lrc_timetag(unsigned char** buffer,unsigned long* time)
{
  /* format: [m:ss.xxx],[mm:ss.xxx],[mm:ss.xx],[mm:ss],[mm:ss:xxx],[mm:ss:xx] */
  bool is_timetag=**buffer=='[' && isdigit((*buffer)[1]) &&
                  ((*buffer)[3]==':' || (*buffer)[2]==':');
  if(is_timetag){
    int mm,ss,ms=0;
    (*buffer)++; /* skip [ */
    tt2int(buffer,&mm,':'); /* mm: */
    tt2int(buffer,&ss,'.') ?  /* ss. ? */
      tt2int(buffer,&ms,']'): /* ms] */
      tt2int(buffer,&ss,':')? /* ss: ? */
      tt2int(buffer,&ms,']'):tt2int(buffer,&ss,']');
    /* +1 ensure that time is > 0 */
    *time=mm*60000+ss*1000+(ms<100 ? ms*10 : ms) +2; /* wrong if ms=0xx !!! */
  }
  return is_timetag;
}

static bool is_wrap_char_or_digit(const unsigned char* const c){
  return *c<'A' && *c!=0 && *c!=0x27;
}

static bool is_wrap_char(const unsigned char* const c){
  return is_wrap_char_or_digit(c) && !isdigit(*c);
}

static unsigned char* utf_char_width(
    const unsigned char* pos, int* size, int* width){
  if (*pos < 0x80) *size = 1;      /* U-00000000 - U-0000007F, 1 *size */
  else if (*pos < 0xe0) *size = 2; /* U-00000080 - U-000007FF, 2 *sizes */
  else if (*pos < 0xf0) *size = 3; /* U-00000800 - U-0000FFFF, 3 *sizes */
  else if (*pos < 0xf5) *size = 4; /* U-00010000 - U-001FFFFF, 4 *sizes */
  /* else: Invalid size. */

  unsigned char* end = (unsigned char*) pos + *size;
  if(width != NULL){
    /* get char width */
    unsigned char c=*end;
    *end=0;                                  /* terminate string */
    rb->lcd_getstringsize(pos, width, NULL); /* get width */
    *end=c;                                  /* restore string */
  }
  return end;
}

static bool store_line_(struct SNCText* snc,const unsigned char* buffer, const int max_width){
  bool ok;
  int w;                             /*  temp vars to hold for result passing */
  int byte=1;
  unsigned char c;
  unsigned char* endpos;
  unsigned char* npos = 0;
  const unsigned char* pos = buffer; /*  current position */
  const unsigned char* sp = buffer;  /*  position of last space */
  int width;                         /*  total width */
  int sp_width = 0;                  /*  width from start to space */
  snc->total_width = 0;

  do{
    width = 0;
    while(*pos != 0 && width<=max_width) {  /*  loop until end of string */
      if(*pos == 0xe3 && *(pos+1) == 0x80 && *(pos+2) == 0x80){
        /* replace 0xe3 0x80 0x80 with 0x20 */
        unsigned char* rep = (unsigned char*) pos;
        *rep = ' ';
        rb->memcpy(rep+1, rep+3, rb->strlen(rep+3)+1);
      }
      if(is_wrap_char(pos)){
        sp = pos;      /*  remember last space position */
        sp_width = width;
      }
      pos=utf_char_width(pos,&byte,&w);

      if(utf8==false && byte>1)
        utf8_tmp=true;
      width += w; /*  add to total */
    }
    if(width <= max_width){
      npos = (unsigned char*) pos;
      *npos = 0; /* ensure end */
    }
    else {/* width exceeds max_width */
      npos = (unsigned char*) sp+1; /* there was a space, we want to wrap after it */
      width -= sp_width; /* update width */
      if(sp==buffer){ /* no space */
        pos -= byte;     /* restore the last position, which was in range */
        npos = (unsigned char*) pos;
      }
      while (is_wrap_char(npos)) npos++; /* new line shouldn't begin with one of these chars */
      pos=sp=npos;  /* set beginning to the new position */
    }
    /* don't store empty lines (exception: 1st line) */
    if(npos>buffer || snc->rows==0){
      endpos = npos;
      while(*(endpos-1) == ' ') --endpos; /* trim trailing spaces */
      if(*npos == 0){
        npos = endpos;
        *npos = 0; /* trim spaces */
      }
      /* calculate width */
      c = *endpos;
      *endpos = 0;
      rb->lcd_getstringsize(snc->lyrics[snc->rows],&(snc->width[snc->rows]),NULL);
      *endpos = c;
      snc->total_width += snc->width[snc->rows];
      /* next */
      buffer = snc->lyrics[++(snc->rows)] = npos;
    }
  }while((ok=(snc->rows+1)<MAX_ROWS)==true && *npos != 0);
  if(!ok) rb->splash(HZ, STR_(SNC_L_LINE_TOO_LONG));
  /* look for end of string and set end mark AFTER \0 to prevent truncation of the line */
  while(*(snc->lyrics[snc->rows])++!=0);
  return ok;
}
static bool store_line(
  struct SNCSection *snc, const unsigned char* buffer, bool reset_rows)
{
  if(reset_rows){
    SET_ROWS(snc,0);
  }

  bool ok=store_line_(&(snc->x[SCROLL]),buffer,
                      bmp_width>0 && prefs.load_bmp == BMP_WEST?
                        LCD_WIDTH-bmp_width : LCD_WIDTH);
  ok &= store_line_(&(snc->x[EDIT]),buffer,LCD_WIDTH-indentwidth);
#ifdef HAVE_REMOTE_LCD
  ok &= store_line_(&(snc->x[RC]),buffer,LCD_REMOTE_WIDTH);
#endif
  return ok;
}

static void init_snc(struct SNCSection* snc, bool reset_time){
  if(snc==NULL) return;
  if(reset_time) snc->time_in_ms=0;
  snc->type = TYPE_STD;
  SET_ROWS(snc,1);
  unsigned char* lyrics=snc->x[SCROLL].lyrics[0];
  *lyrics=0;
  /* [0]0[1] */
  SET_LYRICS(snc,lyrics,0);
  SET_LYRICS(snc,lyrics+1,1);
}

/* snc: extract time from the buffer, if the buffer contains a timetag */
static bool snc_timetag(const char* buffer,unsigned long* time)
{
  /* format: \xA2\xE2hhmmss..\xA2\xD0 */
  bool is_timetag = rb->strncmp(buffer,"\xA2\xE2",2)==0;
  if(is_timetag)
  {
    char xx[3];
    xx[2]='\0';
    int mm,ss,ms;
    const char* pos=&buffer[4]; /* skip hh */

    xx[0]=*pos++;
    xx[1]=*pos++;
    mm=rb->atoi(xx);

    xx[0]=*pos++;
    xx[1]=*pos++;
    ss=rb->atoi(xx);

    xx[0]=*pos++;
    xx[1]=*pos++;
    ms=rb->atoi(xx);

    *time=mm*60000+ss*1000+ms*10+1; /* ms*10, +1 ensure that time is > 0 */
  }
  return is_timetag;
}

static int store_snc_section(struct SNCSection *snc, int fd)
{
  int len;
  unsigned char* lyrics;
  SET_ROWS(snc,0);
  bool is_time_tag;
  bool ok=true;
  while((len = rb->read_line(fd, buf, BUFFERSIZE))>0 &&
        (is_time_tag = snc_timetag(buf,&(snc->time_in_ms)))==false){
    lyrics=snc->x[SCROLL].lyrics[snc->x[SCROLL].rows];
    snc->x[EDIT].lyrics[snc->x[EDIT].rows]=lyrics;
#ifdef HAVE_REMOTE_LCD
    snc->x[RC].lyrics[snc->x[RC].rows]=lyrics;
#endif
    iso2utf8(buf, lyrics, len);
    if(ok) ok=store_line(snc,lyrics,false);
  }
  return len;
}

static bool read_snc(int fd)
{
  num_snc=0;
  if(!prefs.readsnc) return false;
  bool ok;
  do{
    ok=store_snc_section(&sncs[num_snc],fd)>0;
    struct SNCText* snc=&(sncs[num_snc].x[SCROLL]);
    if(snc->rows==0) init_snc(&sncs[num_snc],false);
    if(++num_snc < MAX_SECTIONS)
      sncs[num_snc].x[SCROLL].lyrics[0]=snc->lyrics[snc->rows];
    else ok=false;
  } while(ok);
  /* update time: time[i]=time[i-1] */
  int i=num_snc-1;
  for(;i>0;i--) sncs[i].time_in_ms=sncs[i-1].time_in_ms;
  sncs[0].time_in_ms=0;
  return true;
}

static bool read_id3(int fd){
  if(!prefs.readid3
      || id3->codectype>AFMT_MPA_L3
      || id3->codectype==AFMT_UNKNOWN
      || id3->length>600000){
    /* not mp1/2/3 or > 10 min */
    return false;
  }
  const int ID3_HEADER_SIZE=10;
  char header[ID3_HEADER_SIZE];
  rb->read(fd, header, 10);
  unsigned int pos=ID3_HEADER_SIZE;
  unsigned long framelen=0;
  unsigned char* buffer=0;
  int type=0;
  int header_size=6;
  enum {SYLT=1,USLT};
  while(pos<id3->id3v2len){
    rb->read(fd, header, ID3_HEADER_SIZE);
    framelen = bytes2int(header[4], header[5], header[6], header[7]);
    if(rb->memcmp(header,"SYLT",4)==0) /* SYLT found */
      type=SYLT;
    else if(rb->memcmp(header,"USLT",4)==0){
      type=USLT;
      header_size=4;
      /* check if there's a txt file */
      if(load_file(EXT_TXT)) return true;
      /* handle like a txt file */
      rb->strcpy(rb->strrchr(lyrics_filename,'.')+1,FORMATS[EXT_TXT]);
      g_ext=EXT_TXT;
      auto_scroll=false;
      current_snc_edit=1;
    }
    if(type){
      size_t size;
      buffer=(unsigned char*) rb->plugin_get_buffer(&size);
      rb->read(fd, buffer, framelen);
      break;
    }
    rb->lseek(fd, framelen, SEEK_CUR); /* skip content */
    pos+=HEADER_SIZE;
  }

  if(type){ /* found */
    unsigned char* bpos=buffer+header_size; /* skip header */
    bool unicode=buffer[0]==1;
    unsigned char* (*utf_decode)(const unsigned char *, unsigned char *, int);
    utf_decode=rb->utf16LEdecode;
    if(!unicode)
      while(*bpos++!=0);     /* skip content descriptor */
    else{
      if(rb->strncmp(bpos,"\xFE\xFF",2)==0) utf_decode=rb->utf16BEdecode;
      while(*bpos!=0 || *(bpos+1)!=0) bpos+=2;
      bpos+=2;
      while(*bpos==0xFF || *bpos==0xFE) bpos++; /* skip them (tagrename) */
    }

    init_snc(&sncs[0],true);
    num_snc=1; /* start with snc = 1 */
    SET_LYRICS((&sncs[num_snc]),sncs[0].x[SCROLL].lyrics[0]+1,0);
    unsigned char* endpos=buffer+framelen;
    /* ensure termination (+1: unicode) */
    *(endpos)=0; *(endpos+1)=0;
    while(endpos>bpos){
      struct SNCText* snc=&(sncs[num_snc].x[SCROLL]);
      unsigned char* lyrics=snc->lyrics[0];
      if(type==USLT){
      /* replace USLT endchar 0x0A with 0x00 and 0x0D with a space */
        lyrics=bpos;
        while((*lyrics)!=0x0A){
          if(*lyrics==0x0D) *lyrics=' ';
          lyrics++;
        }
        *lyrics=0; /* now it's like SYLT and independent of endianess */
        lyrics=snc->lyrics[0];
      }
      if(!unicode)
        while((*lyrics++=*bpos++)!=0);
      else{
        while(*bpos==0xFF || *bpos==0xFE) bpos++; /* skip FF and FE */
        unsigned char* start=bpos;
        while(*bpos!=0 || *(bpos+1)!=0) bpos+=2; /* not 00 */
        bpos+=2; /* include 00 */
        utf_decode(start,lyrics,bpos-start);
      }

      SET_LYRICS((&sncs[num_snc]),snc->lyrics[0],0);
      store_line(&sncs[num_snc],snc->lyrics[0],true);
      if(type==SYLT){
        /* timestamp */
        sncs[num_snc].time_in_ms=bytes2int(bpos[0],bpos[1],bpos[2],bpos[3]);
        bpos+=4;
      }
      else sncs[num_snc].time_in_ms=NOT_INIT;
      sncs[++num_snc].x[SCROLL].lyrics[0]=snc->lyrics[snc->rows];
    }
  }
  return type;
}

int lrc_readline(int fd, char* buffer, int buffer_size){
  int count = 0;
  int num_read = 0;
  unsigned char c;
  bool open_bracket_found = false;
  while (count < buffer_size)
  {
    if (1 != rb->read(fd, &c, 1))
      break;
    num_read++;
    if ( c == '\n' )
      break;
    if ( c == '\r' )
      continue;
    /* look for lrc timetag without newlines */
    if(c == '[' && num_read > 1 && buffer[count-1] != ']'){
      open_bracket_found = true;
    }
    else if(open_bracket_found && isdigit(c)){
      /* lrc timetag without newline detected */
      count--;
      rb->lseek(fd,-2,SEEK_CUR);
      break;
    }
    buffer[count++] = c;
  }
  buffer[MIN(count, buffer_size - 1)] = 0;
  return num_read;
}

static bool read_lrc(int fd, bool store_notag, bool append){
  int append_start=0;
  int type = append ? TYPE_TR:TYPE_STD;
  if(append==false){
    init_snc(&sncs[0],true);
    num_snc=1; /* start with snc = 1 */
    SET_LYRICS((&sncs[num_snc]),sncs[0].x[SCROLL].lyrics[0]+1,0);
  }
  else{ /* append */
    if(NO_LYRICS()) return false; /* not possible */
    SET_LYRICS((&sncs[num_snc]),&lyrics_buffer[lyrics_buffer_used],0);
    append_start = num_snc;
  }

  int snc_id,src_id,len,old_len=100;
  unsigned char* buffer;
  while ((len = lrc_readline(fd, buf, BUFFERSIZE))>0 && num_snc<MAX_SECTIONS){
    buffer=buf;
    snc_id=num_snc;
    src_id=num_snc;
    if(lrc_timetag(&buffer,&(sncs[snc_id++].time_in_ms))){
      while(lrc_timetag(&buffer,&(sncs[snc_id].time_in_ms)))
        snc_id++; /* look for more timetags */
      while(*buffer==' ') buffer++; /* skip space(s) */
      if(append && *buffer==0) continue; /* skip blank lines */
    }
    else if(store_notag){ /* plain text */
      if(old_len<3 && len<3) continue; /* 2 consecutive empty lines */
      sncs[num_snc].time_in_ms=NOT_INIT;
    }
    else if(rb->strncmp(buffer,"[la:",4)==0){ /* language tag */
      if(buffer[4] != ']'){
        language[0]=buffer[4];
        language[1]=buffer[5];
        language[2]=0;
      }
      continue;
    }
    else continue; /* not handled lrc tag */

    struct SNCText* snc=&(sncs[num_snc].x[SCROLL]);
    unsigned char* lyrics=snc->lyrics[0];
    iso2utf8(buffer, lyrics, len-(buffer-buf)+1);
    SET_LYRICS((&sncs[num_snc]),lyrics,0);
    store_line(&sncs[num_snc],lyrics,true);
    while(++num_snc!=snc_id){
      COPY_SNC((&sncs[num_snc]),(&sncs[src_id]));
    }
    /* next */
    sncs[num_snc].x[SCROLL].lyrics[0]=snc->lyrics[snc->rows];
    old_len=len;
  }
  lyrics_buffer_used=sncs[num_snc].x[SCROLL].lyrics[0]-lyrics_buffer;
  if(store_notag==false){ /* don't sort if it's txt */
    int i;
    for(i=append_start;i<num_snc;i++) sncs[i].type = type;

    int compare(const void* p1, const void* p2){
      struct SNCSection* s1 = (struct SNCSection*) p1;
      struct SNCSection* s2 = (struct SNCSection*) p2;
      return (s1->time_in_ms + s1->type) -
             (s2->time_in_ms + s2->type);
    }
    rb->qsort(sncs,num_snc,sizeof(struct SNCSection),compare);
  }
  return true;
}

static bool read_cue(int fd){
  init_snc(&sncs[0],true);
  num_snc=0;
  SET_LYRICS((&sncs[num_snc]),sncs[0].x[SCROLL].lyrics[0]+1,0);
  unsigned char *s;
  struct SNCText* snc=NULL;
  /* code copied from cuesheet.c (modified) */
  while (rb->read_line(fd, buf, BUFFERSIZE)>0 && num_snc<MAX_SECTIONS){
      snc=&(sncs[num_snc].x[SCROLL]);
      unsigned char* lyrics=snc->lyrics[0];

      s = buf;
      while(*s==' ' || *s=='\t') s++; /* skip space(s) */
      if (!rb->strncmp(s, "TRACK", 5))
      {
          sncs[++num_snc].x[SCROLL].lyrics[0]=snc->lyrics[snc->rows];
      }
      else if (!rb->strncmp(s, "INDEX 01", 8))
      {
          s+=8;
          while(*s==' ' || *s=='\t') s++; /* skip space(s) */
          sncs[num_snc].time_in_ms = 60000 * rb->atoi(s);
          s = rb->strchr(s,':') + 1;
          sncs[num_snc].time_in_ms += 1000 * rb->atoi(s);
          s = rb->strchr(s,':') + 1;
          sncs[num_snc].time_in_ms += 13 * rb->atoi(s);
          if(sncs[num_snc].time_in_ms==0) sncs[num_snc].time_in_ms=1;
      }
      else if (!rb->strncmp(s, "TITLE", 5))
      {
          char *string, *end;
          string = rb->strchr(s, '"');
          if (!string)
          {
              string = rb->strchr(s, ' ');
              if (!string) break;
          }
          end = rb->strchr(++string, '"');
          if (end) *end = '\0';

          /* iso2utf8(string, lyrics, rb->strlen(string)+1); */
          rb->memcpy(lyrics,string,rb->strlen(string)+1);
          SET_LYRICS((&sncs[num_snc]),lyrics,0);
          store_line(&sncs[num_snc],lyrics,true);
      }
  }
  sncs[0].x[SCROLL].lyrics[0][0]=0;
  SET_ROWS((&sncs[0]),1);
  if(snc!=NULL) sncs[++num_snc].x[SCROLL].lyrics[0]=snc->lyrics[snc->rows];
  return true;
}

static void finalize(void){
  /* add_last_section */
  if(num_snc+1 >= MAX_SECTIONS
  || sncs[num_snc-1].x[SCROLL].lyrics[0] == (unsigned char*) END_MARK)
    return;

  int w;
  rb->lcd_getstringsize(END_MARK, &w, NULL);
  struct SNCSection* last=&sncs[num_snc];
  SET_ROWS(last,1);
  SET_WIDTH(last, w);
  last->time_in_ms=id3->length+UPDATE_LATENCY;
  last->type = TYPE_STD;
  SET_LYRICS(last,END_MARK,0);
  num_snc++;
  /* set snc type to TYPE_STD */
  if(prefs.load_translation!=TR_OK){
    for(w=1; w<num_snc; w++)
      sncs[w].type = TYPE_STD;
  }
}

static bool load_file(enum e_supported_formats type){
  bool ret=true;
  int fd = open_file(type, O_RDONLY);
  if (fd < 0){
    if(type==EXT_TR) prefs.load_translation=TR_FAILED;
    return type==EXT_LRC ? load_file(EXT_LRC8):false; /* try to open lrc8 */
  }
  if(type!=EXT_TR){
    g_ext=type;
    lyrics_buffer_used=0;
    SET_LYRICS((&sncs[0]),lyrics_buffer,0);
  }
  switch(type){
    case EXT_LRC:
    case EXT_LRC8:
      read_lrc(fd, false, false);
      break;
    case EXT_TR:
    {
      prefs.load_translation=TR_OK;
      if(prefs.tr_use_sysfont)
        rb->lcd_setfont(FONT_SYSFIXED);
      read_lrc(fd, false, true);
      rb->lcd_setfont(FONT_UI);
      break;
    }
    case EXT_SNC: read_snc(fd); break;
    case EXT_MP3: ret=read_id3(fd); break;
    case EXT_CUE: read_cue(fd); break;
    default: /* read txt */
      bmp_width=NOT_INIT;
      bmp_height=NOT_INIT;
      read_lrc(fd,true,false);
      if(sncs[1].time_in_ms==MAX_ULONG) sncs[1].time_in_ms=1;
      current_snc_edit=1;
  }
  rb->close(fd);

  /* if prefs.load_translation is set, try to load translation file */
  if(prefs.load_translation &&
     type!=EXT_TR &&    /* prevent loop */
     load_file(EXT_TR)){ /* try to load translation file */
      return true;   /* continue if loading translation fails,
                      return otherwise (finishing is already done by EXT_TR)*/
  }

  /* lrc: after sorting, the last snc could be somewhere in the lyrics buffer */
  if(lyrics_buffer_used==0)
    lyrics_buffer_used=sncs[num_snc].x[SCROLL].lyrics[0]-lyrics_buffer;

  finalize();

  if(utf8_tmp && utf8==false) utf8=true;
  return ret;
}

static void conditional_wait(bool (*condition_cb)(void*), void* data){
  long counter = 0; /* counter to prevent endless loop */
  while(condition_cb(data) && ++counter < 10000){
    rb->yield();
  }
}

static bool in_range(int diff, int tolerance) {
  return diff >= tolerance || diff <= -tolerance;
}

static bool need_ff_rew(void* time_in_ms){
  return in_range(AUDIO_ELAPSED() - *((long*)time_in_ms), FF_REW_TOLERANCE);
}

static void ff_rew(unsigned long time_in_ms){
  if(!need_ff_rew((void*) &time_in_ms)) return;

  bool playing=!(rb->audio_status()&AUDIO_STATUS_PAUSE);
  if(playing) rb->audio_pause();
  rb->audio_ff_rewind(time_in_ms);
  conditional_wait(need_ff_rew, (void*) &time_in_ms);
  if(playing)
    rb->audio_resume();
  update_display();
}

static void ff_rew_snc(int snc){
  snc%=(num_snc-1); /* 0 .. num_snc-2 */
  if(sncs[snc].time_in_ms==MAX_ULONG) return; /* txt */
  ff_rew(sncs[snc].time_in_ms);
}

static void switch_mode(bool discard)
{
  if(num_snc<NOT_INIT) return;
  if(num_snc!=0)
    auto_scroll=!auto_scroll;
  else
  {
    if(!load_file(EXT_TXT)){
      num_snc=-2;
      return;
    }
    /* txt file loaded */
    auto_scroll = false;
  }

  if(!auto_scroll){
    current_snc_edit=current_snc;
  }
  else if(!discard) ff_rew_snc(current_snc_edit);
  update_display();
}

static int auto_cue(void){
  int num_cues;
  if(!prefs.cue_interval ||
     (num_cues = snc_div(id3->length, prefs.cue_interval)) >= MAX_SECTIONS || num_cues < 2)
  {
    if(IS_AUTOCUE()){
      auto_scroll = true;
      end_snc_edit=MAX_SECTIONS;
      num_snc=0;
    }
    return RET_NOK;
  }
  if(NO_LYRICS()) g_ext=EXT_CUE; /* no sncs => set extension to cue */
  else if(!NO_LYRICS() && !IS_AUTOCUE())
    return RET_NOK;

  lyrics_buffer_used=0;
  SET_LYRICS((&sncs[0]),lyrics_buffer,0);
  init_snc(&sncs[0],true);
  sncs[0].time_in_ms=AUTOCUE_ID; /* hack: to identify auto_cue */
  SET_LYRICS((&sncs[1]),sncs[0].x[SCROLL].lyrics[0]+1,0);
  struct SNCText* snc=NULL;
  int width;
  int i=1;
  while(i<=num_cues){
    snc=&(sncs[i].x[SCROLL]);
    sncs[i].time_in_ms=(i-1)*prefs.cue_interval+2; /* +2: ensure it's > sncs[0].time_in_ms */
    int bytes=rb->snprintf(snc->lyrics[0],11,"%02d Track",i)+1; /* XX Track */
    SET_ROWS((&sncs[i]),1);
    SET_LYRICS((&sncs[i]),snc->lyrics[0],0);
    SET_LYRICS((&sncs[i]),snc->lyrics[0]+bytes,1);
    rb->lcd_getstringsize(snc->lyrics[0], &width, NULL);
    SET_WIDTH((&sncs[i]), width);
    sncs[++i].x[SCROLL].lyrics[0]=snc->lyrics[snc->rows];
  }
  num_snc=num_cues + 1;
  lyrics_buffer_used=sncs[num_snc].x[SCROLL].lyrics[0]-lyrics_buffer;
  finalize();
  current_snc = 0;
  end_snc_edit=MAX_SECTIONS;
  return RET_OK;
}

static bool update_id3_cb(void* data){
  (void) data;
  id3 = rb->audio_current_track();
  return !(IS_ID3_VALID(id3)) || id3->length <= 0; /* wait 'til the id3 is valid */
}
static void reset(bool all)
{
  if(all){
    conditional_wait(update_id3_cb, NULL);
    store_artist_title(&g_art, id3->artist);
    store_artist_title(&g_tit,
      id3->title != NULL ? id3->title : rb->strrchr(id3->path,'/')+1);
    store_artist_title(&g_next_at, 0);

    num_snc=0;
    clear_ab();
    time_offset=0;
    utf8=false;
    modified=0;
    utf8_tmp = false;
    abfile_state=AB_UNKNOWN;
    /* update indentwidth */
    if(prefs.time_visible){
      cat_time(buf,id3->length);
      rb->lcd_getstringsize(buf, &indentwidth, NULL);
    }
    else
      indentwidth = 0;
    indentwidth += rb->font_get(FONT_UI)->maxwidth;
  }
  v_scroll_artist_title(true);
  language[0] = 0;
  current_snc=0;
  current_snc_edit=0;
  end_snc_edit=MAX_SECTIONS;
  bmp_width=bmp_height=NOT_INIT;

  if(file_list.auto_insert) add2file_list(false);
  if(auto_scroll)
    base_display();
  update_status_busy(ACTION_REDRAW);

  if(prefs.load_bmp){
    num_snc=MAX_SECTIONS; /* hack: to ensure that we are in prefs.load_bmp mode */
    load_bmp(2);
    num_snc=0;
    /* if BMP_CENTER && AA available then show bitmap without lyrics */
    if(prefs.load_bmp == BMP_CENTER){
      load_bmp(1);
      update_display();
      g_karaoke = false;
      return;
    }
  }
#ifdef HAVE_ADJUSTABLE_CPU_FREQ
  rb->cpu_boost(true);
#endif
  if(!(load_file(EXT_LRC) ||
       load_file(EXT_MP3) ||
       load_file(EXT_SNC) ||
       load_file(EXT_CUE))){ /* all supported formats failed */
    num_snc=0;
    sncs[0].time_in_ms=0; /* to enable ff_rew */
    SET_ROWS((&sncs[0]),0);
    if(prefs.load_translation) prefs.load_translation=TR_FAILED;
    auto_cue();
  }

  if(NO_LYRICS() && !auto_scroll){
    auto_scroll = true;
    base_display();
  }
  update_time_display(AUDIO_ELAPSED(),false);
  if(auto_scroll){
    scroll_snc();
  }
  if(prefs.load_bmp || NO_LYRICS()){
    load_bmp(1);
    if(auto_scroll)
      NO_LYRICS() ? rb->lcd_update() : scroll_snc();
  }
  if(!auto_scroll)
    browse_snc();
#ifdef HAVE_ADJUSTABLE_CPU_FREQ
  rb->cpu_boost(false);
#endif
  g_karaoke = prefs.karaoke && !NO_LYRICS();
  update_display();
}

static void save2cue(void){
  int fd = rb->open(lyrics_filename, O_WRONLY|O_CREAT|O_TRUNC);
  if(fd<0) return;

  rb->fdprintf(fd,"PERFORMER \"%s\"" NL "TITLE \"%s\"" NL "FILE \"%s\" MP3" NL,
    g_art.lyrics[0],g_tit.lyrics[0],id3->path);
  int i = 1,last = num_snc-1;

  for(;i<last;i++){
    rb->fdprintf(fd,
      "TRACK %02d AUDIO" NL " TITLE \"%s\"" NL " INDEX 01 %02d:%02d:%02d" NL,
      i, sncs[i].x[SCROLL].lyrics[0],
      (int) CALC_MM(sncs[i].time_in_ms),
      (int) CALC_SS(sncs[i].time_in_ms),
      (int) CALC_MS(sncs[i].time_in_ms));
  }
  rb->close(fd);
}

static void save2file(void)
{
  if(!modified) return;
  modified=0;
  time_offset=0;
  rb->splash(HZ,STR_(SNC_L_SAVING));

  int r,i;
  int last=num_snc-1;
  bool save_tr=false;
  char filename_buf[MAX_PATH];
  char* filename=lyrics_filename;
  if(prefs.load_translation==TR_OK){
    rb->strcpy(filename_buf,lyrics_filename);
    filename=filename_buf;
  }
  char* ext=rb->strrchr(filename,'.')+1;
  if(g_ext==EXT_CUE){
    rb->strcpy(ext,FORMATS[EXT_CUE]);  /* ensure the extension is cue */
    save2cue();
    return;
  }
  bool is_txt=g_ext==EXT_TXT;
  if( is_txt ||(g_ext==EXT_LRC && utf8)) /* lrc: -> rename to lrc8 */
  {
    bool complete=false;
    i=1;
    while(i<last && (complete=sncs[i++].time_in_ms!=MAX_ULONG));
    if(complete){
      rb->remove(lyrics_filename);    /* remove old file */
      rb->strcpy(ext,FORMATS[utf8 ? EXT_LRC8 : EXT_LRC]);   /* rename file to lrc/8 */
      g_ext=EXT_LRC;
    }
  }
  else if(g_ext==EXT_MP3 || g_ext==EXT_SNC){ /* SYLT or SNC */
    g_ext=utf8 ? EXT_LRC8 : EXT_LRC;
    rb->strcpy(ext,FORMATS[g_ext]);
  }
  int fd;
  SAVE_TRANSLATION:
  fd = is_txt && utf8 ?
    rb->open_utf8(filename, O_WRONLY|O_CREAT|O_TRUNC):
    rb->open(filename, O_WRONLY|O_CREAT|O_TRUNC);
  if(fd<0) return;

  bool is_lrc=g_ext==EXT_LRC || g_ext==EXT_LRC8 || g_ext==EXT_TR;
  if(is_lrc){
    if(language[0]!=0){
      rb->fdprintf(fd,"[la:%s]" NL,language);
    }
    rb->fdprintf(fd,"[ti:%s]" NL "[ar:%s]" NL, g_tit.lyrics[0],g_art.lyrics[0]);
  }
  bool save[last];
  rb->memset(save,true,last);
  int pos,j=last;
  for(i=1;i<last;i++){
    save[i] &= save_tr ? sncs[i].type == TYPE_TR : sncs[i].type == TYPE_STD;
    if(!save[i]) continue;
    pos=i;
    do{ /* save timetag */
      if(sncs[pos].time_in_ms!=MAX_ULONG){
        rb->fdprintf(fd,"[%02d:%02d.%02d]",
          CALC_MM(sncs[pos].time_in_ms),
          CALC_SS(sncs[pos].time_in_ms),
          CALC_MS(sncs[pos].time_in_ms));
      }
      if(is_lrc){ /* [timetag][timetag]..[timetag]lyrics */
        j=pos+1;
        for(;j<last;j++){
          if(save[j] &&
            lrccmp(sncs[i].x[SCROLL].lyrics[0],sncs[j].x[SCROLL].lyrics[0])==0){ /* save timetag */
            save[j]=false;
            pos=j;
            j=last; /* break */
          }
        }
      }
    }while(j!=last);

    /* lyrics are encoded in utf8! */
    struct SNCText* snc=&(sncs[i].x[SCROLL]);
    rb->fdprintf(fd,"%s",snc->lyrics[0]);

    for(r=1; r<snc->rows;r++){
      unsigned char* pos=snc->lyrics[r];
      if(*(--pos)==0){
        if(*(--pos)!=' ') rb->fdprintf(fd," ");
        rb->fdprintf(fd,"%s",snc->lyrics[r]);
      }
    }
    rb->fdprintf(fd,NL);
  }
  rb->close(fd);

  if(prefs.load_translation==TR_OK && save_tr == false){
    rb->strcpy(ext,FORMATS[EXT_TR]);
    save_tr=true;
    goto SAVE_TRANSLATION;
  }
}

static bool yes_no(char* const what){
  const char* items[] = {STR_(SNC_L_YES), STR_(SNC_L_NO)};
  return do_list(what, Icon_Questionmark, 2, items, NULL, 0, 2) == 0;
}

static void confirm_saving(void)
{
  if(modified==0) return;
  if(yes_no(STR_(SNC_L_SAVE_QUESTION))) save2file();
  else modified=0;
}

static int add2file_list(bool remove){
  if(file_list.num >= MAX_FILES2DELETE){
    rb->pcmbuf_beep(1000, 500, 3000);
    rb->splash(HZ, STR_(SNC_L_DEL_BUFFER_FULL));
    file_list_menu();
    if(file_list.num >= MAX_FILES2DELETE)
      return ACTION_REDRAW;
  }
  int i=file_list.num-1;
  while(i>=0){
    if(rb->strcmp(file_list.files[i],id3->path) == 0){ /* is the file already in the list? */
      if(remove){
        rb->strcpy(file_list.files[i],
           file_list.files[--file_list.num]);
      }
      else
        rb->pcmbuf_beep(1000, 500, 3000);
      return ACTION_REDRAW;
    }
    i--;
  }
  rb->strcpy(file_list.files[file_list.num++],id3->path); /* add */
  return ACTION_STD_CANCEL;
}

static int file_list_save(void){
  if(file_list.num <= 0) return ACTION_REDRAW;
  rb->snprintf(buf, BUFFERSIZE, "%s/1", rb->global_settings->playlist_catalog_dir);
  if(!rb->kbd_input(buf,BUFFERSIZE)){
    rb->strcat(buf,".m3u8");
    int fd = rb->open(buf, O_WRONLY|O_CREAT|O_APPEND);
    if(fd>=0){
      int i = 0;
      const int nl_len = rb->strlen(NL);
      while(i<file_list.num){
        rb->write(fd,file_list.files[i], rb->strlen(file_list.files[i]));
        rb->write(fd,NL,nl_len);
        i++;
      }
      rb->close(fd);
    }
  }
  return ACTION_STD_CANCEL;
}

static int file_list_menu(void){
  #define NUM_F2D_MENUITEMS 5
  char* items[file_list.num + NUM_F2D_MENUITEMS];
  enum{FLIST_DEL=0,FLIST_SAVE,FLIST_AUTOINS,FLIST_ADD,FLIST_CLEAR};

  items[FLIST_DEL] = STR_(SNC_L_DELETE_FILES);
  items[FLIST_SAVE] = STR_(SNC_L_SAVE_AS);
  items[FLIST_ADD] = STR_(SNC_L_ADD);
  items[FLIST_AUTOINS] = STR_(SNC_L_AUTO_INSERT);
  items[FLIST_CLEAR] = STR_(SNC_L_CLEAR_LIST);
  int i = 0;
  while(i<file_list.num){
    items[i+NUM_F2D_MENUITEMS] = rb->strrchr(file_list.files[i],'/') + 1;
    i++;
  }

  int list_callback(int action, struct gui_synclist *lists){
    if(action != ACTION_STD_OK) return action;
    int i = rb->gui_synclist_get_sel_pos(lists);
    switch(i){
      case FLIST_DEL: /* delete all the files in the list */
        if(file_list.num == 0) return ACTION_REDRAW;
        rb->splash(HZ, STR_(SNC_L_DELETING));
        while(file_list.num>0)
          rb->remove(file_list.files[--file_list.num]);
        break;
      case FLIST_SAVE:
        return file_list_save();
      case FLIST_AUTOINS:
        file_list.auto_insert = !file_list.auto_insert;
      case FLIST_ADD: /* add current playing file to the list */
        return add2file_list(false);
      case FLIST_CLEAR: /* clear the list */
        if(file_list.num == 0) return ACTION_REDRAW;
        file_list.num = 0;
        break;
      default: /* remove file from list */
        rb->strcpy(file_list.files[i-NUM_F2D_MENUITEMS],
                   file_list.files[--file_list.num]);
        rb->gui_synclist_set_nb_items(lists,file_list.num+NUM_F2D_MENUITEMS);
        if(file_list.num > 0) return ACTION_REDRAW;
    }
    return ACTION_STD_CANCEL;
  }
  return do_list(STR_(SNC_L_FILE_LIST), Icon_Menu_functioncall,
    file_list.num+NUM_F2D_MENUITEMS, items, list_callback, 0, NUM_F2D_MENUITEMS)
      >= 0 ? RET_OK : RET_NOK;
}

static void print_hl_line(
  struct SNCText* snc,
  unsigned long elapsed,
  long len,
  int y0,
  int delta_y
)
{
  int w_hl = prefs.fontheight + snc->total_width *
    (elapsed - sncs[current_snc].time_in_ms)/len;
  bool right = bmp_width != NOT_INIT && prefs.load_bmp == BMP_WEST;
  int x, y = y0;
  int r = 0;
#if LCD_DEPTH > 1
  rb->lcd_set_foreground(prefs.hl_color);
#endif
  while( w_hl > 0 && r < snc->rows){
    x = right ? bmp_width : (LCD_WIDTH-snc->width[r])>>1;
    if(w_hl > snc->width[r]) /* highlight the whole row */
      w_hl -= snc->width[r];
    else{
      /* only highlight part of the row */
      rb->lcd_fillrect(x, y, w_hl, delta_y);
      rb->lcd_set_drawmode(DRMODE_BG);
      w_hl = 0; /* break */
    }
    print_text_line(snc,r,rb->lcd_putsxy,x,y, 0);
    r++;
    y += delta_y;
  }
  rb->lcd_update_rect(0, y0, LCD_WIDTH, y-y0);
  /* restore */
#if LCD_DEPTH > 1
  rb->lcd_set_foreground(prefs.fg_color);
#endif
  rb->lcd_set_drawmode(DRMODE_SOLID);
}

static void highlight_snc(unsigned long elapsed){
  if(!g_karaoke ||
     IS_BLANK(current_snc) ||
     current_snc + 1 >= num_snc)
    return;
  struct SNCText* snc = &(sncs[current_snc].x[SCROLL]);
  int next_snc = current_snc+1;
  bool is_translation = sncs[current_snc].type != sncs[next_snc].type;
  long len = sncs[is_translation ? (next_snc + 1):next_snc].time_in_ms
            - sncs[current_snc].time_in_ms;
  if(elapsed < sncs[current_snc].time_in_ms || len < 1)
    return;
  print_hl_line(snc, elapsed, len, g_current_y0, prefs.fontheight);
  if(is_translation){
    if(prefs.tr_use_sysfont)
      rb->lcd_setfont(FONT_SYSFIXED);
    print_hl_line(&(sncs[next_snc].x[SCROLL]), elapsed, len,
      g_current_y0 + (prefs.fontheight * snc->rows),
      prefs.tr_use_sysfont ? sysfont_height : prefs.fontheight);
    rb->lcd_setfont(FONT_UI);
  }
}

static void update(void)
{
  static unsigned long last_elapsed = 0; /* used to avoid flicker */
  if(id3!=rb->audio_current_track()) /* new track is playing */
  {
    last_elapsed = 0;
    if(rb->global_settings->repeat_mode+1!=RMODE_ONE &&
       rb->global_settings->repeat_mode+1!=RMODE_AB) /* no repeat */
    {
      confirm_saving();
      reset(true);
    }
    else
      id3 = rb->audio_current_track();
  }

  exec_func(0,NULL); /* update func queue */
  if(NO_LYRICS()) return;

  unsigned long elapsed = AUDIO_ELAPSED()+UPDATE_LATENCY;
  int show_snc=current_snc;
  int next_snc=current_snc;
  do{next_snc=(next_snc+1)%num_snc;}
  while(sncs[next_snc].type == TYPE_TR);

  if(auto_scroll){
    /* display countdown */
    struct SNCSection* snc=&sncs[current_snc];
    if(IS_BLANK(current_snc) &&
        (sncs[next_snc].time_in_ms >= snc->time_in_ms + BREAK_LENGTH)){
      int remain=1+(sncs[next_snc].time_in_ms-elapsed)/1000; /* +1: 0.9 -> 0 */
      if(remain<6){
        rb->memset(buf,COUNT_DOWN_MARK,remain);
        buf[remain]=0;
        if(bmp_width == NOT_INIT || prefs.load_bmp == BMP_SOUTH){
          LCD_CLEAR_AREA(SCREEN_MAIN,0,scroll_y0,LCD_WIDTH,prefs.fontheight);
          int w;
          rb->lcd_getstringsize(buf, &w, NULL);
          rb->lcd_putsxy((LCD_WIDTH-w)>>1,scroll_y0,buf);
        }
        else{
          LCD_CLEAR_AREA(SCREEN_MAIN,bmp_width,scroll_y0,LCD_WIDTH,prefs.fontheight);
          rb->lcd_putsxy(bmp_width,scroll_y0,buf);
        }
        rb->lcd_update_rect(0,scroll_y0,LCD_WIDTH,prefs.fontheight);
      }
    }
    else
      highlight_snc(elapsed);
  }

  int loop_cnt = -1;
  /* update snc */
  while(sncs[next_snc].time_in_ms <= elapsed ||
        sncs[current_snc].time_in_ms > elapsed){ /* -> ; <- */
    if(++loop_cnt > num_snc)
    {
      current_snc = show_snc; /* something is wrong! => don't update! */
      break;
    }
    current_snc = next_snc;
    do{next_snc=(next_snc+1)%num_snc;}
    while(sncs[next_snc].type == TYPE_TR); /* update next snc */
  }

  if(show_snc!=current_snc && in_range(elapsed-last_elapsed, UPDATE_LATENCY)){
    last_elapsed = elapsed;
    auto_scroll ? scroll_snc(): browse_snc(); /* update current_snc mark */
  }
  if(auto_scroll && (
    (start_snc!=NOT_INIT &&
    elapsed + FF_REW_TOLERANCE < sncs[start_snc].time_in_ms) ||
    (stop_snc!=NOT_INIT && current_snc>=stop_snc)))
      ff_rew_snc(start_snc);
}

static int insert_sections(void){
  if(start_snc+1==num_snc || current_snc_edit+1==num_snc ||
    (start_snc<current_snc_edit && stop_snc > current_snc_edit))
    return RET_NOK;
  if(start_snc==NOT_INIT) start_snc=0; /* insert blank */
  int offset=sncs[current_snc_edit].time_in_ms-sncs[start_snc].time_in_ms + 1;
  int nr2Copy=(stop_snc==NOT_INIT)?1:stop_snc-start_snc;
  num_snc+=nr2Copy;
  if(num_snc>=MAX_SECTIONS){
    rb->splash(HZ,STR_(SNC_L_TOO_MANY_LINES));
    num_snc=MAX_SECTIONS-1;
  }
  int i=num_snc;
  for(;i>current_snc_edit+nr2Copy;i--) /* move */
    snc_copy(&sncs[i],&sncs[i-nr2Copy],0);
  if(start_snc>current_snc_edit) start_snc+=nr2Copy;

  for(i=0;i<nr2Copy;i++)/* copy */
    snc_copy(&sncs[current_snc_edit+1+i],&sncs[start_snc+i],offset); /* paste and add with an offset */

  clear_ab();
  modified|=MODIFIED_LYRICS;
  end_snc_edit=MAX_SECTIONS;
  if(nr2Copy==1) current_snc_edit++; /* move to the currently inserted line */
  return RET_OK;
}

static int delete_sections(void){
  if(current_snc_edit==0 || start_snc+1==num_snc ||
    (current_snc_edit+1==num_snc && stop_snc==NOT_INIT)) return RET_NOK;
  if(start_snc==NOT_INIT) start_snc=current_snc_edit; /* delete current */

  int stop=stop_snc==NOT_INIT?start_snc+1:stop_snc;
  int nr2Copy=num_snc-stop;
  int i=0;
  for(;i<nr2Copy;i++) snc_copy(&sncs[start_snc+i],&sncs[stop+i],0);
  num_snc-=stop-start_snc;

  current_snc_edit=start_snc;
  clear_ab();
  modified|=MODIFIED_LYRICS;
  if(current_snc_edit>num_snc-1) current_snc_edit=num_snc-1;
  end_snc_edit=MAX_SECTIONS;
  current_snc = 0; /* to ensure current_snc is always in range */
  return RET_OK;
}

static int join_sections(void){
  if(current_snc_edit>=num_snc-2 || current_snc_edit==0 || prefs.load_translation==TR_OK)
    return false;
  /* need to save reference pointer because current_snc_edit is not fixed */
  unsigned char* join_ref1=sncs[current_snc_edit].x[SCROLL].lyrics[0];
  unsigned char* join_ref2=sncs[current_snc_edit+1].x[SCROLL].lyrics[0];
  unsigned char* lyrics;
  unsigned long time=sncs[current_snc_edit].time_in_ms; /* save time for restoring current_snc_edit */
  int i;
  for(i=num_snc-3;i>0;i--){ /* look for other lines with the same constellation */
    if(lrccmp(join_ref1,sncs[i].x[SCROLL].lyrics[0])==0
    && lrccmp(join_ref2,sncs[i+1].x[SCROLL].lyrics[0])==0){

      lyrics=sncs[i+1].x[SCROLL].lyrics[0];
      if(*(lyrics-1)==0){
        rb->strcpy(buf,sncs[i+1].x[SCROLL].lyrics[0]);
        *(lyrics-1)=' '; /* replace \0 with ' ' */
        while(*(lyrics-2)==' ') --lyrics; /* remove spaces */
        rb->strcpy(lyrics,buf);
      }
      store_line(&sncs[i],sncs[i].x[SCROLL].lyrics[0],true);  /* reformat */
      clear_ab();         /* make sure there ab is not set */
      current_snc_edit=i; /* make sure current_snc_edit fulfills the deletion criteria */
      start_snc=i+1;      /* point to merged sync */
      delete_sections();  /* and get rid of it (does other housekeeping as well) */
    }
  }
  for(i=1;i<num_snc-1;i++){ /* restore current_snc_edit */
    if(sncs[i].time_in_ms==time){
      current_snc_edit=i;
      break;
    }
  }
  end_snc_edit=MAX_SECTIONS;
  return RET_OK;
}

static int edit_line(void){
  bool restore=rb->global_settings->repeat_mode+1!=RMODE_ONE;
  set_repeat_mode(RMODE_ONE);
  char* lyrics=sncs[current_snc_edit].x[SCROLL].lyrics[0];
  rb->strcpy(buf,lyrics);
  bool changed;
  if((changed=!rb->kbd_input(buf,BUFFERSIZE)))
  {
    unsigned length=rb->strlen(buf);
    /* append the edited lyrics at the end of the lyrics buffer */
    /* lyrics buffer: line1_line2_..lineN_new */
    struct SNCSection* snc=&sncs[current_snc_edit];
    SET_LYRICS(snc,&lyrics_buffer[lyrics_buffer_used],0);
    lyrics=snc->x[SCROLL].lyrics[0];
    rb->strcpy(lyrics,buf);
    store_line(snc,lyrics,true); /* format */
    /* check if there's enough space left? */
    lyrics_buffer_used+=length+1; /* +1: \0 */
    modified|=MODIFIED_LYRICS;
  }
  if(restore) set_repeat_mode(RMODE_CLEAR);
  update();
  return changed;
}

static int edit_and_dup_lines(void){
  size_t size;
  /* backup old line */
  unsigned char* old_lyrics=rb->plugin_get_buffer(&size);
  rb->strcpy(old_lyrics,sncs[current_snc_edit].x[SCROLL].lyrics[0]);
  if(edit_line()){
    int i=1;
    for(;i<num_snc-1;i++){ /* look for all the lines with the same content */
      if(i!=current_snc_edit && lrccmp(old_lyrics,sncs[i].x[SCROLL].lyrics[0])==0)
        COPY_SNC((&sncs[i]),(&sncs[current_snc_edit]));
    }
  }
  return RET_OK;
}

static void val_as_num(long* val){
  rb->snprintf(buf,BUFFERSIZE,"%d  ",(int) *val);
  rb->lcd_putsxy(0,sysfont_height + prefs.fontheight,buf);
}

static void val_as_time(long* val)
{
  long pos_val=*val;
  char sign = '+';
  if(pos_val<0){
    sign = '-';
    pos_val = -pos_val;
  }
  rb->snprintf(buf,BUFFERSIZE,"%c" TIME_FORMAT ".%.1d", sign,
    CALC_MM(pos_val),CALC_SS(pos_val),CALC_MS(pos_val));
  rb->lcd_putsxy(0,sysfont_height + prefs.fontheight,buf);
}

static int set_val_screen(const char* title, int step,
    long* init_val, int min, void (*draw_cb)(long*), void (*upd_cb)(void)){
  long old_val=NOT_INIT;
  int button;
  long val=*init_val;
  bool loop=true;
  const int hz = HZ >> 1;
#if LCD_DEPTH > 1
  fb_data* bd = rb->lcd_get_backdrop();
  if(bd!=snc_backdrop)
    rb->lcd_set_backdrop(snc_backdrop);
  else
    bd = NULL;
#endif
  rb->lcd_clear_display();
  rb->lcd_putsxy(0,sysfont_height,title);
  do{
    if(old_val!=val){
      draw_cb(&val);
      old_val = val;
      rb->lcd_update();
    }
    button = rb->get_action(CONTEXT_SNC,hz);
    switch(button){
      case SNC_NEXT: val+=step/5; break;
      case SNC_PREV: val-=step/5; break;
      case SNC_SELECT_OR_MODE:
        *init_val=val;
      case SNC_QUIT:
        loop=false;
        break;
      case SNC_INC: val += step; break;
      case SNC_DEC: val -= step; break;
      case SNC_AB: val = 0; break;
      default:
        if(upd_cb!=NULL) upd_cb();
    }
    if(val<min) val=min;
    if(id3!=rb->audio_current_track()){ /* track has changed -> exit */
      return 0;
    }
  } while(loop);
#if LCD_DEPTH > 1
  if(bd!=NULL)
    rb->lcd_set_backdrop(bd);
#endif
  return button;
}
static int set_time_offset(void)
{
  if(NO_LYRICS()) return RET_NOK;
  /* save old value */
  long offset = time_offset;
  bool as = auto_scroll;
  int sb = rb->global_settings->statusbar;
  rb->global_settings->statusbar = 0;
  int i, start=1, stop=num_snc-1;

  auto_scroll = true;
  set_repeat_mode(RMODE_ONE);
  if(start_snc!=NOT_INIT){
    start=start_snc;
    if(stop_snc!=NOT_INIT) stop=stop_snc;
  }
  unsigned long t0 = sncs[start].time_in_ms;

  void val_changed(long* val){
    val_as_time(val);
    offset=*val-offset; /* calculate change */
    if(offset!=0){
      for(i=start;i<stop;i++){
        if(sncs[i].time_in_ms==MAX_ULONG) break;
        sncs[i].time_in_ms+=offset;
      }
    }
    offset = *val;
    load_bmp(0);
    scroll_snc();
  }
  void upd_cb(void){
    update();
    update_time_display(AUDIO_ELAPSED(), false);
  }
  update_display();
  if(set_val_screen(STR_(SNC_L_TIMEOFFSET),500,&time_offset,
      start_snc==NOT_INIT ? -sncs[1].time_in_ms+2 : -sncs[start_snc].time_in_ms,
      val_changed, upd_cb)!=SNC_QUIT){
    modified = (time_offset!=0)?
      modified|MODIFIED_OFFSET_TIMETAG:modified&~MODIFIED_OFFSET_TIMETAG;
  }
  else{   /* cancelled */
    offset = sncs[start].time_in_ms;
    val_changed(&t0);
  }
  /* restore */
  auto_scroll = as;
  set_repeat_mode(RMODE_CLEAR);
  rb->global_settings->statusbar = sb;
  return RET_OK;
}

static int set_auto_cue(void){
  long old = prefs.cue_interval;
  set_val_screen(STR_(SNC_L_CUE_INTERVAL),10000, &prefs.cue_interval, 0, val_as_time, NULL); /* 10 s */
  if(old == prefs.cue_interval)
    return RET_NOK;
  auto_cue();
  if(old == 0 || prefs.cue_interval == 0) {
    load_bmp(1); /* reload AA */
  }
  prefs.save=PREF_SAVE;
  return RET_OK;
}

static int colors_menu(void){
  #define COLOR_OFFSET 2
  const char* const color_titles[] = {
     STR_(SNC_L_RESET)
    ,STR_(SNC_L_INVERT)
    ,STR_(SNC_L_FOREGROUND)
    ,STR_(SNC_L_BACKGROUND)
    ,STR_(SNC_L_INACTIVE)
    ,STR_(SNC_L_HIGHLIGHT)
    ,STR_(SNC_L_WARNING)
    ,STR_(SNC_L_ELAPSED_TIME)
    ,STR_(SNC_L_REMAINING_TIME)
    ,STR_(SNC_L_TRANSLATION)
  };
  const int num_colors = COLOR_OFFSET + sizeof(color_items)/sizeof(unsigned*);

  enum themable_icons list_color_icon(int item, void * data){
    (void) data;
    if(item == 1) return prefs.invert_colors ? Icon_Cursor : Icon_Recording;
    return item < COLOR_OFFSET ? Icon_Menu_functioncall : Icon_NOICON;
  }
#ifdef HAVE_LCD_COLOR
  int list_color_cb(int item, void * data){
    (void) data;
    return item < COLOR_OFFSET ?
      rb->global_settings->fg_color : (int)*(color_items[item-COLOR_OFFSET]);
  }
#endif
  int list_cb(int action, struct gui_synclist *lists){
#ifdef HAVE_LCD_COLOR
    if(lists->callback_get_item_color == NULL) /* hack */
      lists->callback_get_item_color = list_color_cb;
#endif
    if(action != ACTION_STD_OK) return action;
    int sel = rb->gui_synclist_get_sel_pos(lists);
#if LCD_DEPTH > 1
    unsigned bg = prefs.bg_color;
#endif
    switch(sel){
      case 0:
      {
        if(prefs.invert_colors)
        {
          prefs.invert_colors = !prefs.invert_colors;
          invert_colors(true);
        }
        reset_colors();
        rb->splash(HZ, STR_(SNC_L_COLORS_CLEARED));
        break;
      }
      case 1:
        prefs.invert_colors = !prefs.invert_colors;
        invert_colors(true);
        break;
      default:
      {
#ifdef HAVE_LCD_COLOR
        rb->set_color(
          NULL, (char*) color_titles[sel], color_items[sel-COLOR_OFFSET], -1);
#elif LCD_DEPTH > 1
        #define NUM_COLORS 4
        unsigned colors[]={LCD_BLACK, LCD_DARKGRAY, LCD_LIGHTGRAY, LCD_WHITE};
        const char* items[] = {
           STR_(SNC_L_BLACK)
          ,STR_(SNC_L_DARK_GREY)
          ,STR_(SNC_L_LIGHT_GREY)
          ,STR_(SNC_L_WHITE)};
        int i = 0;
        while(colors[i]!=*(color_items[sel-COLOR_OFFSET]) && i<NUM_COLORS) i++;
        i = do_list(color_titles[sel], Icon_Display_menu, NUM_COLORS, items, NULL, i, NUM_COLORS);
        if(i >= 0)
          *(color_items[sel-COLOR_OFFSET]) = colors[i];
        #undef NUM_COLORS
#endif
      }
    }
#if LCD_DEPTH > 1
    if(bg != prefs.bg_color)
      rb->lcd_set_background(prefs.bg_color);
#endif
    prefs.save=PREF_SAVE;
    return ACTION_REDRAW;
  }
  struct simplelist_info list;
  rb->simplelist_info_init(&list, STR_(SNC_L_COLORS), num_colors, (void*)color_titles);
  list.get_icon = list_color_icon;
  list.get_name = std_list_name;
  list.action_callback = list_cb;
  list.selection = 0;
  rb->simplelist_show_list(&list);

  #undef COLOR_OFFSET
  return list.selection >= 0 ? RET_OK : RET_NOK;
}
/****************************************************************/
/* dictionary definitions only */
#define DICT_EXT "dic"
#define MAX_INDEX 200
#define MAX_WORDS 30
#define MAX_DICTS 10
#define KEY_LENGTH 3
#define KB_BUF_LEN 60
#define LF 10
struct dict_index_struct{
  unsigned long pos;
  char key[KEY_LENGTH];
};

static unsigned long get_dict_pos(const struct dict_index_struct* dict_index,
    const char* word){
  int i=1;
  int sign=NOT_INIT,res=0;
  int len=rb->strlen(word);
  if(len>KEY_LENGTH) len=KEY_LENGTH;
  for(;i<MAX_INDEX;i++){
    if(dict_index[i].pos==0 || /* end reached */
      (res=rb->strncasecmp(word,dict_index[i].key,len)) == 0)
      break;

    if(sign != NOT_INIT){
      if(sign != (res >= 0)) /* sign has changed! */
        break;
    }
    else
      sign = res >= 0; /* sign = 1 if res positiv */

  }
  if(--i < 0) /* return previous index */
    i = 0;
  return dict_index[i].pos;
}

static int create_dict_index(struct dict_index_struct* dict_index,
    const char* dict, const char* idx){
  int fd=rb->open_utf8(dict, O_RDONLY);
  if(fd<0) return RET_NOK;
#if LCD_DEPTH > 1
  if(rb->lcd_get_backdrop()!=NULL){
    rb->lcd_set_backdrop(NULL);
    rb->lcd_update();
  }
#endif
  rb->splash(HZ, STR_(SNC_L_CREATING_INDEX));
  ssize_t section_size=rb->filesize(fd)/MAX_INDEX;
  size_t size;
  char* buffer=rb->plugin_get_buffer(&size);
  char* old_key = "???";
  ssize_t len;
  ssize_t plen;
  int i=0;
  unsigned long offset=0;
  char* pos;
  char* end;
  while((len=rb->read(fd,buffer,size))>0){
    pos=buffer;
    pos[len-1]=LF;
    end = buffer + len;
    if(i!=0){
      while(pos < end && *pos++!=LF);
      old_key = pos; /* to prevent the key not being the 1st entry */
    }
    while((plen=pos-buffer)<len && i+1<MAX_INDEX){
      if(rb->strncasecmp(old_key,pos,KEY_LENGTH)!=0){
        rb->memcpy(dict_index[i].key,pos,KEY_LENGTH);
        dict_index[i++].pos=offset+plen;
        if(pos + section_size >= end)
          break;
        pos+=section_size; /* move to next section */
        while(pos < end && *pos++!=LF); /* ensure the next read pos will start at ^*/
      }
      old_key=pos;
      while(pos < end && *pos++!=LF); /* next line */
    }
    offset+=size;
  }
  rb->close(fd);
  dict_index[i].pos=0; /* terminate */
  fd = rb->creat(idx,0666);
  rb->write(fd, dict_index, sizeof(struct dict_index_struct)*i);
  rb->close(fd);
  return RET_OK;
}

static int open_dict(struct dict_index_struct* dict_index){
  /* open default dictionary if the language is not set */
  const char* lang = language[0]==0 ? APP_NAME : language;
  char idx[SNC_MAX_PATH];
  char dict[SNC_MAX_PATH];
  rb->snprintf(idx,SNC_MAX_PATH,SNCLRC_DATA_DIR"/%s.idx",lang);
  rb->snprintf(dict,SNC_MAX_PATH,SNCLRC_DATA_DIR"/%s."DICT_EXT,lang);

  int fd=rb->open(idx, O_RDONLY);
  if(fd >= 0){
    rb->read(fd,dict_index,sizeof(struct dict_index_struct)*MAX_INDEX);
    rb->close(fd);
  }
  else if(create_dict_index(dict_index,dict,idx)!=RET_OK){
    rb->splashf(HZ, STR_(SNC_L_OPEN_ERROR), dict);
    return RET_NOK;
  }
  /* open dictionary */
  return rb->open_utf8(dict, O_RDONLY);
}

static int change_dict(int* fd, struct dict_index_struct* dict_index){
  void cb(int selected, const char** items){
    language[0] = 0;
    if(rb->strlen(items[selected])-rb->strlen(DICT_EXT)-1 == 2){
      language[0] = items[selected][0];
      language[1] = items[selected][1];
      language[2] = 0;
    }
    if(*fd>=0) rb->close(*fd);
    *fd = open_dict(dict_index);
    if(!NO_LYRICS())
      modified = 1;
  }
  list_dir(STR_(SNC_L_DICTIONARY), DICT_EXT, Icon_Language, cb);
  return *fd;
}

static int lookup_word(void){
  struct dict_index_struct dict_index[MAX_INDEX];

  /* parse line (build menu) */
  struct Dict_Entry{
    const char* word;
    int length;
  } word_list[MAX_WORDS]; /* wordlist: items[0] items[1] ... */
  char* items[MAX_WORDS];

  const char* new_pos;
  int id = auto_scroll ? current_snc : current_snc_edit;
  const char* pos= id > 0 ?
    sncs[id].x[SCROLL].lyrics[0] : g_tit.lyrics[0];
  #define I1ST 2
  int byte,i=I1ST;

  items[0]=STR_(SNC_L_CHANGE_DICT);
  items[1]=STR_(SNC_L_KEYBOARD);
  items[I1ST]=buf;
  while(*pos != 0 && i<MAX_WORDS) {     /*  loop until end of string	 */
    while(is_wrap_char_or_digit(pos)) pos++;
    if(*pos == 0) break;
    word_list[i].word=pos;
    word_list[i].length=0;
    new_pos=pos;
    do{
      new_pos=utf_char_width(new_pos,&byte, NULL);
      word_list[i].length+=byte;
    } while(byte<3 && !(is_wrap_char_or_digit(new_pos) || *new_pos==0));
    rb->strlcpy(items[i],pos,word_list[i].length+1); /* copy word into item list */
    items[i+1]=items[i]+word_list[i].length+1; /* next item */
    pos=new_pos;
    i++;
  }
  if(i==I1ST) return RET_NOK;

  int cmp_len;
  const char* search_word;
  char* entries[MAX_WORDS];
  char* str;
  size_t size;
  ssize_t len, len_bak;
  char* const plugin_buffer = rb->plugin_get_buffer(&size);
  char kb_buffer[KB_BUF_LEN];
  kb_buffer[0]=0;
  int fd=-1;
  ssize_t read_next_block(void){
    len=rb->read(fd,plugin_buffer,
      dict_index[2].pos < size ? dict_index[2].pos : size); /* read two sections */
    len_bak = len;
    plugin_buffer[len-1]=0; /* ensure termination */
    return len;
  }

  int list_callback(int action, struct gui_synclist *lists){
    if(action != ACTION_STD_OK
    && action != ACTION_STD_CONTEXT
    && action != ACTION_STD_MENU)
      return action;
    int selected = rb->gui_synclist_get_sel_pos(lists);
    if(action == ACTION_STD_CONTEXT){
      rb->strcpy(kb_buffer,items[selected]);/* put selected word into keybord buffer */
      selected = 1;	/* call keyboard input */
    }

    switch(selected){
      case 0:
        change_dict(&fd, dict_index);
        return ACTION_REDRAW;
      case 1:
      {
        if(rb->kbd_input(kb_buffer,KB_BUF_LEN) || kb_buffer[0]==0)
          return ACTION_REDRAW; /* cancel input: return to menu */
        search_word = kb_buffer;
        cmp_len = rb->strlen(search_word);
        break;
      }
      default:
      {
        search_word = word_list[selected].word;
        cmp_len = word_list[selected].length;
        if(action == ACTION_STD_MENU && selected+1 < lists->nb_items){
          cmp_len += word_list[selected+1].length;
          if(*(word_list[selected+1].word-1)==' ')
            cmp_len++; /* there's a space between the words */
        }
      }
    }
    if(fd<0) fd = open_dict(dict_index);
    if(fd<0) return RET_NOK;
    rb->splash(HZ, STR_(SNC_L_SEARCHING));
    rb->lseek(fd,get_dict_pos(dict_index,search_word),SEEK_SET);
    read_next_block(); /* first block */

    SEEKING_WORD:
    entries[0] = str = plugin_buffer;

    NEXT_PAGE:
    i=0;
    do{
      while(*str!=LF && *str!=0) str++; /* set str to end of line */
      *str++=0;
      len-=(str-entries[i]);
      if(rb->strncasecmp(search_word, entries[i], cmp_len)==0){
        /* found */
        i++;
        if(len<=0) read_next_block();
      }
      else if(i!=0) break; /* stop if it doesn't match */
    }while(len>0 &&
           i+1<MAX_WORDS &&  /* last entry is <NEXT> */
          *(entries[i]=str)!=0);
    if(i>0){ /* show found entries */
      if(i+1==MAX_WORDS) entries[i++] = STR_(SNC_L_NEXT);
      int sel = do_list(search_word, Icon_NOICON, i, entries, NULL, 0, i);
      if(sel >= 0){
        if(sel + 1 == MAX_WORDS){
          entries[0]=str; /* next entries */
          goto NEXT_PAGE;
        }
        else
          rb->strlcpy(kb_buffer,entries[sel],KB_BUF_LEN);
      }
    }
    else {
      if(cmp_len>KEY_LENGTH){
        /* no exact match found, try with parts of the word */
        cmp_len >>= 1;
        if(cmp_len<KEY_LENGTH)
          cmp_len = KEY_LENGTH;
        len=len_bak;
        goto SEEKING_WORD;
      }
      else if(read_next_block()>0){
        /* maybe the search word is in the next block? */
        goto SEEKING_WORD;
      }
      else
        rb->splashf(HZ, STR_(SNC_L_WORD_NOT_FOUND),
          selected>=I1ST ? items[selected] : kb_buffer);
    }
    return ACTION_REDRAW;
  }
  do_list(language, Icon_Menu_functioncall, i, items, list_callback, I1ST, I1ST);
  rb->close(fd);
  return RET_OK;
}

static void set_lang_cb(int selected, const char** items){
  rb->snprintf(prefs.lang_file, SNC_MAX_PATH, "%s/%s", SNCLRC_DATA_DIR, items[selected]);
  snc_lang_load(prefs.lang_file);
  prefs.save = PREF_SAVE;
}

static int set_lang_file(void){
  return list_dir(STR_(SNC_L_LANGUAGE), "lng", Icon_Language, set_lang_cb);
}
#ifdef CUSTOM_PLUGIN_PATCH
/* execute a function with possible track change */
static void exec_func_tc(bool (*func)(void)){
  unsigned long l = id3->length;
#ifdef HAVE_LCD_BITMAP
  int i;
  FOR_NB_SCREENS(i)
    rb->viewportmanager_theme_enable(i, true, NULL);
#endif
  func();
#ifdef HAVE_LCD_BITMAP
  FOR_NB_SCREENS(i)
    rb->viewportmanager_theme_undo(i, false);
#endif
  rb->lcd_stop_scroll();
  if(l != rb->audio_current_track()->length)
    reset(true);
  else{
    g_next_at.rows=0; /* force reread next title */
    update();
  }
}

static int show_playlist(void){
  exec_func_tc((bool (*)(void))rb->playlist_viewer);
  return RET_OK;
}

static int browse_dir(void){
  bool browse_dir_(void){
    int ret = 0;
    while((ret = rb->rockbox_browse(
      ret!=GO_TO_ROOT ? id3->path:"/",SHOW_MUSIC))== GO_TO_ROOT);
    return ret;
  }
  exec_func_tc(browse_dir_);
  return RET_OK;
}

static int show_id3(void){
  exec_func_tc(rb->browse_id3);
  return RET_OK;
}
#endif

static int delete_lyrics_file(void){
  if(NO_LYRICS() || !rb->file_exists(lyrics_filename)) return RET_NOK;
  char* ext=rb->strrchr(lyrics_filename,'/')+1;
  rb->snprintf(buf,BUFFERSIZE,STR_(SNC_L_DELETE_QUESTION),ext);
  if(yes_no(buf)){
    rb->remove(lyrics_filename);    /* remove lyrics file */
    if(prefs.load_translation==TR_OK){
      ext=rb->strrchr(lyrics_filename,'.')+1;
      rb->strcpy(ext,FORMATS[EXT_TR]);
      rb->remove(lyrics_filename); /* also remove translation file */
    }
    reset(true);    /* reset to ensure all is done */
  }
  return RET_OK;
}

/*
  clean_blanks:
  delete the blank snc, if the blank length is shorter than the set time
*/
static int clean_blanks(void){
  if(NO_LYRICS()) return RET_NOK;
  int id = 1;
  int num_blanks=0;
  long delta = 1000;
  if(set_val_screen(STR_(SNC_L_MIN_BLANK_LENGTH),500,&delta,500, val_as_time, NULL)==SNC_QUIT)
    return RET_NOK; /* cancelled */
  while(id < num_snc){
    if(IS_BLANK(id)
      && (sncs[id].time_in_ms == MAX_ULONG
          || (sncs[id].time_in_ms + delta > sncs[id+1].time_in_ms))){
      if(sncs[id].time_in_ms != MAX_ULONG){
        /* copy time from blank snc and delete the blank */
        sncs[id+1].time_in_ms = sncs[id].time_in_ms + 1;
      }
      current_snc_edit = id;
      start_snc = id;
      stop_snc = id + 1;
      delete_sections();
      num_blanks++;
    }
    id++;
  }
  /* report */
  if(num_blanks)
    rb->splashf(HZ, STR_(SNC_L_NUM_BLANKS_DELETED),num_blanks);
  return RET_OK;
}

/*
  set_scroll_limit:
  configure the vertical scroll limit
*/
static int set_scroll_limit(void){
  if(NO_LYRICS()) return RET_NOK; /* only possible with lyrics */
  int button;
  bool save = false;
  int old_val = prefs.scroll_limit_y;
  int sb = rb->global_settings->statusbar;
  rb->global_settings->statusbar = 0;
  auto_scroll = true;
  update_display();
  do{
    button=rb->get_action(CONTEXT_SNC,HZ>>1);
    switch (button) {
      case SNC_SELECT_OR_MODE:
        save = true;
        button = SNC_QUIT;
        break;
      case SNC_LIST_DOWN:
        if(prefs.scroll_limit_y + prefs.fontheight < LCD_HEIGHT - prefs.fontheight)
          prefs.scroll_limit_y += prefs.fontheight;
        break;
      case SNC_LIST_UP:
        prefs.scroll_limit_y -= prefs.fontheight;
        if(prefs.scroll_limit_y < scroll_y0)
          prefs.scroll_limit_y = scroll_y0;
        break;
    }
    update();
    (button==ACTION_NONE) ?
      update_time_display(AUDIO_ELAPSED(),false):
      update_display();
    /* show that we are in configuration mode */
    rb->lcd_set_drawmode(DRMODE_COMPLEMENT);
    rb->lcd_fillrect(SCROLL_MARKER_X,prefs.scroll_limit_y,
                     SCROLL_MARKER_WIDTH,prefs.fontheight);
    rb->lcd_set_drawmode(DRMODE_SOLID);
    rb->lcd_update();
  } while (button!=SNC_QUIT);
  rb->global_settings->statusbar = sb;
  if(save)
    prefs.save=PREF_SAVE;
  else
    prefs.scroll_limit_y = old_val;
  return RET_OK;
}

static void switch_bool_setting(int save_mask, bool* bsetting){
  prefs.save^=save_mask;
  *bsetting=!(*bsetting);
}

static int switch_karaoke(void){
  switch_bool_setting(PREF_KARAOKE, &prefs.karaoke);
  g_karaoke = prefs.karaoke && !NO_LYRICS();
  return RET_OK;
}
static int switch_backlight(void){
  switch_bool_setting(PREF_BACKLIGHT, &prefs.backlight);
  prefs.backlight? force_backlight_on():default_backlight();
  return RET_OK;
}
static int switch_peakmeter(void){
  switch_bool_setting(PREF_PEAKMETER, &prefs.peakmeter);
  load_bmp(1);
  return RET_OK;
}
static int switch_readid3(void){
  switch_bool_setting(PREF_READID3, &prefs.readid3);
  return RET_OK;
}
static int switch_readsnc(void){
  switch_bool_setting(PREF_READSNC, &prefs.readsnc);
  return RET_OK;
}
static int switch_tr_use_sysfont(void){
  switch_bool_setting(PREF_TRSYSFONT, &prefs.tr_use_sysfont);
  return ACTION_UNKNOWN;
}
static int switch_show_time(void){
  switch_bool_setting(PREF_TIMEVISIBLE, &prefs.time_visible);
  return ACTION_UNKNOWN;  /* call reset(true) */
}
static int switch_show_tr(void){
  switch_bool_setting(PREF_TRVISIBLE, &prefs.tr_visible);
  end_snc_edit = MAX_SECTIONS;
  return RET_OK;
}
static void exec_button_func(void){
  if(prefs.button.num == 0) return;
  static short func_id = 0;
  int aa_mode = prefs.load_bmp;
  if(prefs.button.num == 1)
    prefs.button.func[0](); /* execute function */
  else if(prefs.button.num > 1){
    prefs.button.func[func_id]();
    func_id = (func_id + 1)%prefs.button.num; /* next function */
    if(aa_mode == prefs.load_bmp){
      exec_button_func();
      return;
    }
  }
  update_display();
}

static int set_aa_size(void){
  void preview_aa_size(long* size){
    const int max_size = LCD_HEIGHT-(scroll_y0+prefs.fontheight);
    if(*size > max_size) *size = max_size;
    LCD_CLEAR_AREA(SCREEN_MAIN,0,scroll_y0, LCD_WIDTH, max_size);
    val_as_num(size);
    rb->lcd_drawrect(0,scroll_y0,*size,*size);
  }
  if(set_val_screen(STR_(SNC_L_AASIZE), 1, (long*) &prefs.sxs_bmp, 30,
      preview_aa_size, NULL) != SNC_QUIT){
    prefs.save=PREF_SAVE;
    reset(false);
  }
  return RET_OK;
}

static void switch_album_art(int mode){
  if(mode == prefs.load_bmp) return;
  prefs.save=PREF_SAVE;
  prefs.load_bmp=mode;
  if(!prefs.load_bmp)
    bmp_width=bmp_height=NOT_INIT;
  reset(false);
}
static int switch_aa_0(void){
  switch_album_art(0);
  return RET_OK;
}
static int switch_aa_w(void){
  switch_album_art(BMP_WEST);
  return RET_OK;
}
static int switch_aa_s(void){
  switch_album_art(BMP_SOUTH);
  return RET_OK;
}
static int switch_aa_c(void){
  switch_album_art(BMP_CENTER);
  return RET_OK;
}
static int album_art_menu(void)
{
  const char* get_list_name(int item, void* data,
                       char * buffer, size_t buffer_len){
    (void) buffer;
    (void) buffer_len;
    (void) data;
    return STR_(func_items[item].lang_id);
  }
  enum themable_icons get_list_icon(int item, void * data){
    (void) data;
    if(item == NUM_AA_ITEMS-1)
      return Icon_Menu_functioncall;
    return prefs.load_bmp==item ? Icon_Cursor : Icon_Recording;
  }
  int list_callback(int action, struct gui_synclist *lists){
    if(action != ACTION_STD_OK) return action;
    func_items[rb->gui_synclist_get_sel_pos(lists)].func();
    return ACTION_STD_CANCEL;
  }
  struct simplelist_info list;
  rb->simplelist_info_init(&list, STR_(SNC_L_AA), NUM_AA_ITEMS, NULL);
  list.get_icon = get_list_icon;
  list.get_name = get_list_name;
  list.action_callback = list_callback;
  list.selection = prefs.load_bmp;
  rb->simplelist_show_list(&list);
  return list.selection >= 0 ? RET_OK : RET_NOK;
}

static int set_scrolling(void){
  const char* items[] = {
      STR_(SNC_L_SCROLL_LIMIT)
    , STR_(SNC_L_LINE_SCROLL_START_DELAY)
    , STR_(SNC_L_LINE_SCROLL_DELAY)
  };
  int selected =
    do_list(STR_(SNC_L_SCROLLING), Icon_Moving, 3, items, NULL, 0, 3);
  if(selected < 0)
    return RET_NOK; /* cancelled */
  if(selected == 0)
    return set_scroll_limit();

  if(set_val_screen(
      items[selected], 1,
      (long*) (selected == 2 ? &prefs.vscroll_speed:&prefs.vscroll_delay), 0,
      val_as_num, NULL) != SNC_QUIT){
    prefs.save=PREF_SAVE;
  }
  return RET_OK;
}

static int switch_repeat1(void){
  set_repeat_mode(rb->global_settings->repeat_mode+1!=RMODE_ONE ?
                  RMODE_ONE:RMODE_CLEAR);
  return RET_OK;
}

static int load_translation(void){
  confirm_saving();
  prefs.save^=PREF_TRANSLATION;
  prefs.load_translation = prefs.load_translation == TR_OFF ? TR_OK : TR_OFF;
  unsigned long a=MAX_ULONG, b=MAX_ULONG, c=MAX_ULONG;
  if(start_snc!=NOT_INIT)
    a = sncs[start_snc].time_in_ms;
  if(stop_snc!=NOT_INIT)
    b = sncs[stop_snc].time_in_ms;
  c = sncs[current_snc_edit].time_in_ms;
  if(prefs.load_translation){
    load_file(EXT_TR);
    end_snc_edit=MAX_SECTIONS;
    update_display();
  }
  else reset(false);

  start_snc = get_snc_index(a);
  stop_snc = get_snc_index(b);
  current_snc_edit = get_snc_index(c);
  return RET_OK;
}

static int config_button(void)
{
  int num_selected = prefs.button.num;
  const int num_funcs = sizeof(func_items)/sizeof(struct FuncItem);
  int i;
  bool changed = false;
  const char* get_list_name(int item, void * data,
                       char * buffer, size_t buffer_len){
    (void) buffer;
    (void) buffer_len;
    return STR_(((struct FuncItem*) data )[item].lang_id);
  }
  enum themable_icons get_list_icon(int item, void * data){
    return ((struct FuncItem*) data )[item].selected ?
      Icon_Cursor : Icon_NOICON;
  }
  int list_callback(int action, struct gui_synclist *lists){
    if(action != ACTION_STD_OK) return action;
    int n = rb->gui_synclist_get_sel_pos(lists);
    if(func_items[n].selected){
      func_items[n].selected = false;
      num_selected--;
    }
    else {
      for(i=0;num_selected > 0 && i<num_funcs;i++){
        if(num_selected == MAX_BUTTON_FUNC ||
            (
               func_items[i].selected &&
               (
                  func_items[i].allow2nd == false ||
                  func_items[n].allow2nd == false
               )
            )
          )
        {
          if(func_items[i].selected) num_selected--;
          func_items[i].selected = false;
        }
      }
      func_items[n].selected = true;
      num_selected++;
    }
    changed = true;
    return ACTION_REDRAW;
  }

  /* get selections */
  for(i=0; i<prefs.button.num; i++)
    func_items[prefs.button.id[i]].selected = true;

  struct simplelist_info list;
  rb->simplelist_info_init(&list, STR_(SNC_L_CONF_BUTTON), num_funcs, func_items);
  list.get_icon = get_list_icon;
  list.get_name = get_list_name;
  list.action_callback = list_callback;
  list.selection = 0;
  rb->simplelist_show_list(&list);
  if(changed){
    /* clear button functions */
    prefs.button.num = 0;
    /* set selections */
    for(i=0;i<num_funcs;i++){
      if(func_items[i].selected){
        prefs.button.func[prefs.button.num] = func_items[i].func;
        prefs.button.id[prefs.button.num] = i;
        prefs.button.num++;
      }
    }
    prefs.save = PREF_SAVE;
  }
  return list.selection >= 0 ? RET_OK : RET_NOK;
}

static int _setting_menu(
  const char** settings,
  short num_offset,
  struct simplelist_info* list,
  int (*settings_func_cb)(int))
{
  int ret = RET_OK;
  int tmp_ret;
  const char* get_list_name(int item, void* data,
                       char * buffer, size_t buffer_len){
    (void) buffer;
    (void) buffer_len;
    struct FuncItem* funcs = (struct FuncItem*) data;
    return item < num_offset ? settings[item] :
      (const char*) STR_(funcs[item-num_offset].lang_id);
  }
  enum themable_icons get_list_icon(int item, void * data){
    struct FuncItem* funcs = (struct FuncItem*) data;
    return item < num_offset ? Icon_Menu_functioncall :
      (*((bool*)(funcs[item-num_offset].value)) ? Icon_Cursor : Icon_Recording);
  }
  int list_callback(int action, struct gui_synclist *lists){
    if(action != ACTION_STD_OK) return action;
    int sel = rb->gui_synclist_get_sel_pos(lists);
    if(sel < num_offset)
      return settings_func_cb(sel);
    else{
      tmp_ret = ((struct FuncItem*)lists->data)[sel-num_offset].func();
      /* only overwrite ret, when it's still RET_OK */
      if(ret == RET_OK) ret = tmp_ret;
      else if(ret == RET_NOK && tmp_ret != RET_OK) ret = tmp_ret;
    }
    return ACTION_REDRAW;
  }

  list->get_icon = get_list_icon;
  list->get_name = get_list_name;
  list->action_callback = list_callback;
  list->selection = 0;
  rb->simplelist_show_list(list);

  switch(ret){
    case RET_OK: break; /* do nothing */
    case RET_NOK: reset(false); break;
    default:{
#if LCD_DEPTH > 1
      fb_data* bd = rb->lcd_get_backdrop();
      reset(true);
      rb->lcd_set_backdrop(bd);
#else
      reset(true);
#endif
    }
  }
  return list->selection >= 0 ? RET_OK : RET_NOK;
}

static int edit_setting_menu(void)
{
  struct FuncItem funcs[] = {
    {SNC_L_DISPLAY_TIME, switch_show_time, false, false, &prefs.time_visible},
    {SNC_L_DISPLAY_TR, switch_show_tr, false, false, &prefs.tr_visible},
  };
  struct simplelist_info list;
  rb->simplelist_info_init(
    &list, STR_(SNC_L_EDIT_SETTINGS), sizeof(funcs)/sizeof(struct FuncItem), funcs);
  return _setting_menu(NULL, 0, &list, NULL);
}

static int setting_menu(void)
{
  #define _SETTINGS_OFFSET 2
  const char* settings_func[_SETTINGS_OFFSET] = {STR_(SNC_L_LOAD), STR_(SNC_L_SAVE_AS)};

  void load_settings_cb(int selected, const char** items){
    rb->snprintf(buf, BUFFERSIZE, SNCLRC_DATA_DIR"/%s", items[selected]);
    if(load_settings(buf))
      reset(true);
  }
  int settings_func_cb(int sel){
    switch(sel){
      case 0:
        if(RET_OK == list_dir(STR_(SNC_L_SETTINGS), "dat", Icon_Config, load_settings_cb))
          return ACTION_STD_CANCEL;
        break;
      case 1:{
        rb->strlcpy(buf, SNCLRC_DATA_DIR"/1", BUFFERSIZE);
        int len = rb->strlen(SNCLRC_DATA_DIR) + 1;
        if(!rb->kbd_input(buf+len,BUFFERSIZE-len)){
          rb->strcat(buf,".dat");
          save_settings(buf);
        }
        break;
      }
    }
    return ACTION_REDRAW;
  }
  struct simplelist_info list;
  rb->simplelist_info_init(&list, STR_(SNC_L_SETTINGS),
    _SETTINGS_OFFSET + NUM_SETTINGS_ITEMS,
    &func_items[SETTINGS_OFFSET]);

  int ret = _setting_menu(settings_func, _SETTINGS_OFFSET, &list, settings_func_cb);
  #undef _SETTINGS_OFFSET

  return ret;
}

enum Menu_attr {FUNC_MENU, SUB_MENU, DLG_MENU};
struct MenuItem{
  enum Lang_ID lang_id;
  int(*func) (void);
  enum themable_icons icon;
  enum Menu_attr attr;
};

static const char* get_menu_name(int item, void* data,
                     char * buffer, size_t buffer_len){
  (void) buffer;
  (void) buffer_len;
  struct MenuItem* pitem = &(((struct MenuItem*) data)[item]);
  if(pitem->attr == FUNC_MENU)
    return STR_(pitem->lang_id);
  rb->snprintf(buf, BUFFERSIZE, "%s %s", STR_(pitem->lang_id),
    pitem->attr == SUB_MENU ? ">" : "...");
  return buf;
}

static enum themable_icons get_menu_icon(int item, void * data){
  return ((struct MenuItem*) data)[item].icon;
}

static int menu_cb(int action, struct gui_synclist *lists){
  if(action != ACTION_STD_OK) return action;
  return
    ((struct MenuItem*)lists->data)[rb->gui_synclist_get_sel_pos(lists)].func();
}

static void show_menu(void)
{
  static int edit_selected=0;
  static int scroll_selected=0;

  #define GENERAL_MENU_ITEMS \
    {SNC_L_DICTIONARY, lookup_word, Icon_Language, FUNC_MENU}, \
    {SNC_L_TRANSLATION, load_translation, Icon_Language, FUNC_MENU}, \
    {SNC_L_TIMEOFFSET, set_time_offset,Icon_Menu_functioncall, DLG_MENU}, \
    {SNC_L_REPEAT1, switch_repeat1, Icon_Audio, FUNC_MENU}, \
    {SNC_L_AUTOCUE, set_auto_cue, Icon_Menu_functioncall, DLG_MENU}, \
    {SNC_L_CLEAN_BLANKS, clean_blanks, Icon_Menu_functioncall, DLG_MENU}, \
    {SNC_L_DELETE_FILE, delete_lyrics_file, Icon_Recording, DLG_MENU}, \
    {SNC_L_FILE_LIST, file_list_menu, Icon_file_view_menu, DLG_MENU}, \
    {SNC_L_COLORS, colors_menu, Icon_Display_menu, SUB_MENU}, \
    {SNC_L_LANGUAGE, set_lang_file, Icon_Language, SUB_MENU}, \
    {SNC_L_SETTINGS, setting_menu, Icon_Config, SUB_MENU},
#ifdef CUSTOM_PLUGIN_PATCH
  #define CUSTOM_MENU_ITEMS \
    {SNC_L_ID3INFO, show_id3, Icon_Questionmark, FUNC_MENU}, \
    {SNC_L_PLAYLIST, show_playlist, Icon_Playlist, FUNC_MENU}, \
    {SNC_L_FILES, browse_dir, Icon_Folder, FUNC_MENU},
#else
  #define CUSTOM_MENU_ITEMS
#endif

  static struct MenuItem edit_items[]={
    {SNC_L_INSERT_LINE, insert_sections, Icon_Menu_functioncall, FUNC_MENU},
    {SNC_L_DELETE_LINE, delete_sections, Icon_Menu_functioncall, FUNC_MENU},
    {SNC_L_JOIN_LINES, join_sections, Icon_Menu_functioncall, FUNC_MENU},
    {SNC_L_EDIT_LINES, edit_and_dup_lines, Icon_Menu_functioncall, FUNC_MENU},
    {SNC_L_EDIT_LINE, edit_line, Icon_Menu_functioncall, FUNC_MENU},
    {SNC_L_EDIT_SETTINGS, edit_setting_menu, Icon_Display_menu, SUB_MENU},
    GENERAL_MENU_ITEMS
    CUSTOM_MENU_ITEMS
  };

  static struct MenuItem scroll_items[]={
    GENERAL_MENU_ITEMS
    {SNC_L_AA, album_art_menu, Icon_Display_menu, SUB_MENU},
    {SNC_L_SCROLLING, set_scrolling, Icon_Moving, SUB_MENU},
    {SNC_L_CONF_BUTTON, config_button, Icon_Config, false},
    CUSTOM_MENU_ITEMS
  };

  struct simplelist_info list;
  if(auto_scroll){
    rb->simplelist_info_init(
      &list, "", sizeof(scroll_items)/sizeof(struct MenuItem), scroll_items);
    list.selection = scroll_selected;
  }
  else{
    rb->simplelist_info_init(
      &list, STR_(SNC_L_EDIT), sizeof(edit_items)/sizeof(struct MenuItem), edit_items);
    list.selection = edit_selected;
  }
  list.get_icon = get_menu_icon;
  list.get_name = get_menu_name;
  list.action_callback = menu_cb;
  rb->simplelist_show_list(&list);

  if(list.selection > -1){
    if(auto_scroll)
      scroll_selected = list.selection;
    else
      edit_selected = list.selection;
  }
  update_display();
}

static void set_time_tag(void)
{
  if(auto_scroll || current_snc_edit==0 || current_snc_edit+1==num_snc)
    return;
  unsigned long t = AUDIO_ELAPSED()-SET_TIME_LATENCY; /* reaction time */
  if(sncs[current_snc_edit].type == TYPE_TR)
    current_snc_edit--;
  sncs[current_snc_edit].time_in_ms=t;
  current_snc_edit++;
  if(sncs[current_snc_edit].type == TYPE_TR){
    sncs[current_snc_edit].time_in_ms=t;
    current_snc_edit++;
  }
  if(current_snc_edit+1 >= num_snc)
    current_snc_edit=num_snc-2;
  modified|=MODIFIED_TIMETAG;
  update_status_display();
  browse_snc();
}

static int handle_button_scroll(int button){
  switch (button) {
    case SNC_TSTAMP_HK:
      exec_button_func();
      break;
    case SNC_DEC:
      set_volume(rb->global_settings->volume - 1);
      break;
    case SNC_INC:
      set_volume(rb->global_settings->volume + 1);
      break;
    case SNC_SELECT_OR_MODE:
      switch_mode(false);
      break;
    default:button=ACTION_NONE;
  }
  return button;
}
static int handle_button_edit(int button){
  static const int TIMEOUT = HZ>>2;
  switch (button) {
    case SNC_TSTAMP_HK:
      set_time_tag();
      break;
    case SNC_LIST_DOWN:
      if(++current_snc_edit>=num_snc){
        current_snc_edit = 1;
        while(rb->button_get_w_tmo(TIMEOUT)&BUTTON_REPEAT){
          current_snc_edit=num_snc-1;
          rb->yield();
        }
      }
      if(sncs[current_snc_edit].type == TYPE_TR && prefs.tr_visible == false)
        current_snc_edit++;
      browse_snc();
      break;
    case SNC_LIST_UP:
      if(--current_snc_edit<0){
        current_snc_edit = num_snc-2;
        while(rb->button_get_w_tmo(TIMEOUT)&BUTTON_REPEAT){
          current_snc_edit=0;
          rb->yield();
        }
      }
      if(sncs[current_snc_edit].type == TYPE_TR && prefs.tr_visible == false)
        current_snc_edit--;
      browse_snc();
      break;
    case SNC_SELECT_OR_MODE:
      ff_rew_snc(current_snc_edit);
      break;
    default:
      button=ACTION_NONE;
  }
  return button;
}

/* this is the plugin entry point */
enum plugin_status plugin_start(const void* parameter)
{
  (void)parameter;
  rb_lang_init(snc_lang_builtin, snc_lang_strings, SNC_L_LAST_INDEX_IN_ARRAY);

  if(!rb->dir_exists(SNCLRC_DATA_DIR)){
    rb->mkdir(SNCLRC_DATA_DIR);
  }
#if LCD_DEPTH > 1
  struct bitmap backdrop;
  size_t buffer_size;
  backdrop.data = rb->plugin_get_buffer(&buffer_size);
  const int max_backdrop_size = LCD_HEIGHT * LCD_WIDTH * sizeof(fb_data);
  int size = 0;
  if(buffer_size > (size_t) max_backdrop_size){
    /* move pointer to the end of the buffer */
    backdrop.data += buffer_size - max_backdrop_size;
    size = rb->read_bmp_file(SNCLRC_DATA_DIR"/"APP_NAME"_bd.bmp",
      &backdrop, max_backdrop_size, FORMAT_NATIVE, NULL);
  }
  snc_backdrop = size > 0 ? (fb_data*) backdrop.data : NULL;
  rb->lcd_set_backdrop(snc_backdrop);
#endif
  /* init */
  sysfont_height = rb->font_get(FONT_SYSFIXED)->height;
  auto_scroll = true;
  file_list.num = 0;
  prefs.invert_colors = false;
  load_settings(SETTINGS_FILE);

  int button;
  long newPos=NOT_INIT;
  const int short_hz = HZ >> 3;
  const int long_hz = HZ >> 1;
  const int dbl_click_hz = HZ >> 2;
  #define PLUGIN_STAY 333
  int status=PLUGIN_STAY; /* see enum plugin_status */
  if(rb->audio_status() & AUDIO_STATUS_PLAY)
    reset(true);
  else{
    rb->splash(HZ, STR_(SNC_L_AUDIO_PLAYING));
    status = PLUGIN_OK;
  }
  while(status==PLUGIN_STAY){
    button=rb->get_action(CONTEXT_SNC, g_karaoke ? short_hz : long_hz);
    update_status_busy(button);
    switch (button) { /* valid for both modes */
#ifdef CUSTOM_PATCH
      case SNC_PLIST:
        show_playlist();
        update_display();
        break;
#endif
      case SNC_AB:
        IS_AUTOCUE() || NO_LYRICS() ? add2file_list(true): set_ab_marker();
        update_display();
        break;
      case SNC_AB_MENU:
        ab_menu();
        update_display();
        break;
      case SNC_SAVE:
        save2file();
        update_display(); /* clear splash */
        break;
      case SNC_PLAY_PAUSE:
        (rb->audio_status() & AUDIO_STATUS_PAUSE) ?
          rb->audio_resume():rb->audio_pause();
        exec_func(1,update_status_display);
        break;
      case SNC_STOP_SEEK:
        ff_rew(newPos);
        newPos=NOT_INIT;
        break;
      case SNC_VISUAL_REW:
      case SNC_VISUAL_FF:
        if(newPos==NOT_INIT)
          newPos=AUDIO_ELAPSED();
        newPos+=(button==SNC_VISUAL_FF) ? 2000:-2000;
        newPos=update_time_display(newPos,true);
        break;
      case SNC_NEXT:
        (num_snc > 1 && rb->button_get_w_tmo(dbl_click_hz)!=BUTTON_NONE) ?
          ff_rew_snc(current_snc+(prefs.load_translation!=TR_OK ? 1:2)):
          rb->audio_next();
        break;
      case SNC_PREV:
        rb->button_get_w_tmo(dbl_click_hz)!=BUTTON_NONE?
            rb->audio_prev():
            auto_scroll ?
            ff_rew_snc(start_snc!=NOT_INIT?start_snc:0) : switch_mode(true);
        break;
      case SNC_QUIT:
        status=PLUGIN_OK;
        break;
      case SNC_OPEN_MENU: show_menu(); break;
      default:
        if(rb->default_event_handler(button)==SYS_USB_CONNECTED)
          status=PLUGIN_USB_CONNECTED;
        if((rb->audio_status() & AUDIO_STATUS_PLAY)==false)
          status=PLUGIN_OK;
        if((auto_scroll ? handle_button_scroll(button):
                          handle_button_edit(button))==ACTION_NONE)
        {
            v_scroll_artist_title(false);
            update();    /* only update if no button was pressed */
        }
        if(prefs.peakmeter &&
            (NO_LYRICS() ||
              (prefs.load_bmp == BMP_SOUTH
               && auto_scroll
               && bmp_width != NOT_INIT
              )
            )
          )
          peak_meter();
        update_time_display(AUDIO_ELAPSED(), false);
    }
  }
  /* clean up before quit */
  confirm_saving();
  if(file_list.num>0) file_list_menu();
  default_backlight(); /* restore backlight */
  set_repeat_mode(RMODE_CLEAR);
  if(prefs.save){
    prefs.save=0;
    save_settings(SETTINGS_FILE);
  }
#ifdef HAVE_LCD_COLOR
  /* restore color settings */
  rb->lcd_set_background(rb->global_settings->bg_color);
  rb->lcd_set_foreground(rb->global_settings->fg_color);
#endif
  /* consumes all button presses (button_clear_queue() doesn't really work */
  while(rb->get_action(CONTEXT_SNC,short_hz) != ACTION_NONE);
  return GO_TO_PREVIOUS;
}
