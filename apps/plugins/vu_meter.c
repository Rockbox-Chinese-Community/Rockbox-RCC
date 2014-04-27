/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 *
 * Copyright (C) 2003 Lee Pilgrim
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 **************************************************************************/
#include "plugin.h"
#include "fixedpoint.h"
#include "lib/playback_control.h"



/* variable button definitions */
#if CONFIG_KEYPAD == RECORDER_PAD
#define VUMETER_QUIT BUTTON_OFF
#define VUMETER_HELP BUTTON_ON
#define VUMETER_MENU BUTTON_F1
#define VUMETER_UP BUTTON_UP
#define VUMETER_DOWN BUTTON_DOWN
#define LABEL_HELP "ON"
#define LABEL_QUIT "OFF"
#define LABEL_MENU "F1"
#define LABEL_VOLUME "UP/DOWN"

#elif CONFIG_KEYPAD == ARCHOS_AV300_PAD
#define VUMETER_QUIT BUTTON_OFF
#define VUMETER_HELP BUTTON_ON
#define VUMETER_MENU BUTTON_F1
#define VUMETER_UP BUTTON_UP
#define VUMETER_DOWN BUTTON_DOWN
#define LABEL_HELP "ON"
#define LABEL_QUIT "OFF"
#define LABEL_MENU "F1"
#define LABEL_VOLUME "UP/DOWN"

#elif CONFIG_KEYPAD == ONDIO_PAD
#define VUMETER_QUIT BUTTON_OFF
#define VUMETER_HELP_PRE BUTTON_MENU
#define VUMETER_HELP (BUTTON_MENU | BUTTON_REL)
#define VUMETER_MENU_PRE BUTTON_MENU
#define VUMETER_MENU (BUTTON_MENU | BUTTON_REPEAT)
#define VUMETER_UP BUTTON_UP
#define VUMETER_DOWN BUTTON_DOWN
#define LABEL_HELP "MODE"
#define LABEL_QUIT "OFF"
#define LABEL_MENU "MODE.."
#define LABEL_VOLUME "UP/DOWN"

#elif (CONFIG_KEYPAD == IRIVER_H100_PAD) || \
      (CONFIG_KEYPAD == IRIVER_H300_PAD)
#define VUMETER_QUIT BUTTON_OFF
#define VUMETER_HELP BUTTON_ON
#define VUMETER_MENU BUTTON_SELECT
#define VUMETER_MENU2 BUTTON_MODE
#define VUMETER_UP BUTTON_UP
#define VUMETER_DOWN BUTTON_DOWN
#define LABEL_HELP "PLAY"
#define LABEL_QUIT "STOP"
#define LABEL_MENU "SELECT,MODE"
#define LABEL_VOLUME "UP/DOWN"

#define VUMETER_RC_QUIT BUTTON_RC_STOP

#elif (CONFIG_KEYPAD == IPOD_4G_PAD) || \
      (CONFIG_KEYPAD == IPOD_3G_PAD) || \
      (CONFIG_KEYPAD == IPOD_1G2G_PAD)
#define VUMETER_QUIT BUTTON_MENU
#define VUMETER_HELP BUTTON_PLAY
#define VUMETER_MENU BUTTON_SELECT
#define VUMETER_UP BUTTON_SCROLL_FWD
#define VUMETER_DOWN BUTTON_SCROLL_BACK
#define LABEL_HELP "PLAY"
#define LABEL_QUIT "MENU"
#define LABEL_MENU "SELECT"
#define LABEL_VOLUME "Wheel"

#elif (CONFIG_KEYPAD == GIGABEAT_PAD)
#define VUMETER_QUIT BUTTON_POWER
#define VUMETER_HELP BUTTON_A
#define VUMETER_MENU BUTTON_MENU
#define VUMETER_UP BUTTON_UP
#define VUMETER_DOWN BUTTON_DOWN
#define LABEL_HELP "A"
#define LABEL_QUIT "POWER"
#define LABEL_MENU "MENU"
#define LABEL_VOLUME "UP/DOWN"

#elif (CONFIG_KEYPAD == SANSA_E200_PAD)
#define VUMETER_QUIT BUTTON_POWER
#define VUMETER_HELP BUTTON_REC
#define VUMETER_MENU BUTTON_SELECT
#define VUMETER_UP BUTTON_SCROLL_FWD
#define VUMETER_DOWN BUTTON_SCROLL_BACK
#define LABEL_HELP "REC"
#define LABEL_QUIT "POWER"
#define LABEL_MENU "SELECT"
#define LABEL_VOLUME "Wheel"

#elif (CONFIG_KEYPAD == SANSA_FUZE_PAD)
#define VUMETER_QUIT        (BUTTON_HOME|BUTTON_REPEAT)
#define VUMETER_HELP        BUTTON_SELECT|BUTTON_REPEAT
#define VUMETER_MENU        BUTTON_SELECT|BUTTON_REL
#define VUMETER_UP          BUTTON_SCROLL_FWD
#define VUMETER_DOWN        BUTTON_SCROLL_BACK
#define LABEL_HELP          "Hold Select"
#define LABEL_QUIT          "HOME"
#define LABEL_MENU          "Select"
#define LABEL_VOLUME        "Scrollwheel"

