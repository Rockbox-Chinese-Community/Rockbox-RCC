/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2002 Kjell Ericson
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/
#include "plugin.h"
#include "lib/playback_control.h"



/* variable button definitions */
#if CONFIG_KEYPAD == RECORDER_PAD
#define CHC_QUIT BUTTON_OFF
#define CHC_STARTSTOP BUTTON_PLAY
#define CHC_RESET BUTTON_LEFT
#define CHC_MENU BUTTON_F1
#define CHC_SETTINGS_INC BUTTON_UP
#define CHC_SETTINGS_DEC BUTTON_DOWN
#define CHC_SETTINGS_OK BUTTON_PLAY
#define CHC_SETTINGS_OK2 BUTTON_LEFT
#define CHC_SETTINGS_CANCEL BUTTON_OFF

#elif CONFIG_KEYPAD == ARCHOS_AV300_PAD
#define CHC_QUIT BUTTON_OFF
#define CHC_STARTSTOP BUTTON_SELECT
#define CHC_RESET BUTTON_LEFT
#define CHC_MENU BUTTON_F1
#define CHC_SETTINGS_INC BUTTON_UP
#define CHC_SETTINGS_DEC BUTTON_DOWN
#define CHC_SETTINGS_OK BUTTON_SELECT
#define CHC_SETTINGS_OK2 BUTTON_LEFT
#define CHC_SETTINGS_CANCEL BUTTON_OFF

#elif CONFIG_KEYPAD == ONDIO_PAD
#define CHC_QUIT BUTTON_OFF
#define CHC_STARTSTOP BUTTON_RIGHT
#define CHC_RESET BUTTON_LEFT
#define CHC_MENU BUTTON_MENU
#define CHC_SETTINGS_INC BUTTON_UP
#define CHC_SETTINGS_DEC BUTTON_DOWN
#define CHC_SETTINGS_OK BUTTON_RIGHT
#define CHC_SETTINGS_OK2 BUTTON_LEFT
#define CHC_SETTINGS_CANCEL BUTTON_MENU

#elif CONFIG_KEYPAD == PLAYER_PAD
#define CHC_QUIT BUTTON_ON
#define CHC_STARTSTOP BUTTON_PLAY
#define CHC_RESET BUTTON_STOP
#define CHC_MENU BUTTON_MENU
#define CHC_SETTINGS_INC BUTTON_RIGHT
#define CHC_SETTINGS_DEC BUTTON_LEFT
#define CHC_SETTINGS_OK BUTTON_PLAY
#define CHC_SETTINGS_CANCEL BUTTON_STOP
#define CHC_SETTINGS_CANCEL2 BUTTON_MENU

#elif (CONFIG_KEYPAD == IRIVER_H100_PAD) || \
      (CONFIG_KEYPAD == IRIVER_H300_PAD)
#define CHC_QUIT BUTTON_SELECT
#define CHC_STARTSTOP BUTTON_ON
#define CHC_RESET BUTTON_OFF
#define CHC_MENU BUTTON_REC
#define CHC_SETTINGS_INC BUTTON_RIGHT
#define CHC_SETTINGS_DEC BUTTON_LEFT
#define CHC_SETTINGS_OK BUTTON_ON
#define CHC_SETTINGS_CANCEL BUTTON_OFF
#define CHC_SETTINGS_CANCEL2 BUTTON_REC

#elif (CONFIG_KEYPAD == IPOD_4G_PAD) || \
      (CONFIG_KEYPAD == IPOD_3G_PAD) || \
      (CONFIG_KEYPAD == IPOD_1G2G_PAD)
#define CHC_QUIT BUTTON_PLAY
#define CHC_STARTSTOP BUTTON_SELECT
#define CHC_RESET BUTTON_LEFT
#define CHC_MENU BUTTON_MENU
#define CHC_SETTINGS_INC BUTTON_SCROLL_FWD
#define CHC_SETTINGS_DEC BUTTON_SCROLL_BACK
#define CHC_SETTINGS_OK BUTTON_SELECT
#define CHC_SETTINGS_CANCEL BUTTON_MENU