#elif (CONFIG_KEYPAD == SANSA_C200_PAD)
#define VUMETER_QUIT BUTTON_POWER
#define VUMETER_HELP BUTTON_REC
#define VUMETER_MENU BUTTON_SELECT
#define VUMETER_UP BUTTON_VOL_UP
#define VUMETER_DOWN BUTTON_VOL_DOWN
#define LABEL_HELP "REC"
#define LABEL_QUIT "POWER"
#define LABEL_MENU "SELECT"
#define LABEL_VOLUME "VOL UP/DN"

#elif (CONFIG_KEYPAD == SANSA_CLIP_PAD)
#define VUMETER_QUIT BUTTON_POWER
#define VUMETER_HELP BUTTON_HOME
#define VUMETER_MENU BUTTON_SELECT
#define VUMETER_UP BUTTON_VOL_UP
#define VUMETER_DOWN BUTTON_VOL_DOWN
#define LABEL_HELP "HOME"
#define LABEL_QUIT "POWER"
#define LABEL_MENU "SELECT"
#define LABEL_VOLUME "VOL UP/DN"

#elif (CONFIG_KEYPAD == SANSA_M200_PAD)
#define VUMETER_QUIT BUTTON_POWER
#define VUMETER_HELP (BUTTON_SELECT | BUTTON_UP)
#define VUMETER_MENU (BUTTON_SELECT | BUTTON_REL)
#define VUMETER_UP BUTTON_VOL_UP
#define VUMETER_DOWN BUTTON_VOL_DOWN
#define LABEL_HELP "SELECT + UP"
#define LABEL_QUIT "POWER"
#define LABEL_MENU "SELECT"
#define LABEL_VOLUME "VOL UP/DN"

#elif CONFIG_KEYPAD == IAUDIO_X5M5_PAD
#define VUMETER_QUIT BUTTON_POWER
#define VUMETER_HELP BUTTON_PLAY
#define VUMETER_MENU BUTTON_SELECT
#define VUMETER_UP BUTTON_UP
#define VUMETER_DOWN BUTTON_DOWN
#define LABEL_HELP "PLAY"
#define LABEL_QUIT "POWER"
#define LABEL_MENU "SELECT"
#define LABEL_VOLUME "UP/DOWN"

#elif CONFIG_KEYPAD == IRIVER_H10_PAD
#define VUMETER_QUIT BUTTON_POWER
#define VUMETER_HELP BUTTON_PLAY
#define VUMETER_MENU BUTTON_REW
#define VUMETER_UP BUTTON_SCROLL_UP
#define VUMETER_DOWN BUTTON_SCROLL_DOWN
#define LABEL_HELP "PLAY"
#define LABEL_QUIT "POWER"
#define LABEL_MENU "REW"
#define LABEL_VOLUME "Scroller"

#elif (CONFIG_KEYPAD == GIGABEAT_S_PAD)
#define VUMETER_QUIT BUTTON_BACK
#define VUMETER_HELP BUTTON_NEXT
#define VUMETER_MENU BUTTON_MENU
#define VUMETER_UP BUTTON_UP
#define VUMETER_DOWN BUTTON_DOWN
#define LABEL_HELP "NEXT"
#define LABEL_QUIT "BACK"
#define LABEL_MENU "MENU"
#define LABEL_VOLUME "UP/DOWN"

#elif (CONFIG_KEYPAD == MROBE100_PAD)
#define VUMETER_QUIT BUTTON_POWER
#define VUMETER_HELP BUTTON_DISPLAY
#define VUMETER_MENU BUTTON_MENU
#define VUMETER_UP BUTTON_UP
#define VUMETER_DOWN BUTTON_DOWN
#define LABEL_HELP "DISPLAY"
#define LABEL_QUIT "POWER"
#define LABEL_MENU "MENU"
#define LABEL_VOLUME "UP/DOWN"

#elif CONFIG_KEYPAD == IAUDIO_M3_PAD
#define VUMETER_QUIT BUTTON_RC_REC
#define VUMETER_HELP BUTTON_RC_MODE
#define VUMETER_MENU BUTTON_RC_MENU
#define VUMETER_UP BUTTON_RC_VOL_UP
#define VUMETER_DOWN BUTTON_RC_VOL_DOWN
#define LABEL_HELP "MODE"
#define LABEL_QUIT "REC"
#define LABEL_MENU "MENU"
#define LABEL_VOLUME "VOL UP/DN"

#elif CONFIG_KEYPAD == COWON_D2_PAD
#define VUMETER_QUIT BUTTON_POWER
#define VUMETER_MENU BUTTON_MENU
#define LABEL_QUIT "POWER"
#define LABEL_MENU "MENU"