#elif CONFIG_KEYPAD == IRIVER_IFP7XX_PAD
#define CHC_QUIT BUTTON_PLAY
#define CHC_STARTSTOP BUTTON_MODE
#define CHC_RESET BUTTON_EQ
#define CHC_MENU BUTTON_SELECT
#define CHC_SETTINGS_INC BUTTON_RIGHT
#define CHC_SETTINGS_DEC BUTTON_LEFT
#define CHC_SETTINGS_OK BUTTON_SELECT
#define CHC_SETTINGS_CANCEL BUTTON_PLAY

#elif CONFIG_KEYPAD == IAUDIO_X5M5_PAD
#define CHC_QUIT BUTTON_REC
#define CHC_STARTSTOP BUTTON_PLAY
#define CHC_RESET BUTTON_POWER
#define CHC_MENU BUTTON_SELECT
#define CHC_SETTINGS_INC BUTTON_RIGHT
#define CHC_SETTINGS_DEC BUTTON_LEFT
#define CHC_SETTINGS_OK BUTTON_SELECT
#define CHC_SETTINGS_CANCEL BUTTON_REC

#elif CONFIG_KEYPAD == GIGABEAT_PAD
#define CHC_QUIT BUTTON_POWER
#define CHC_STARTSTOP BUTTON_SELECT
#define CHC_RESET BUTTON_A
#define CHC_MENU BUTTON_MENU
#define CHC_SETTINGS_INC BUTTON_UP
#define CHC_SETTINGS_DEC BUTTON_DOWN
#define CHC_SETTINGS_OK BUTTON_SELECT
#define CHC_SETTINGS_CANCEL BUTTON_POWER

#elif (CONFIG_KEYPAD == SANSA_E200_PAD) || \
      (CONFIG_KEYPAD == SANSA_C200_PAD) || \
      (CONFIG_KEYPAD == SANSA_CLIP_PAD) || \
      (CONFIG_KEYPAD == SANSA_M200_PAD)
#define CHC_QUIT BUTTON_POWER
#define CHC_STARTSTOP BUTTON_SELECT
#define CHC_RESET BUTTON_DOWN
#define CHC_MENU BUTTON_UP
#define CHC_SETTINGS_INC BUTTON_RIGHT
#define CHC_SETTINGS_DEC BUTTON_LEFT
#define CHC_SETTINGS_OK BUTTON_SELECT
#define CHC_SETTINGS_CANCEL BUTTON_POWER

#elif (CONFIG_KEYPAD == SANSA_FUZE_PAD)
#define CHC_QUIT BUTTON_POWER
#define CHC_STARTSTOP BUTTON_SELECT
#define CHC_RESET BUTTON_DOWN
#define CHC_MENU BUTTON_UP
#define CHC_SETTINGS_INC BUTTON_RIGHT
#define CHC_SETTINGS_DEC BUTTON_LEFT
#define CHC_SETTINGS_OK BUTTON_SELECT
#define CHC_SETTINGS_CANCEL (BUTTON_HOME|BUTTON_REPEAT)

#elif CONFIG_KEYPAD == IRIVER_H10_PAD
#define CHC_QUIT BUTTON_POWER
#define CHC_STARTSTOP BUTTON_PLAY
#define CHC_RESET BUTTON_FF
#define CHC_MENU BUTTON_REW
#define CHC_SETTINGS_INC BUTTON_RIGHT
#define CHC_SETTINGS_DEC BUTTON_LEFT
#define CHC_SETTINGS_OK BUTTON_PLAY
#define CHC_SETTINGS_CANCEL BUTTON_POWER

#elif CONFIG_KEYPAD == MROBE500_PAD
#define CHC_QUIT BUTTON_POWER
#define CHC_STARTSTOP BUTTON_RC_PLAY
#define CHC_RESET BUTTON_RC_HEART
#define CHC_MENU BUTTON_RC_MODE
#define CHC_SETTINGS_INC BUTTON_RC_VOL_UP
#define CHC_SETTINGS_DEC BUTTON_RC_VOL_DOWN
#define CHC_SETTINGS_OK BUTTON_RC_PLAY
#define CHC_SETTINGS_CANCEL BUTTON_POWER

#elif CONFIG_KEYPAD == GIGABEAT_S_PAD
#define CHC_QUIT BUTTON_BACK
#define CHC_STARTSTOP BUTTON_PLAY
#define CHC_RESET BUTTON_PREV
#define CHC_MENU BUTTON_MENU
#define CHC_SETTINGS_INC BUTTON_UP
#define CHC_SETTINGS_DEC BUTTON_DOWN
#define CHC_SETTINGS_OK BUTTON_SELECT
#define CHC_SETTINGS_CANCEL BUTTON_BACK

#elif CONFIG_KEYPAD == MROBE100_PAD
#define CHC_QUIT BUTTON_POWER
#define CHC_STARTSTOP BUTTON_SELECT
#define CHC_RESET BUTTON_DISPLAY
#define CHC_MENU BUTTON_MENU
#define CHC_SETTINGS_INC BUTTON_UP
#define CHC_SETTINGS_DEC BUTTON_DOWN
#define CHC_SETTINGS_OK BUTTON_SELECT
#define CHC_SETTINGS_CANCEL BUTTON_POWER

#elif CONFIG_KEYPAD == IAUDIO_M3_PAD
#define CHC_QUIT BUTTON_RC_REC
#define CHC_STARTSTOP BUTTON_RC_PLAY
#define CHC_RESET BUTTON_RC_REW
#define CHC_MENU BUTTON_RC_MENU
#define CHC_SETTINGS_INC BUTTON_RC_VOL_UP
#define CHC_SETTINGS_DEC BUTTON_RC_VOL_DOWN
#define CHC_SETTINGS_OK BUTTON_RC_PLAY
#define CHC_SETTINGS_CANCEL BUTTON_RC_REC

#elif CONFIG_KEYPAD == COWON_D2_PAD
#define CHC_QUIT            BUTTON_POWER
#define CHC_RESET          (BUTTON_CENTER|BUTTON_MENU)
#define CHC_MENU            BUTTON_MENU
#define CHC_SETTINGS_INC    BUTTON_PLUS
#define CHC_SETTINGS_DEC    BUTTON_MINUS
#define CHC_SETTINGS_CANCEL BUTTON_POWER

#elif CONFIG_KEYPAD == IAUDIO67_PAD
#define CHC_QUIT BUTTON_POWER
#define CHC_STARTSTOP BUTTON_PLAY
#define CHC_RESET BUTTON_LEFT
#define CHC_MENU BUTTON_MENU
#define CHC_SETTINGS_INC BUTTON_VOLUP
#define CHC_SETTINGS_DEC BUTTON_VOLDOWN
#define CHC_SETTINGS_OK BUTTON_PLAY
#define CHC_SETTINGS_OK2 BUTTON_LEFT
#define CHC_SETTINGS_CANCEL BUTTON_POWER

#elif CONFIG_KEYPAD == CREATIVEZVM_PAD
#define CHC_QUIT BUTTON_BACK
#define CHC_STARTSTOP BUTTON_PLAY
#define CHC_RESET BUTTON_CUSTOM
#define CHC_MENU BUTTON_MENU
#define CHC_SETTINGS_INC BUTTON_UP
#define CHC_SETTINGS_DEC BUTTON_DOWN
#define CHC_SETTINGS_OK BUTTON_SELECT
#define CHC_SETTINGS_CANCEL BUTTON_BACK