#elif CONFIG_KEYPAD == CREATIVEZVM_PAD
#define VUMETER_QUIT BUTTON_BACK
#define VUMETER_HELP BUTTON_SELECT
#define VUMETER_MENU BUTTON_MENU
#define VUMETER_UP BUTTON_UP
#define VUMETER_DOWN BUTTON_DOWN
#define LABEL_HELP "MIDDLE"
#define LABEL_QUIT "BACK"
#define LABEL_MENU "MENU"
#define LABEL_VOLUME "UP/DOWN"

#elif (CONFIG_KEYPAD == CREATIVE_ZENXFI3_PAD)
#define VUMETER_QUIT        BUTTON_POWER
#define VUMETER_HELP        BUTTON_MENU|BUTTON_REPEAT
#define VUMETER_MENU        BUTTON_MENU|BUTTON_REL
#define VUMETER_UP          BUTTON_VOL_UP
#define VUMETER_DOWN        BUTTON_VOL_DOWN
#define LABEL_HELP          "Hold Menu"
#define LABEL_QUIT          "Power"
#define LABEL_MENU          "Menu"
#define LABEL_VOLUME        "Volume +/-"

#elif CONFIG_KEYPAD == PHILIPS_HDD1630_PAD
#define VUMETER_QUIT BUTTON_POWER
#define VUMETER_HELP BUTTON_VIEW
#define VUMETER_MENU BUTTON_MENU
#define VUMETER_UP BUTTON_UP
#define VUMETER_DOWN BUTTON_DOWN
#define LABEL_HELP "VIEW"
#define LABEL_QUIT "POWER"
#define LABEL_MENU "MENU"
#define LABEL_VOLUME "UP/DOWN"

#elif CONFIG_KEYPAD == PHILIPS_HDD6330_PAD
#define VUMETER_QUIT BUTTON_POWER
#define VUMETER_HELP BUTTON_NEXT
#define VUMETER_MENU BUTTON_MENU
#define VUMETER_UP BUTTON_UP
#define VUMETER_DOWN BUTTON_DOWN
#define LABEL_HELP "NEXT"
#define LABEL_QUIT "QUIT"
#define LABEL_MENU "MENU"
#define LABEL_VOLUME "UP/DOWN"

#elif CONFIG_KEYPAD == PHILIPS_SA9200_PAD
#define VUMETER_QUIT BUTTON_POWER
#define VUMETER_HELP BUTTON_RIGHT
#define VUMETER_MENU BUTTON_MENU
#define VUMETER_UP BUTTON_UP
#define VUMETER_DOWN BUTTON_DOWN
#define LABEL_HELP "RIGHT"
#define LABEL_QUIT "POWER"
#define LABEL_MENU "MENU"
#define LABEL_VOLUME "UP/DOWN"

#elif CONFIG_KEYPAD == ONDAVX747_PAD
#define VUMETER_QUIT BUTTON_POWER
#define VUMETER_MENU BUTTON_MENU
#define LABEL_QUIT "POWER"
#define LABEL_MENU "MENU"

#elif CONFIG_KEYPAD == ONDAVX777_PAD
#define VUMETER_QUIT BUTTON_POWER
#define LABEL_QUIT "POWER"

#elif CONFIG_KEYPAD == MROBE500_PAD
#define VUMETER_QUIT BUTTON_POWER
#define LABEL_QUIT "POWER"

#elif CONFIG_KEYPAD == SAMSUNG_YH_PAD
#define VUMETER_QUIT       BUTTON_REC
#define VUMETER_HELP       BUTTON_PLAY
#define VUMETER_MENU       BUTTON_LEFT
#define VUMETER_UP         BUTTON_UP
#define VUMETER_DOWN       BUTTON_DOWN
#define LABEL_HELP "PLAY"
#define LABEL_QUIT "REC"
#define LABEL_MENU "LEFT"
#define LABEL_VOLUME "UP/DOWN"

#elif CONFIG_KEYPAD == PBELL_VIBE500_PAD
#define VUMETER_QUIT BUTTON_REC
#define VUMETER_HELP BUTTON_PLAY
#define VUMETER_MENU BUTTON_MENU
#define VUMETER_UP   BUTTON_UP
#define VUMETER_DOWN BUTTON_DOWN
#define LABEL_HELP "PLAY"
#define LABEL_QUIT "REC"
#define LABEL_MENU "MENU"
#define LABEL_VOLUME "UP/DOWN"

#elif CONFIG_KEYPAD == MPIO_HD200_PAD
#define VUMETER_QUIT (BUTTON_REC|BUTTON_PLAY)
#define VUMETER_HELP BUTTON_PLAY
#define VUMETER_MENU BUTTON_FUNC
#define VUMETER_UP BUTTON_VOL_UP
#define VUMETER_DOWN BUTTON_VOL_DOWN
#define LABEL_HELP "PLAY"
#define LABEL_QUIT "REC+PLAY"
#define LABEL_MENU "FUNC"
#define LABEL_VOLUME "UP/DOWN"