#elif CONFIG_KEYPAD == CREATIVE_ZENXFI3_PAD
#define CHC_QUIT BUTTON_POWER
#define CHC_STARTSTOP BUTTON_PLAY
#define CHC_RESET (BUTTON_BACK|BUTTON_REPEAT)
#define CHC_MENU BUTTON_MENU
#define CHC_SETTINGS_INC BUTTON_UP
#define CHC_SETTINGS_DEC BUTTON_DOWN
#define CHC_SETTINGS_OK BUTTON_PLAY
#define CHC_SETTINGS_CANCEL BUTTON_POWER

#elif CONFIG_KEYPAD == PHILIPS_HDD1630_PAD
#define CHC_QUIT BUTTON_POWER
#define CHC_STARTSTOP BUTTON_SELECT
#define CHC_RESET BUTTON_VIEW
#define CHC_MENU BUTTON_MENU
#define CHC_SETTINGS_INC BUTTON_RIGHT
#define CHC_SETTINGS_DEC BUTTON_LEFT
#define CHC_SETTINGS_OK BUTTON_SELECT
#define CHC_SETTINGS_CANCEL BUTTON_POWER

#elif CONFIG_KEYPAD == PHILIPS_HDD6330_PAD
#define CHC_QUIT BUTTON_POWER
#define CHC_STARTSTOP BUTTON_PLAY
#define CHC_RESET BUTTON_RIGHT
#define CHC_MENU BUTTON_MENU
#define CHC_SETTINGS_INC BUTTON_NEXT
#define CHC_SETTINGS_DEC BUTTON_PREV
#define CHC_SETTINGS_OK BUTTON_PLAY
#define CHC_SETTINGS_CANCEL BUTTON_POWER

#elif CONFIG_KEYPAD == PHILIPS_SA9200_PAD
#define CHC_QUIT BUTTON_POWER
#define CHC_STARTSTOP BUTTON_PLAY
#define CHC_RESET BUTTON_RIGHT
#define CHC_MENU BUTTON_MENU
#define CHC_SETTINGS_INC BUTTON_NEXT
#define CHC_SETTINGS_DEC BUTTON_PREV
#define CHC_SETTINGS_OK BUTTON_PLAY
#define CHC_SETTINGS_CANCEL BUTTON_POWER

#elif CONFIG_KEYPAD == ONDAVX747_PAD
#define CHC_QUIT            BUTTON_POWER
#define CHC_RESET          (BUTTON_VOL_UP|BUTTON_VOL_DOWN)
#define CHC_MENU            BUTTON_MENU
#define CHC_SETTINGS_INC    BUTTON_VOL_UP
#define CHC_SETTINGS_DEC    BUTTON_VOL_DOWN
#define CHC_SETTINGS_CANCEL BUTTON_POWER

#elif CONFIG_KEYPAD == ONDAVX777_PAD
#define CHC_QUIT            BUTTON_POWER
#define CHC_SETTINGS_CANCEL BUTTON_POWER

#elif CONFIG_KEYPAD == SAMSUNG_YH_PAD
#define CHC_QUIT             BUTTON_REC
#define CHC_STARTSTOP        BUTTON_PLAY
#define CHC_RESET            BUTTON_FFWD
#define CHC_MENU             BUTTON_LEFT
#define CHC_SETTINGS_INC     BUTTON_UP
#define CHC_SETTINGS_DEC     BUTTON_DOWN
#define CHC_SETTINGS_OK      BUTTON_PLAY
#define CHC_SETTINGS_CANCEL  BUTTON_REW

#elif CONFIG_KEYPAD == PBELL_VIBE500_PAD
#define CHC_QUIT             BUTTON_REC
#define CHC_STARTSTOP        BUTTON_PLAY
#define CHC_RESET            BUTTON_PREV
#define CHC_MENU             BUTTON_MENU
#define CHC_SETTINGS_INC     BUTTON_UP
#define CHC_SETTINGS_DEC     BUTTON_DOWN
#define CHC_SETTINGS_OK      BUTTON_OK
#define CHC_SETTINGS_CANCEL  BUTTON_CANCEL