#elif CONFIG_KEYPAD == MPIO_HD300_PAD
#define VUMETER_QUIT (BUTTON_MENU|BUTTON_REPEAT)
#define VUMETER_HELP BUTTON_PLAY
#define VUMETER_MENU BUTTON_MENU
#define VUMETER_UP BUTTON_UP
#define VUMETER_DOWN BUTTON_DOWN
#define LABEL_HELP "PLAY"
#define LABEL_QUIT "LONG MENU"
#define LABEL_MENU "MENU"
#define LABEL_VOLUME "UP/DOWN"

#elif CONFIG_KEYPAD == SANSA_FUZEPLUS_PAD
#define VUMETER_QUIT BUTTON_BACK
#define VUMETER_HELP BUTTON_PLAYPAUSE
#define VUMETER_MENU (BUTTON_SELECT|BUTTON_REPEAT)
#define VUMETER_UP BUTTON_VOL_UP
#define VUMETER_DOWN BUTTON_VOL_DOWN
#define LABEL_HELP "PLAY"
#define LABEL_QUIT "BACK"
#define LABEL_MENU "Long SELECT"
#define LABEL_VOLUME "Vol+/Vol-"

#elif CONFIG_KEYPAD == SANSA_CONNECT_PAD
#define VUMETER_QUIT BUTTON_POWER
#define VUMETER_HELP BUTTON_NEXT
#define VUMETER_MENU BUTTON_PREV
#define VUMETER_UP BUTTON_UP
#define VUMETER_DOWN BUTTON_DOWN
#define LABEL_HELP "NEXT"
#define LABEL_QUIT "POWER"
#define LABEL_MENU "PREV"
#define LABEL_VOLUME "VOL+/VOL-"

#elif (CONFIG_KEYPAD == SAMSUNG_YPR0_PAD)
#define VUMETER_QUIT BUTTON_BACK
#define VUMETER_HELP BUTTON_USER
#define VUMETER_MENU BUTTON_MENU
#define VUMETER_UP   BUTTON_UP
#define VUMETER_DOWN BUTTON_DOWN
#define LABEL_HELP   "User"
#define LABEL_QUIT   "Back"
#define LABEL_MENU   "Menu"
#define LABEL_VOLUME "Up/Down"

#elif (CONFIG_KEYPAD == HM60X_PAD) || \
    (CONFIG_KEYPAD == HM801_PAD)
#define VUMETER_QUIT       BUTTON_POWER
#define VUMETER_HELP       BUTTON_RIGHT
#define VUMETER_MENU       BUTTON_LEFT
#define VUMETER_UP         BUTTON_UP
#define VUMETER_DOWN       BUTTON_DOWN
#define LABEL_HELP "RIGHT"
#define LABEL_QUIT "POWER"
#define LABEL_MENU "LEFT"
#define LABEL_VOLUME "UP/DOWN"

#elif (CONFIG_KEYPAD == ANDROID_PAD)

#else
#error No keymap defined!
#endif

#ifdef HAVE_TOUCHSCREEN
#ifndef VUMETER_QUIT
#ifdef ANDROID
#define VUMETER_QUIT      BUTTON_BACK
#else
#define VUMETER_QUIT      BUTTON_TOPLEFT
#endif
#define LABEL_QUIT        "TOPLEFT"
#endif
#ifndef VUMETER_HELP
#define VUMETER_HELP      BUTTON_CENTER
#define LABEL_HELP        "CENTRE"
#endif
#ifndef VUMETER_MENU
#define VUMETER_MENU      BUTTON_TOPRIGHT
#define LABEL_MENU        "TOPRIGHT"
#endif
#ifndef VUMETER_UP
#define VUMETER_UP        BUTTON_TOPMIDDLE
#endif
#ifndef VUMETER_DOWN
#define VUMETER_DOWN      BUTTON_BOTTOMMIDDLE
#endif
#ifndef LABEL_VOLUME
#define LABEL_VOLUME      "UP/DOWN"
#endif
#endif

#if defined(SIMULATOR) && (CONFIG_CODEC != SWCODEC)
#define mas_codec_readreg(x) rand()%MAX_PEAK
#endif

/* Defines x positions on a logarithmic (dBfs) scale. */
unsigned char analog_db_scale[LCD_WIDTH/2];

/* Define's y positions, to make the needle arch, like a real needle would. */
unsigned char y_values[LCD_WIDTH/2];

const unsigned char digital_db_scale[] =
{0,2,3,5,5,6,6,6,7,7,7,7,8,8,8,8,8,9,9,9,9,9,9,9,9,10,10,
10,10,10,10,10,10,10,10,10,11,11,11,11,11,11,11,11};

const unsigned char needle_cover[] =
{0x18, 0x1c, 0x1c, 0x1e, 0x1e, 0x1f, 0x1f, 0x1f, 0x1e, 0x1e, 0x1c, 0x1c, 0x18};