#elif CONFIG_KEYPAD == MPIO_HD200_PAD
#define CHC_QUIT            (BUTTON_REC|BUTTON_PLAY)
#define CHC_STARTSTOP       BUTTON_PLAY
#define CHC_RESET           BUTTON_REW
#define CHC_MENU            (BUTTON_FUNC|BUTTON_REPEAT)
#define CHC_SETTINGS_INC    BUTTON_VOL_UP
#define CHC_SETTINGS_DEC    BUTTON_VOL_DOWN
#define CHC_SETTINGS_OK     BUTTON_FUNC
#define CHC_SETTINGS_CANCEL BUTTON_REC

#elif CONFIG_KEYPAD == MPIO_HD300_PAD
#define CHC_QUIT            (BUTTON_MENU|BUTTON_REPEAT)
#define CHC_STARTSTOP       BUTTON_PLAY
#define CHC_RESET           BUTTON_REW
#define CHC_MENU            BUTTON_MENU
#define CHC_SETTINGS_INC    BUTTON_UP
#define CHC_SETTINGS_DEC    BUTTON_DOWN
#define CHC_SETTINGS_OK     BUTTON_ENTER
#define CHC_SETTINGS_CANCEL BUTTON_REC

#elif CONFIG_KEYPAD == SANSA_FUZEPLUS_PAD
#define CHC_QUIT BUTTON_POWER
#define CHC_STARTSTOP BUTTON_PLAYPAUSE
#define CHC_RESET BUTTON_BACK
#define CHC_MENU (BUTTON_SELECT|BUTTON_REPEAT)
#define CHC_SETTINGS_INC BUTTON_UP
#define CHC_SETTINGS_DEC BUTTON_DOWN
#define CHC_SETTINGS_OK BUTTON_SELECT
#define CHC_SETTINGS_CANCEL BUTTON_BACK

#elif CONFIG_KEYPAD == SANSA_CONNECT_PAD
#define CHC_QUIT BUTTON_POWER
#define CHC_STARTSTOP BUTTON_NEXT
#define CHC_RESET BUTTON_PREV
#define CHC_MENU BUTTON_SELECT
#define CHC_SETTINGS_INC BUTTON_VOL_UP
#define CHC_SETTINGS_DEC BUTTON_VOL_DOWN
#define CHC_SETTINGS_OK BUTTON_SELECT
#define CHC_SETTINGS_CANCEL BUTTON_LEFT

#elif CONFIG_KEYPAD == SAMSUNG_YPR0_PAD
#define CHC_QUIT BUTTON_BACK
#define CHC_STARTSTOP BUTTON_SELECT
#define CHC_RESET BUTTON_USER
#define CHC_MENU BUTTON_MENU
#define CHC_SETTINGS_INC BUTTON_UP
#define CHC_SETTINGS_DEC BUTTON_DOWN
#define CHC_SETTINGS_OK BUTTON_SELECT
#define CHC_SETTINGS_CANCEL BUTTON_BACK

#elif CONFIG_KEYPAD == HM60X_PAD
#define CHC_QUIT BUTTON_POWER
#define CHC_STARTSTOP BUTTON_SELECT
#define CHC_RESET BUTTON_LEFT
#define CHC_MENU BUTTON_RIGHT
#define CHC_SETTINGS_INC BUTTON_UP
#define CHC_SETTINGS_DEC BUTTON_DOWN
#define CHC_SETTINGS_OK BUTTON_SELECT
#define CHC_SETTINGS_CANCEL BUTTON_POWER

#elif CONFIG_KEYPAD == HM801_PAD
#define CHC_QUIT BUTTON_POWER
#define CHC_STARTSTOP BUTTON_PLAY
#define CHC_RESET BUTTON_LEFT
#define CHC_MENU BUTTON_RIGHT
#define CHC_SETTINGS_INC BUTTON_UP
#define CHC_SETTINGS_DEC BUTTON_DOWN
#define CHC_SETTINGS_OK BUTTON_SELECT
#define CHC_SETTINGS_CANCEL BUTTON_PREV

#else
#error No keymap defined!
#endif