const unsigned char sound_speaker[] = {0x18,0x24,0x42,0xFF};
const unsigned char sound_low_level[] = {0x24,0x18};
const unsigned char sound_med_level[] = {0x42,0x3C};
const unsigned char sound_high_level[] = {0x81,0x7E};
const unsigned char sound_max_level[] = {0x0E,0xDF,0x0E};

const int half_width = LCD_WIDTH / 2;
const int quarter_width = LCD_WIDTH / 4;
const int half_height = LCD_HEIGHT / 2;

/* approx ratio of the previous hard coded values */
const int analog_mini_1 = (LCD_WIDTH / 2)*0.1;
const int analog_mini_2 = (LCD_WIDTH / 2)*0.25;
const int analog_mini_3 = (LCD_WIDTH / 2)*0.4;
const int analog_mini_4 = (LCD_WIDTH / 2)*0.75;

const int digital_block_width = LCD_WIDTH / 11;
const int digital_block_gap = (int)(LCD_WIDTH / 11) / 10;
/* ammount to lead in on left so 11x blocks are centered - is often 0 */
const int digital_lead = (LCD_WIDTH - (((int)(LCD_WIDTH / 11))*11) ) / 2;

const int digital_block_height = (LCD_HEIGHT - 54) / 2 ;

#define ANALOG 0 /* The two meter types */
#define DIGITAL 1

int left_needle_top_y;
int left_needle_top_x;
int last_left_needle_top_x;
int right_needle_top_y;
int right_needle_top_x;
int last_right_needle_top_x = LCD_WIDTH / 2;

int num_left_leds;
int num_right_leds;
int last_num_left_leds;
int last_num_right_leds;

int i;
#ifdef HAVE_LCD_COLOR
int screen_foreground;
#endif

#define MAX_PEAK 0x8000

/* gap at the top for left/right etc */
#define NEEDLE_TOP 25

/* pow(M_E, 5) * 65536 */
#define E_POW_5 9726404

struct saved_settings {
    int meter_type;
    bool analog_use_db_scale;
    bool digital_use_db_scale;
    bool analog_minimeters;
    bool digital_minimeters;
    int analog_decay;
    int digital_decay; 
} vumeter_settings;

static void reset_settings(void) {
    vumeter_settings.meter_type=ANALOG;
    vumeter_settings.analog_use_db_scale=true;
    vumeter_settings.digital_use_db_scale=true;
    vumeter_settings.analog_minimeters=true;
    vumeter_settings.digital_minimeters=false;
    vumeter_settings.analog_decay=3;
    vumeter_settings.digital_decay=0; 
}

static void calc_scales(void)
{
    unsigned int fx_log_factor = E_POW_5/half_width;
    unsigned int y,z;

    long j;
    long k;
    int nh = LCD_HEIGHT - NEEDLE_TOP;
    long nh2 = nh*nh;

    for (i=1; i <= half_width; i++)
    {
        /* analog scale */
        y = (half_width/5)*fp16_log(i*fx_log_factor);

        /* better way of checking for negative values? */
        z = y>>16;
        if (z > LCD_WIDTH)
            z = 0;

        analog_db_scale[i-1] = z;
        /* play nice */
        rb->yield();

        /* y values (analog needle co-ords) */
        j = i - (int)(half_width/2);
        k = nh2 - ( j * j );

        /* fsqrt+1 seems to give a closer approximation */
        y_values[i-1] = LCD_HEIGHT - (fp_sqrt(k, 16)>>8) - 1;
        rb->yield();
    }
}

static void load_settings(void) {
    int fp = rb->open(PLUGIN_DEMOS_DATA_DIR "/.vu_meter", O_RDONLY);
    if(fp>=0) {
            rb->read(fp, &vumeter_settings, sizeof(struct saved_settings));
            rb->close(fp);
    }
    else {
        reset_settings();
        rb->splash(HZ, "Press " LABEL_HELP " for help");
    }
}

static void save_settings(void) {
    int fp = rb->creat(PLUGIN_DEMOS_DATA_DIR "/.vu_meter", 0666);
    if(fp >= 0) {
        rb->write (fp, &vumeter_settings, sizeof(struct saved_settings));
        rb->close(fp);
    }
}

static void change_volume(int delta) {
    int minvol = rb->sound_min(SOUND_VOLUME);
    int maxvol = rb->sound_max(SOUND_VOLUME);
    int vol = rb->global_settings->volume + delta;

    if (vol > maxvol) vol = maxvol;
    else if (vol < minvol) vol = minvol;
    if (vol != rb->global_settings->volume) {
        rb->sound_set(SOUND_VOLUME, vol);
        rb->global_settings->volume = vol;
        rb->lcd_putsxyf(0,0, "%d", vol);
        rb->lcd_update();
        rb->sleep(HZ/12);
    }
}

static bool vu_meter_menu(void)
{
    int selection;
    bool menu_quit = false;
    bool exit = false;
    
    MENUITEM_STRINGLIST(menu,"VU指示器菜单(VU Meter Menu)",NULL,"仪表类型(Meter Type)","标度(Scale)",
                        "Mini指示器(Minimeters)","衰变速度(Decay Speed)","后台播放控置(Playback Control)",
                        "退出(Quit)");
    
    static const struct opt_items meter_type_option[2] = {
        { "模拟(Analog)", -1 },
        { "数字(Digital)", -1 },
    };

    static const struct opt_items decay_speed_option[7] = {
        { "无衰变(No Decay)", -1 },
        { "非常快(Very Fast)", -1 },
        { "快(Fast)", -1 },
        { "中(Medium)", -1 },
        { "中慢(Medium-Slow)", -1 },
        { "慢(Slow)", -1 },
        { "非常慢(Very Slow)", -1 },
    };

    while (!menu_quit) {
        switch(rb->do_menu(&menu, &selection, NULL, false))
        {
            case 0:
                rb->set_option("仪表类型(Meter Type)", &vumeter_settings.meter_type, INT,
                               meter_type_option, 2, NULL);
                break;
                
            case 1:
                if(vumeter_settings.meter_type==ANALOG)
                {
                    rb->set_bool_options("标度(Scale)", &vumeter_settings.analog_use_db_scale,
                                         "对数刻度(dBfs)", -1, "线性(Linear)", -1, NULL);
                }
                else
                {
                    rb->set_bool_options("标度(Scale)", &vumeter_settings.digital_use_db_scale,
                                         "对数刻度(dBfs)", -1, "线性(Linear)", -1, NULL);
                }
                break;
                
            case 2:
                if(vumeter_settings.meter_type==ANALOG)
                {
                    rb->set_bool("启用Mini指示器(Enable Minimeters)",
                                 &vumeter_settings.analog_minimeters);
                }
                else
                {
                    rb->set_bool("启用Mini指示器(Enable Minimeters)",
                                 &vumeter_settings.digital_minimeters);
                }
                break;
                
            case 3:
                if(vumeter_settings.meter_type==ANALOG)
                {
                    rb->set_option("衰变速度(Decay Speed)", &vumeter_settings.analog_decay, INT, 
                               decay_speed_option, 7, NULL);
                }
                else
                {
                    rb->set_option("衰变速度(Decay Speed)", &vumeter_settings.digital_decay, INT, 
                               decay_speed_option, 7, NULL);
                }
                break;

            case 4:
                playback_control(NULL);
                break;

            case 5:
                exit = true;
                /* fall through to exit the menu */
            default:
                menu_quit = true;
                break;
        }
    }
    /* the menu uses the userfont, set it back to sysfont */
    rb->lcd_setfont(FONT_UI);
    return exit;
}

static void draw_analog_minimeters(void) {
    rb->lcd_mono_bitmap(sound_speaker, quarter_width-28, 12, 4, 8);
    rb->lcd_set_drawmode(DRMODE_FG);
    if(analog_mini_1<left_needle_top_x)
        rb->lcd_mono_bitmap(sound_low_level, quarter_width-23, 12, 2, 8);
    if(analog_mini_2<left_needle_top_x)
        rb->lcd_mono_bitmap(sound_med_level, quarter_width-21, 12, 2, 8);
    if(analog_mini_3<left_needle_top_x)
        rb->lcd_mono_bitmap(sound_high_level, quarter_width-19, 12, 2, 8);
    if(analog_mini_4<left_needle_top_x)
        rb->lcd_mono_bitmap(sound_max_level, quarter_width-16, 12, 3, 8);

    rb->lcd_set_drawmode(DRMODE_SOLID);
    rb->lcd_mono_bitmap(sound_speaker, quarter_width+half_width-30, 12, 4, 8);
    rb->lcd_set_drawmode(DRMODE_FG);
    if(analog_mini_1<(right_needle_top_x-half_width))
        rb->lcd_mono_bitmap(sound_low_level, quarter_width+half_width-25, 12, 2, 8);
    if(analog_mini_2<(right_needle_top_x-half_width))
        rb->lcd_mono_bitmap(sound_med_level, quarter_width+half_width-23, 12, 2, 8);
    if(analog_mini_3<(right_needle_top_x-half_width))
        rb->lcd_mono_bitmap(sound_high_level, quarter_width+half_width-21, 12, 2, 8);
    if(analog_mini_4<(right_needle_top_x-half_width))
        rb->lcd_mono_bitmap(sound_max_level, quarter_width+half_width-18, 12, 3, 8);
    rb->lcd_set_drawmode(DRMODE_SOLID);
}