#ifdef HAVE_TOUCHSCREEN
#ifndef CHC_SETTINGS_OK
#define CHC_SETTINGS_OK  BUTTON_CENTER
#endif
#ifndef CHC_STARTSTOP
#define CHC_STARTSTOP    BUTTON_CENTER
#endif
#ifndef CHC_SETTINGS_INC
#define CHC_SETTINGS_INC BUTTON_TOPMIDDLE
#endif
#ifndef CHC_SETTINGS_DEC
#define CHC_SETTINGS_DEC BUTTON_BOTTOMMIDDLE
#endif
#ifndef CHC_RESET
#define CHC_RESET        BUTTON_TOPLEFT
#endif
#ifndef CHC_MENU
#define CHC_MENU         BUTTON_TOPRIGHT
#endif
#endif


/* leave first line blank on bitmap display, for pause icon */
#ifdef HAVE_LCD_BITMAP
#define FIRST_LINE 1
#else
#define FIRST_LINE 0
#endif

#define MAX_PLAYERS 10

enum {
    CHCL_OK,
    CHCL_CANCEL,
    CHCL_USB,
    CHCL_NEXT,
    CHCL_PREV,
};

static struct {
    int nr_timers;
    int total_time;
    int round_time;
} settings;

static struct {
    int total_time;
    int used_time;
    bool hidden;
} timer_holder[MAX_PLAYERS];

static int run_timer(int nr);
static int chessclock_set_int(char* string, 
                              int* variable,
                              int step,
                              int min,
                              int max,
                              int flags);
#define FLAGS_SET_INT_SECONDS 1

static char * show_time(int secs);

static bool chesspause;

#define MAX_TIME 7200

/* this is the plugin entry point */
enum plugin_status plugin_start(const void* parameter)
{
    int i;
    bool done;
    int nr;

    (void)parameter;

    settings.nr_timers = 1;
    settings.total_time = 10;
    settings.round_time = 10;

    /* now go ahead and have fun! */
    rb->splash(HZ, "Chess Clock");

    rb->lcd_clear_display();
    i=0;
    while (1) {
        int res;
        switch (i) {
            case 0:
                res=chessclock_set_int("Number of players",
                                       &settings.nr_timers, 1, 1,
                                       MAX_PLAYERS, 0);
                break;
            case 1:
                res=chessclock_set_int("Total time",
                                       &settings.total_time, 10, 10, MAX_TIME,
                                       FLAGS_SET_INT_SECONDS);
                settings.round_time=settings.total_time;
                break;
            case 2:
                res=chessclock_set_int("Max round time", &settings.round_time,
                                       10, 10, settings.round_time,
                                       FLAGS_SET_INT_SECONDS);
                break;
            default:
                res=-2; /* done */
                break;
        }
        if (res==CHCL_USB) {
            return PLUGIN_USB_CONNECTED;
        } else if (res==CHCL_CANCEL) {
            i--;
            if (i<0) {
                return PLUGIN_OK;
            }
        } else if (res==CHCL_OK) {
            i++;
        } else if (res==-2) { /* done */
            break;
        }
    }
    for (i=0; i<settings.nr_timers; i++) {
        timer_holder[i].total_time=settings.total_time;
        timer_holder[i].used_time=0;
        timer_holder[i].hidden=false;
    }

    chesspause=true; /* We start paused */

    nr=0;
    do {
        int ret=0;
        done=true;
        i = nr;
        do {
            if (!timer_holder[i].hidden)
            {
                nr = i;
                done = false;
                break;
            }
            if (++i == settings.nr_timers)
                i = 0;
        } while (i != nr);

        if (done) {
            return PLUGIN_OK;
        }

        ret = run_timer(nr);
        switch (ret) {
            case CHCL_CANCEL: /* exit */
                done=true;
                break;
            case CHCL_USB:
                return PLUGIN_USB_CONNECTED;
            case CHCL_NEXT:
                nr++;
                if (nr>=settings.nr_timers)
                    nr=0;
                break;
            case CHCL_PREV:
                do {
                    nr--;
                    if (nr<0)
                        nr=settings.nr_timers-1;
                } while (timer_holder[nr].hidden);
                break;
        }
    } while (!done);
    return PLUGIN_OK;
}