static void draw_digital_minimeters(void) {
#ifdef HAVE_LCD_COLOR
    rb->lcd_set_foreground(LCD_RGBPACK(255, 255 - 23 * num_left_leds, 0));
#endif
    rb->lcd_mono_bitmap(sound_speaker, 34, half_height-8, 4, 8);
    rb->lcd_set_drawmode(DRMODE_FG);
    if(1<num_left_leds)
        rb->lcd_mono_bitmap(sound_low_level, 39, half_height-8, 2, 8);
    if(2<num_left_leds)
        rb->lcd_mono_bitmap(sound_med_level, 41, half_height-8, 2, 8);
    if(5<num_left_leds)
        rb->lcd_mono_bitmap(sound_high_level, 43, half_height-8, 2, 8);
    if(8<num_left_leds)
        rb->lcd_mono_bitmap(sound_max_level, 46, half_height-8, 3, 8);

#ifdef HAVE_LCD_COLOR
    rb->lcd_set_foreground(LCD_RGBPACK(255, 255 - 23 * num_right_leds, 0));
#endif
    rb->lcd_set_drawmode(DRMODE_SOLID);
    rb->lcd_mono_bitmap(sound_speaker, 34, half_height+8, 4, 8);
    rb->lcd_set_drawmode(DRMODE_FG);
    if(1<(num_right_leds))
        rb->lcd_mono_bitmap(sound_low_level, 39, half_height+8, 2, 8);
    if(2<(num_right_leds))
        rb->lcd_mono_bitmap(sound_med_level, 41, half_height+8, 2, 8);
    if(5<(num_right_leds))
        rb->lcd_mono_bitmap(sound_high_level, 43, half_height+8, 2, 8);
    if(8<(num_right_leds))
        rb->lcd_mono_bitmap(sound_max_level, 46, half_height+8, 3, 8);
    rb->lcd_set_drawmode(DRMODE_SOLID);
    
#ifdef HAVE_LCD_COLOR
    rb->lcd_set_foreground(screen_foreground);
#endif
}

static void analog_meter(void) {

#if (CONFIG_CODEC == MAS3587F) || (CONFIG_CODEC == MAS3539F)
    int left_peak = rb->mas_codec_readreg(0xC);
    int right_peak = rb->mas_codec_readreg(0xD);
#elif (CONFIG_CODEC == SWCODEC)
    static struct pcm_peaks peaks;
    rb->mixer_channel_calculate_peaks(PCM_MIXER_CHAN_PLAYBACK,
                                      &peaks);
    #define left_peak peaks.left
    #define right_peak peaks.right
#endif

    if(vumeter_settings.analog_use_db_scale) {
        left_needle_top_x = analog_db_scale[left_peak * half_width / MAX_PEAK];
        right_needle_top_x = analog_db_scale[right_peak * half_width / MAX_PEAK] + half_width;
    }
    else {
        left_needle_top_x = left_peak * half_width / MAX_PEAK;
        right_needle_top_x = right_peak * half_width / MAX_PEAK + half_width;
    }

    /* Makes a decay on the needle */
    left_needle_top_x = (left_needle_top_x+last_left_needle_top_x*vumeter_settings.analog_decay)
                        /(vumeter_settings.analog_decay+1);
    right_needle_top_x = (right_needle_top_x+last_right_needle_top_x*vumeter_settings.analog_decay)
                         /(vumeter_settings.analog_decay+1);

    last_left_needle_top_x = left_needle_top_x;
    last_right_needle_top_x = right_needle_top_x;

    left_needle_top_y = y_values[left_needle_top_x];
    right_needle_top_y = y_values[right_needle_top_x-half_width];

    /* Needles */
    rb->lcd_drawline(quarter_width, LCD_HEIGHT-1, left_needle_top_x, left_needle_top_y);
    rb->lcd_drawline((quarter_width+half_width), LCD_HEIGHT-1, right_needle_top_x, right_needle_top_y);

    if(vumeter_settings.analog_minimeters)
        draw_analog_minimeters();

    /* Needle covers */
    rb->lcd_set_drawmode(DRMODE_FG);
    rb->lcd_mono_bitmap(needle_cover, quarter_width-6, LCD_HEIGHT-5, 13, 5);
    rb->lcd_mono_bitmap(needle_cover, half_width+quarter_width-6, LCD_HEIGHT-5, 13, 5);
    rb->lcd_set_drawmode(DRMODE_SOLID);

    /* Show Left/Right */
    rb->lcd_putsxy(quarter_width-12, 12, "Left");
    rb->lcd_putsxy(half_width+quarter_width-12, 12, "Right");

    /* Line above/below  the Left/Right text */
    rb->lcd_hline(0,LCD_WIDTH-1,9);
    rb->lcd_hline(0,LCD_WIDTH-1,21);

    for(i=0; i<half_width; i++) {
        rb->lcd_drawpixel(i, (y_values[i])-2);
        rb->lcd_drawpixel(i+half_width, (y_values[i])-2);
    }
}

static void digital_meter(void) {
#if (CONFIG_CODEC == MAS3587F) || (CONFIG_CODEC == MAS3539F)
    int left_peak = rb->mas_codec_readreg(0xC);
    int right_peak = rb->mas_codec_readreg(0xD);
#elif (CONFIG_CODEC == SWCODEC)
    static struct pcm_peaks peaks;
    rb->mixer_channel_calculate_peaks(PCM_MIXER_CHAN_PLAYBACK,
                                      &peaks);
    #define left_peak peaks.left
    #define right_peak peaks.right
#endif

    if(vumeter_settings.digital_use_db_scale) {
        num_left_leds = digital_db_scale[left_peak * 44 / MAX_PEAK];
        num_right_leds = digital_db_scale[right_peak * 44 / MAX_PEAK];
    }
    else {
        num_left_leds = left_peak * 11 / MAX_PEAK;
        num_right_leds = right_peak * 11 / MAX_PEAK;
    }

    num_left_leds = (num_left_leds+last_num_left_leds*vumeter_settings.digital_decay)
                    /(vumeter_settings.digital_decay+1);
    num_right_leds = (num_right_leds+last_num_right_leds*vumeter_settings.digital_decay)
                     /(vumeter_settings.digital_decay+1);

    last_num_left_leds = num_left_leds;
    last_num_right_leds = num_right_leds;

    rb->lcd_set_drawmode(DRMODE_FG);
    /* LEDS */
    for(i=0; i<num_left_leds; i++) {
#ifdef HAVE_LCD_COLOR
        rb->lcd_set_foreground(LCD_RGBPACK(255, 255 - 23 * i, 0));
#endif
        rb->lcd_fillrect((digital_lead + (i*digital_block_width)),
            14, digital_block_width - digital_block_gap, digital_block_height);
    }

    for(i=0; i<num_right_leds; i++) {
#ifdef HAVE_LCD_COLOR
        rb->lcd_set_foreground(LCD_RGBPACK(255, 255 - 23 * i, 0));
#endif
        rb->lcd_fillrect((digital_lead + (i*digital_block_width)),
            (half_height + 20), digital_block_width - digital_block_gap, 
            digital_block_height);
    }
    
#ifdef HAVE_LCD_COLOR
    rb->lcd_set_foreground(screen_foreground);
#endif
    rb->lcd_set_drawmode(DRMODE_SOLID);

    if(vumeter_settings.digital_minimeters)
        draw_digital_minimeters();

    /* Lines above/below where the LEDS are */
    rb->lcd_hline(0,LCD_WIDTH-1,12);
    rb->lcd_hline(0,LCD_WIDTH-1,half_height-12);

    rb->lcd_hline(0,LCD_WIDTH-1,half_height+18);
    rb->lcd_hline(0,LCD_WIDTH-1,LCD_HEIGHT-6);

    /* Show Left/Right */
    rb->lcd_putsxy(2, half_height-8, "Left");
    rb->lcd_putsxy(2, half_height+8, "Right");

    /* Line in the middle */
    rb->lcd_hline(0,LCD_WIDTH-1,half_height+3);
}

enum plugin_status plugin_start(const void* parameter) {
    int button;
#if defined(VUMETER_HELP_PRE) || defined(VUMETER_MENU_PRE)
    int lastbutton = BUTTON_NONE;
#endif

    (void) parameter;

    calc_scales();

    load_settings();
    rb->lcd_setfont(FONT_SYSFIXED);
#ifdef HAVE_LCD_COLOR
    screen_foreground = rb->lcd_get_foreground();
#endif

    while (1)
    {
        rb->lcd_clear_display();

        rb->lcd_putsxy(half_width-23, 0, "VU Meter");

        if(vumeter_settings.meter_type==ANALOG)
            analog_meter();
        else
            digital_meter();

        rb->lcd_update();

        button = rb->button_get_w_tmo(1);
        switch (button)
        {
#ifdef VUMETER_RC_QUIT
            case VUMETER_RC_QUIT:
#endif
            case VUMETER_QUIT:
                save_settings();
                return PLUGIN_OK;
                break;

            case VUMETER_HELP:
#ifdef VUMETER_HELP_PRE
                if (lastbutton != VUMETER_HELP_PRE)
                    break;
#endif
                rb->lcd_clear_display();
                rb->lcd_puts(0, 0, LABEL_QUIT ": Exit");
                rb->lcd_puts(0, 1, LABEL_MENU ": Settings");
                rb->lcd_puts(0, 2, LABEL_VOLUME ": Volume");
                rb->lcd_update();
                rb->sleep(HZ*3);
                break;

            case VUMETER_MENU:

#ifdef VUMETER_MENU2
            case VUMETER_MENU2:
#endif

#ifdef VUMETER_MENU_PRE
                if (lastbutton != VUMETER_MENU_PRE)
                    break;
#endif
                if(vu_meter_menu())
                    return PLUGIN_OK;
                break;

            case VUMETER_UP:
            case VUMETER_UP | BUTTON_REPEAT:
                change_volume(1);
                break;

            case VUMETER_DOWN:
            case VUMETER_DOWN | BUTTON_REPEAT:
                change_volume(-1);
                break;

            default:
                if(rb->default_event_handler(button) == SYS_USB_CONNECTED)
                    return PLUGIN_USB_CONNECTED;
                break;
        }
#if defined(VUMETER_HELP_PRE) || defined(VUMETER_MENU_PRE)
        if (button != BUTTON_NONE)
            lastbutton = button;
#endif
    }
}