#ifdef HAVE_LCD_BITMAP
static void show_pause_mode(bool enabled)
{
    static const char pause_icon[] = {0x00,0x7f,0x7f,0x00,0x7f,0x7f,0x00};

    if (enabled)
        rb->lcd_mono_bitmap((unsigned char *)pause_icon, 52, 0, 7, 8);
    else
    {
        rb->lcd_set_drawmode(DRMODE_SOLID|DRMODE_INVERSEVID);
        rb->lcd_fillrect(52, 0, 7, 8);
        rb->lcd_set_drawmode(DRMODE_SOLID);
    }
}
#else
#define show_pause_mode(enabled)  rb->lcd_icon(ICON_PAUSE, enabled)
#endif

static int run_timer(int nr)
{
    char buf[40];
    char player_info[13];
    long last_tick;
    bool done=false;
    int retval=CHCL_OK;
    long max_ticks=timer_holder[nr].total_time*HZ-timer_holder[nr].used_time;
    long ticks=0;
    bool round_time=false;

    show_pause_mode(chesspause);

    if (settings.round_time*HZ<max_ticks) {
        max_ticks=settings.round_time*HZ;
        round_time=true;
    }
    rb->snprintf(player_info, sizeof(player_info), "Player %d", nr+1);
    rb->lcd_puts(0, FIRST_LINE, (unsigned char *)player_info);
    last_tick=*rb->current_tick;

    while (!done) {
        int button;
        long now;
        if (ticks>=max_ticks) {
            if (round_time)
                rb->lcd_puts(0, FIRST_LINE+1, (unsigned char *)"ROUND UP!");
            else
                rb->lcd_puts(0, FIRST_LINE+1, (unsigned char *)"TIME OUT!");
            rb->backlight_on();
            ticks = max_ticks;
        } else {
            now=*rb->current_tick;
            if (!chesspause) {
                ticks+=now-last_tick;
                if ((max_ticks-ticks)/HZ == 10) {
                     /* Backlight on if 10 seconds remain */
                    rb->backlight_on();
                }
            }
            last_tick=now;
            if (round_time) {
                rb->snprintf(buf, sizeof(buf), "%s/",
                             show_time((max_ticks-ticks+HZ-1)/HZ));
                /* Append total time */
                rb->strcpy(&buf[rb->strlen(buf)],
                           show_time((timer_holder[nr].total_time*HZ-
                                      timer_holder[nr].used_time-
                                      ticks+HZ-1)/HZ));
                rb->lcd_puts(0, FIRST_LINE+1, (unsigned char *)buf);
            } else {
                rb->lcd_puts(0, FIRST_LINE+1,
                             (unsigned char *)show_time((max_ticks-ticks+HZ-1)/HZ));
            }
        }
        rb->lcd_update();

        button = rb->button_get(false);
        switch (button) {
            /* OFF/ON key to exit */
            case CHC_QUIT:
                return CHCL_CANCEL; /* Indicate exit */

            /* PLAY = Stop/Start toggle */
            case CHC_STARTSTOP:
                chesspause=!chesspause;
                show_pause_mode(chesspause);
                break;

            /* LEFT = Reset timer */
            case CHC_RESET:
                ticks=0;
                break;

                /* MENU  */
            case CHC_MENU:
            {
                MENUITEM_STRINGLIST(menu, "Menu", NULL,
                                    "Delete player", "Restart round",
                                    "Set round time", "Set total time",
                                    "Playback Control");

                int val, res;
                switch(rb->do_menu(&menu, NULL, NULL, false))
                {
                    case 0:
                        /* delete player */
                        timer_holder[nr].hidden=true;
                        retval = CHCL_NEXT;
                        done=true;
                        break;
                    case 1:
                        /* restart */
                        ticks=0;
                        last_tick=*rb->current_tick;
                        break;
                    case 2:
                        /* set round time */
                        val=(max_ticks-ticks)/HZ;
                        res=chessclock_set_int("Round time",
                                               &val, 10, 0, MAX_TIME,
                                               FLAGS_SET_INT_SECONDS);
                        if (res==CHCL_USB) {
                            retval = CHCL_USB;
                            done=true;
                        } else if (res==CHCL_OK) {
                            ticks=max_ticks-val*HZ;
                            last_tick=*rb->current_tick;
                        }
                        break;
                    case 3:
                        /* set total time */
                        val=timer_holder[nr].total_time;
                        res=chessclock_set_int("Total time",
                                               &val,
                                               10, 0, MAX_TIME,
                                               FLAGS_SET_INT_SECONDS);
                        if (res==CHCL_USB) {
                            retval = CHCL_USB;
                            done=true;
                        } else if (res==CHCL_OK) {
                            timer_holder[nr].total_time=val;
                        }
                        break;
                    case 4:
                        playback_control(NULL);
                        break;
                    case MENU_ATTACHED_USB:
                        retval = CHCL_USB;
                        done=true;
                        break;
                }
                rb->lcd_clear_display();
                show_pause_mode(chesspause);
                rb->lcd_puts(0, FIRST_LINE, (unsigned char *)player_info);
            }
            break;

            /* UP (RIGHT/+) = Scroll Lap timer up */
            case CHC_SETTINGS_INC:
                retval = CHCL_NEXT;
                done = true;
                break;

            /* DOWN (LEFT/-) = Scroll Lap timer down */
            case CHC_SETTINGS_DEC:
                retval = CHCL_PREV;
                done = true;
                break;

            default:
                if (rb->default_event_handler(button) == SYS_USB_CONNECTED) {
                    retval = CHCL_USB;
                    done = true;
                }
                break;
        }
        rb->sleep(HZ/4); /* Sleep 1/4 of a second */
    }

    timer_holder[nr].used_time+=ticks;

    return retval;
}

static int chessclock_set_int(char* string, 
                              int* variable,
                              int step,
                              int min,
                              int max,
                              int flags)
{
    bool done = false;
    int button;

    rb->lcd_clear_display();
    rb->lcd_puts_scroll(0, FIRST_LINE, (unsigned char *)string);

    while (!done) {
        char str[32];
        if (flags & FLAGS_SET_INT_SECONDS)
            rb->snprintf(str, sizeof str,"%s (m:s)", show_time(*variable));
        else
            rb->snprintf(str, sizeof str,"%d", *variable);
        rb->lcd_puts(0, FIRST_LINE+1, (unsigned char *)str);
        rb->lcd_update();

        button = rb->button_get(true);
        switch(button) {
            case CHC_SETTINGS_INC:
            case CHC_SETTINGS_INC | BUTTON_REPEAT:
                *variable += step;
                break;

            case CHC_SETTINGS_DEC:
            case CHC_SETTINGS_DEC | BUTTON_REPEAT:
                *variable -= step;
                break;

            case CHC_SETTINGS_OK:
#ifdef CHC_SETTINGS_OK2
            case CHC_SETTINGS_OK2:
#endif
                done = true;
                break;

            case CHC_SETTINGS_CANCEL:
#ifdef CHC_SETTINGS_CANCEL2
            case CHC_SETTINGS_CANCEL2:
#endif
                return CHCL_CANCEL;
                break;

            default:
                if (rb->default_event_handler(button) == SYS_USB_CONNECTED)
                    return CHCL_USB;
                break;

        }
        if(*variable > max )
            *variable = max;

        if(*variable < min )
            *variable = min;

    }
    rb->lcd_scroll_stop();

    return CHCL_OK;
}

static char * show_time(int seconds)
{
    static char buf[]="00:00";
    rb->snprintf(buf, sizeof(buf), "%02d:%02d", seconds/60, seconds%60);
    return buf;
}

