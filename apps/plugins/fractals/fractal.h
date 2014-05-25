/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2009 Tomer Shalev
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
#ifndef _FRACTAL_H
#define _FRACTAL_H

/* variable button definitions */
#if CONFIG_KEYPAD == RECORDER_PAD
#define FRACTAL_QUIT            BUTTON_OFF
#define FRACTAL_UP              BUTTON_UP
#define FRACTAL_DOWN            BUTTON_DOWN
#define FRACTAL_LEFT            BUTTON_LEFT
#define FRACTAL_RIGHT           BUTTON_RIGHT
#define FRACTAL_ZOOM_IN         BUTTON_PLAY
#define FRACTAL_ZOOM_OUT        BUTTON_ON
#define FRACTAL_PRECISION_INC   BUTTON_F2
#define FRACTAL_PRECISION_DEC   BUTTON_F1
#define FRACTAL_RESET           BUTTON_F3

#elif CONFIG_KEYPAD == ARCHOS_AV300_PAD
#define FRACTAL_QUIT            BUTTON_OFF
#define FRACTAL_UP              BUTTON_UP
#define FRACTAL_DOWN            BUTTON_DOWN
#define FRACTAL_LEFT            BUTTON_LEFT
#define FRACTAL_RIGHT           BUTTON_RIGHT
#define FRACTAL_ZOOM_IN         BUTTON_SELECT
#define FRACTAL_ZOOM_OUT        BUTTON_ON
#define FRACTAL_PRECISION_INC   BUTTON_F2
#define FRACTAL_PRECISION_DEC   BUTTON_F1
#define FRACTAL_RESET           BUTTON_F3

#elif CONFIG_KEYPAD == ONDIO_PAD
#define FRACTAL_QUIT            BUTTON_OFF
#define FRACTAL_UP              BUTTON_UP
#define FRACTAL_DOWN            BUTTON_DOWN
#define FRACTAL_LEFT            BUTTON_LEFT
#define FRACTAL_RIGHT           BUTTON_RIGHT
#define FRACTAL_ZOOM_IN_PRE     BUTTON_MENU
#define FRACTAL_ZOOM_IN         (BUTTON_MENU | BUTTON_REL)
#define FRACTAL_ZOOM_IN2        (BUTTON_MENU | BUTTON_UP)
#define FRACTAL_ZOOM_OUT        (BUTTON_MENU | BUTTON_DOWN)
#define FRACTAL_PRECISION_INC   (BUTTON_MENU | BUTTON_RIGHT)
#define FRACTAL_PRECISION_DEC   (BUTTON_MENU | BUTTON_LEFT)
#define FRACTAL_RESET           (BUTTON_MENU | BUTTON_OFF)

#elif (CONFIG_KEYPAD == IRIVER_H100_PAD) || \
      (CONFIG_KEYPAD == IRIVER_H300_PAD)
#define FRACTAL_QUIT            BUTTON_OFF
#define FRACTAL_UP              BUTTON_UP
#define FRACTAL_DOWN            BUTTON_DOWN
#define FRACTAL_LEFT            BUTTON_LEFT
#define FRACTAL_RIGHT           BUTTON_RIGHT
#define FRACTAL_ZOOM_IN         BUTTON_SELECT
#define FRACTAL_ZOOM_OUT        BUTTON_MODE
#define FRACTAL_PRECISION_INC   (BUTTON_ON | BUTTON_RIGHT)
#define FRACTAL_PRECISION_DEC   (BUTTON_ON | BUTTON_LEFT)
#define FRACTAL_RESET           BUTTON_REC

#define FRACTAL_RC_QUIT BUTTON_RC_STOP

#elif (CONFIG_KEYPAD == IPOD_4G_PAD) || \
      (CONFIG_KEYPAD == IPOD_3G_PAD) || \
      (CONFIG_KEYPAD == IPOD_1G2G_PAD)
#define FRACTAL_QUIT            (BUTTON_SELECT | BUTTON_MENU)
#define FRACTAL_UP              BUTTON_MENU
#define FRACTAL_DOWN            BUTTON_PLAY
#define FRACTAL_LEFT            BUTTON_LEFT
#define FRACTAL_RIGHT           BUTTON_RIGHT
#define FRACTAL_ZOOM_IN         BUTTON_SCROLL_FWD
#define FRACTAL_ZOOM_OUT        BUTTON_SCROLL_BACK
#define FRACTAL_PRECISION_INC   (BUTTON_SELECT | BUTTON_RIGHT)
#define FRACTAL_PRECISION_DEC   (BUTTON_SELECT | BUTTON_LEFT)
#define FRACTAL_RESET           (BUTTON_SELECT | BUTTON_PLAY)

#elif CONFIG_KEYPAD == IAUDIO_X5M5_PAD
#define FRACTAL_QUIT            BUTTON_POWER
#define FRACTAL_UP              BUTTON_UP
#define FRACTAL_DOWN            BUTTON_DOWN
#define FRACTAL_LEFT            BUTTON_LEFT
#define FRACTAL_RIGHT           BUTTON_RIGHT
#define FRACTAL_ZOOM_IN_PRE     BUTTON_SELECT
#define FRACTAL_ZOOM_IN         (BUTTON_SELECT | BUTTON_REL)
#define FRACTAL_ZOOM_OUT_PRE    BUTTON_SELECT
#define FRACTAL_ZOOM_OUT        (BUTTON_SELECT | BUTTON_REPEAT)
#define FRACTAL_PRECISION_INC_PRE BUTTON_PLAY
#define FRACTAL_PRECISION_INC   (BUTTON_PLAY | BUTTON_REL)
#define FRACTAL_PRECISION_DEC_PRE BUTTON_PLAY
#define FRACTAL_PRECISION_DEC   (BUTTON_PLAY | BUTTON_REPEAT)
#define FRACTAL_RESET           BUTTON_REC

#elif CONFIG_KEYPAD == GIGABEAT_PAD
#define FRACTAL_QUIT            BUTTON_POWER
#define FRACTAL_UP              BUTTON_UP
#define FRACTAL_DOWN            BUTTON_DOWN
#define FRACTAL_LEFT            BUTTON_LEFT
#define FRACTAL_RIGHT           BUTTON_RIGHT
#define FRACTAL_ZOOM_IN_PRE     BUTTON_SELECT
#define FRACTAL_ZOOM_IN         (BUTTON_SELECT | BUTTON_REL)
#define FRACTAL_ZOOM_OUT_PRE    BUTTON_SELECT
#define FRACTAL_ZOOM_OUT        (BUTTON_SELECT | BUTTON_REPEAT)
#define FRACTAL_PRECISION_INC   BUTTON_VOL_UP
#define FRACTAL_PRECISION_DEC   BUTTON_VOL_DOWN
#define FRACTAL_RESET           BUTTON_A

#elif CONFIG_KEYPAD == SANSA_E200_PAD
#define FRACTAL_QUIT            BUTTON_POWER
#define FRACTAL_UP              BUTTON_UP
#define FRACTAL_DOWN            BUTTON_DOWN
#define FRACTAL_LEFT            BUTTON_LEFT
#define FRACTAL_RIGHT           BUTTON_RIGHT
#define FRACTAL_ZOOM_IN         BUTTON_SCROLL_FWD
#define FRACTAL_ZOOM_OUT        BUTTON_SCROLL_BACK
#define FRACTAL_PRECISION_INC   (BUTTON_SELECT | BUTTON_RIGHT)
#define FRACTAL_PRECISION_DEC   (BUTTON_SELECT | BUTTON_LEFT)
#define FRACTAL_RESET           BUTTON_REC

#elif CONFIG_KEYPAD == SANSA_FUZE_PAD
#define FRACTAL_QUIT            (BUTTON_HOME|BUTTON_REPEAT)
#define FRACTAL_UP              BUTTON_UP
#define FRACTAL_DOWN            BUTTON_DOWN
#define FRACTAL_LEFT            BUTTON_LEFT
#define FRACTAL_RIGHT           BUTTON_RIGHT
#define FRACTAL_ZOOM_IN         BUTTON_SCROLL_FWD
#define FRACTAL_ZOOM_OUT        BUTTON_SCROLL_BACK
#define FRACTAL_PRECISION_INC   (BUTTON_SELECT | BUTTON_RIGHT)
#define FRACTAL_PRECISION_DEC   (BUTTON_SELECT | BUTTON_LEFT)
#define FRACTAL_RESET           (BUTTON_SELECT | BUTTON_REPEAT)

#elif CONFIG_KEYPAD == SANSA_C200_PAD
#define FRACTAL_QUIT            BUTTON_POWER
#define FRACTAL_UP              BUTTON_UP
#define FRACTAL_DOWN            BUTTON_DOWN
#define FRACTAL_LEFT            BUTTON_LEFT
#define FRACTAL_RIGHT           BUTTON_RIGHT
#define FRACTAL_ZOOM_IN         BUTTON_VOL_UP
#define FRACTAL_ZOOM_OUT        BUTTON_VOL_DOWN
#define FRACTAL_PRECISION_INC   (BUTTON_SELECT | BUTTON_RIGHT)
#define FRACTAL_PRECISION_DEC   (BUTTON_SELECT | BUTTON_LEFT)
#define FRACTAL_RESET           BUTTON_REC

#elif CONFIG_KEYPAD == SANSA_CLIP_PAD
#define FRACTAL_QUIT            BUTTON_POWER
#define FRACTAL_UP              BUTTON_UP
#define FRACTAL_DOWN            BUTTON_DOWN
#define FRACTAL_LEFT            BUTTON_LEFT
#define FRACTAL_RIGHT           BUTTON_RIGHT
#define FRACTAL_ZOOM_IN         BUTTON_VOL_UP
#define FRACTAL_ZOOM_OUT        BUTTON_VOL_DOWN
#define FRACTAL_PRECISION_INC   (BUTTON_SELECT | BUTTON_RIGHT)
#define FRACTAL_PRECISION_DEC   (BUTTON_SELECT | BUTTON_LEFT)
#define FRACTAL_RESET           BUTTON_HOME

#elif CONFIG_KEYPAD == SANSA_M200_PAD
#define FRACTAL_QUIT            BUTTON_POWER
#define FRACTAL_UP              BUTTON_UP
#define FRACTAL_DOWN            BUTTON_DOWN
#define FRACTAL_LEFT            BUTTON_LEFT
#define FRACTAL_RIGHT           BUTTON_RIGHT
#define FRACTAL_ZOOM_IN         BUTTON_VOL_UP
#define FRACTAL_ZOOM_OUT        BUTTON_VOL_DOWN
#define FRACTAL_PRECISION_INC   (BUTTON_SELECT | BUTTON_RIGHT)
#define FRACTAL_PRECISION_DEC   (BUTTON_SELECT | BUTTON_LEFT)
#define FRACTAL_RESET           (BUTTON_SELECT | BUTTON_UP)

#elif CONFIG_KEYPAD == IRIVER_H10_PAD
#define FRACTAL_QUIT            BUTTON_POWER
#define FRACTAL_UP              BUTTON_SCROLL_UP
#define FRACTAL_DOWN            BUTTON_SCROLL_DOWN
#define FRACTAL_LEFT            BUTTON_LEFT
#define FRACTAL_RIGHT           BUTTON_RIGHT
#define FRACTAL_ZOOM_IN_PRE     BUTTON_PLAY
#define FRACTAL_ZOOM_IN         (BUTTON_PLAY | BUTTON_REL)
#define FRACTAL_ZOOM_OUT_PRE    BUTTON_PLAY
#define FRACTAL_ZOOM_OUT        (BUTTON_PLAY | BUTTON_REPEAT)
#define FRACTAL_PRECISION_INC   BUTTON_FF
#define FRACTAL_PRECISION_DEC   BUTTON_REW
#define FRACTAL_RESET           (BUTTON_PLAY | BUTTON_REW)

#elif CONFIG_KEYPAD == IRIVER_IFP7XX_PAD
#define FRACTAL_QUIT            BUTTON_EQ
#define FRACTAL_UP              BUTTON_UP
#define FRACTAL_DOWN            BUTTON_DOWN
#define FRACTAL_LEFT            BUTTON_LEFT
#define FRACTAL_RIGHT           BUTTON_RIGHT
#define FRACTAL_ZOOM_IN_PRE     BUTTON_SELECT
#define FRACTAL_ZOOM_IN         (BUTTON_SELECT | BUTTON_REL)
#define FRACTAL_ZOOM_OUT_PRE    BUTTON_SELECT
#define FRACTAL_ZOOM_OUT        (BUTTON_SELECT | BUTTON_REPEAT)
#define FRACTAL_PRECISION_INC   (BUTTON_PLAY | BUTTON_RIGHT)
#define FRACTAL_PRECISION_DEC   (BUTTON_PLAY | BUTTON_LEFT)
#define FRACTAL_RESET           BUTTON_MODE

#elif CONFIG_KEYPAD == GIGABEAT_S_PAD
#define FRACTAL_QUIT            BUTTON_BACK
#define FRACTAL_UP              BUTTON_UP
#define FRACTAL_DOWN            BUTTON_DOWN
#define FRACTAL_LEFT            BUTTON_LEFT
#define FRACTAL_RIGHT           BUTTON_RIGHT
#define FRACTAL_ZOOM_IN         BUTTON_VOL_UP
#define FRACTAL_ZOOM_OUT        BUTTON_VOL_DOWN
#define FRACTAL_PRECISION_INC   BUTTON_PREV
#define FRACTAL_PRECISION_DEC   BUTTON_NEXT
#define FRACTAL_RESET           BUTTON_MENU

#elif CONFIG_KEYPAD == MROBE100_PAD
#define FRACTAL_QUIT            BUTTON_POWER
#define FRACTAL_UP              BUTTON_UP
#define FRACTAL_DOWN            BUTTON_DOWN
#define FRACTAL_LEFT            BUTTON_LEFT
#define FRACTAL_RIGHT           BUTTON_RIGHT
#define FRACTAL_ZOOM_IN_PRE     BUTTON_SELECT
#define FRACTAL_ZOOM_IN         (BUTTON_SELECT | BUTTON_REL)
#define FRACTAL_ZOOM_OUT_PRE    BUTTON_SELECT
#define FRACTAL_ZOOM_OUT        (BUTTON_SELECT | BUTTON_REPEAT)
#define FRACTAL_PRECISION_INC   BUTTON_MENU
#define FRACTAL_PRECISION_DEC   BUTTON_PLAY
#define FRACTAL_RESET           BUTTON_DISPLAY

#elif CONFIG_KEYPAD == IAUDIO_M3_PAD
#define FRACTAL_QUIT            BUTTON_RC_REC
#define FRACTAL_UP              BUTTON_RC_VOL_UP
#define FRACTAL_DOWN            BUTTON_RC_VOL_DOWN
#define FRACTAL_LEFT            BUTTON_RC_REW
#define FRACTAL_RIGHT           BUTTON_RC_FF
#define FRACTAL_ZOOM_IN_PRE     BUTTON_RC_PLAY
#define FRACTAL_ZOOM_IN         (BUTTON_RC_PLAY | BUTTON_REL)
#define FRACTAL_ZOOM_OUT_PRE    BUTTON_RC_PLAY
#define FRACTAL_ZOOM_OUT        (BUTTON_RC_PLAY | BUTTON_REPEAT)
#define FRACTAL_PRECISION_INC_PRE BUTTON_RC_MODE
#define FRACTAL_PRECISION_INC   (BUTTON_RC_MODE|BUTTON_REL)
#define FRACTAL_PRECISION_DEC_PRE BUTTON_RC_MODE
#define FRACTAL_PRECISION_DEC   (BUTTON_RC_MODE|BUTTON_REPEAT)
#define FRACTAL_RESET           BUTTON_RC_MENU

#elif CONFIG_KEYPAD == COWON_D2_PAD
#define FRACTAL_QUIT            BUTTON_POWER

#elif CONFIG_KEYPAD == CREATIVEZVM_PAD
#define FRACTAL_QUIT            BUTTON_BACK
#define FRACTAL_UP              BUTTON_UP
#define FRACTAL_DOWN            BUTTON_DOWN
#define FRACTAL_LEFT            BUTTON_LEFT
#define FRACTAL_RIGHT           BUTTON_RIGHT
#define FRACTAL_ZOOM_IN         BUTTON_PLAY
#define FRACTAL_ZOOM_OUT        BUTTON_MENU
#define FRACTAL_PRECISION_INC   (BUTTON_UP | BUTTON_CUSTOM)
#define FRACTAL_PRECISION_DEC   (BUTTON_DOWN | BUTTON_CUSTOM)
#define FRACTAL_RESET           BUTTON_SELECT

#elif CONFIG_KEYPAD == CREATIVE_ZENXFI3_PAD
#define FRACTAL_QUIT            BUTTON_POWER
#define FRACTAL_UP              BUTTON_UP
#define FRACTAL_DOWN            BUTTON_DOWN
#define FRACTAL_LEFT            BUTTON_BACK
#define FRACTAL_RIGHT           BUTTON_MENU
#define FRACTAL_ZOOM_IN         BUTTON_VOL_UP
#define FRACTAL_ZOOM_OUT        BUTTON_VOL_DOWN
#define FRACTAL_PRECISION_INC   (BUTTON_PLAY | BUTTON_VOL_UP)
#define FRACTAL_PRECISION_DEC   (BUTTON_PLAY | BUTTON_VOL_DOWN)
#define FRACTAL_RESET           (BUTTON_PLAY | BUTTON_REPEAT)

#elif CONFIG_KEYPAD == PHILIPS_HDD1630_PAD
#define FRACTAL_QUIT            BUTTON_POWER
#define FRACTAL_UP              BUTTON_UP
#define FRACTAL_DOWN            BUTTON_DOWN
#define FRACTAL_LEFT            BUTTON_LEFT
#define FRACTAL_RIGHT           BUTTON_RIGHT
#define FRACTAL_ZOOM_IN         BUTTON_VIEW
#define FRACTAL_ZOOM_OUT        BUTTON_PLAYLIST
#define FRACTAL_PRECISION_INC   BUTTON_VOL_UP
#define FRACTAL_PRECISION_DEC   BUTTON_VOL_DOWN
#define FRACTAL_RESET           BUTTON_MENU

#elif CONFIG_KEYPAD == PHILIPS_HDD6330_PAD
#define FRACTAL_QUIT            BUTTON_POWER
#define FRACTAL_UP              BUTTON_UP
#define FRACTAL_DOWN            BUTTON_DOWN
#define FRACTAL_LEFT            BUTTON_PREV
#define FRACTAL_RIGHT           BUTTON_NEXT
#define FRACTAL_ZOOM_IN         BUTTON_RIGHT
#define FRACTAL_ZOOM_OUT        BUTTON_LEFT
#define FRACTAL_PRECISION_INC   BUTTON_VOL_UP
#define FRACTAL_PRECISION_DEC   BUTTON_VOL_DOWN
#define FRACTAL_RESET           BUTTON_MENU

#elif CONFIG_KEYPAD == PHILIPS_SA9200_PAD
#define FRACTAL_QUIT            BUTTON_POWER
#define FRACTAL_UP              BUTTON_UP
#define FRACTAL_DOWN            BUTTON_DOWN
#define FRACTAL_LEFT            BUTTON_PREV
#define FRACTAL_RIGHT           BUTTON_NEXT
#define FRACTAL_ZOOM_IN         BUTTON_RIGHT
#define FRACTAL_ZOOM_OUT        BUTTON_LEFT
#define FRACTAL_PRECISION_INC   BUTTON_VOL_UP
#define FRACTAL_PRECISION_DEC   BUTTON_VOL_DOWN
#define FRACTAL_RESET           BUTTON_MENU

#elif CONFIG_KEYPAD == ONDAVX747_PAD || CONFIG_KEYPAD == MROBE500_PAD
#define FRACTAL_QUIT            BUTTON_POWER

#elif CONFIG_KEYPAD == ONDAVX777_PAD
#define FRACTAL_QUIT            BUTTON_POWER

#elif (CONFIG_KEYPAD == SAMSUNG_YH820_PAD) || \
      (CONFIG_KEYPAD == SAMSUNG_YH920_PAD)
#define FRACTAL_QUIT            BUTTON_REC
#define FRACTAL_UP              BUTTON_UP
#define FRACTAL_DOWN            BUTTON_DOWN
#define FRACTAL_LEFT            BUTTON_LEFT
#define FRACTAL_RIGHT           BUTTON_RIGHT
#define FRACTAL_ZOOM_IN         BUTTON_FFWD
#define FRACTAL_ZOOM_OUT        BUTTON_REW
#define FRACTAL_PRECISION_INC   (BUTTON_PLAY | BUTTON_UP)
#define FRACTAL_PRECISION_DEC   (BUTTON_PLAY | BUTTON_DOWN)
#define FRACTAL_RESET           (BUTTON_PLAY | BUTTON_RIGHT)

#elif CONFIG_KEYPAD == PBELL_VIBE500_PAD
#define FRACTAL_QUIT            BUTTON_REC
#define FRACTAL_UP              BUTTON_UP
#define FRACTAL_DOWN            BUTTON_DOWN
#define FRACTAL_LEFT            BUTTON_PREV
#define FRACTAL_RIGHT           BUTTON_NEXT
#define FRACTAL_ZOOM_IN         BUTTON_PLAY
#define FRACTAL_ZOOM_OUT        BUTTON_MENU
#define FRACTAL_PRECISION_INC   BUTTON_OK
#define FRACTAL_PRECISION_DEC   BUTTON_CANCEL
#define FRACTAL_RESET           (BUTTON_CANCEL | BUTTON_REPEAT)

#elif CONFIG_KEYPAD == MPIO_HD200_PAD
#define FRACTAL_QUIT            (BUTTON_REC | BUTTON_PLAY)
#define FRACTAL_UP              BUTTON_REW
#define FRACTAL_DOWN            BUTTON_FF
#define FRACTAL_LEFT            BUTTON_VOL_DOWN
#define FRACTAL_RIGHT           BUTTON_VOL_UP
#define FRACTAL_ZOOM_IN         (BUTTON_PLAY|BUTTON_FF)
#define FRACTAL_ZOOM_OUT        (BUTTON_PLAY|BUTTON_REW)
#define FRACTAL_PRECISION_INC   (BUTTON_PLAY | BUTTON_VOL_UP)
#define FRACTAL_PRECISION_DEC   (BUTTON_PLAY | BUTTON_VOL_DOWN)
#define FRACTAL_RESET           BUTTON_REC

#elif CONFIG_KEYPAD == MPIO_HD300_PAD
#define FRACTAL_QUIT            BUTTON_MENU
#define FRACTAL_UP              BUTTON_UP
#define FRACTAL_DOWN            BUTTON_DOWN
#define FRACTAL_LEFT            BUTTON_REW
#define FRACTAL_RIGHT           BUTTON_FF
#define FRACTAL_ZOOM_IN         (BUTTON_PLAY | BUTTON_UP)
#define FRACTAL_ZOOM_OUT        (BUTTON_PLAY | BUTTON_DOWN)
#define FRACTAL_PRECISION_INC   (BUTTON_PLAY | BUTTON_FF)
#define FRACTAL_PRECISION_DEC   (BUTTON_PLAY | BUTTON_REW)
#define FRACTAL_RESET           BUTTON_REC

#elif CONFIG_KEYPAD == SANSA_FUZEPLUS_PAD
#define FRACTAL_QUIT            BUTTON_POWER
#define FRACTAL_UP              BUTTON_UP
#define FRACTAL_DOWN            BUTTON_DOWN
#define FRACTAL_LEFT            BUTTON_LEFT
#define FRACTAL_RIGHT           BUTTON_RIGHT
#define FRACTAL_ZOOM_IN         BUTTON_SELECT
#define FRACTAL_ZOOM_OUT        BUTTON_PLAYPAUSE
#define FRACTAL_PRECISION_INC   BUTTON_BOTTOMRIGHT
#define FRACTAL_PRECISION_DEC   BUTTON_BOTTOMLEFT
#define FRACTAL_RESET           BUTTON_BACK

#elif CONFIG_KEYPAD == SANSA_CONNECT_PAD
#define FRACTAL_QUIT            BUTTON_POWER
#define FRACTAL_UP              BUTTON_UP
#define FRACTAL_DOWN            BUTTON_DOWN
#define FRACTAL_LEFT            BUTTON_LEFT
#define FRACTAL_RIGHT           BUTTON_RIGHT
#define FRACTAL_ZOOM_IN         BUTTON_SCROLL_FWD
#define FRACTAL_ZOOM_OUT        BUTTON_SCROLL_BACK
#define FRACTAL_PRECISION_INC   BUTTON_VOL_UP
#define FRACTAL_PRECISION_DEC   BUTTON_VOL_DOWN
#define FRACTAL_RESET           BUTTON_PREV

#elif CONFIG_KEYPAD == SAMSUNG_YPR0_PAD
#define FRACTAL_QUIT            BUTTON_BACK
#define FRACTAL_UP              BUTTON_UP
#define FRACTAL_DOWN            BUTTON_DOWN
#define FRACTAL_LEFT            BUTTON_LEFT
#define FRACTAL_RIGHT           BUTTON_RIGHT
#define FRACTAL_ZOOM_IN         (BUTTON_MENU|BUTTON_REL)
#define FRACTAL_ZOOM_OUT        (BUTTON_USER|BUTTON_REL)
#define FRACTAL_PRECISION_INC   (BUTTON_MENU|BUTTON_REPEAT)
#define FRACTAL_PRECISION_DEC   (BUTTON_USER|BUTTON_REPEAT)
#define FRACTAL_RESET           BUTTON_POWER

#elif CONFIG_KEYPAD == HM60X_PAD
#define FRACTAL_QUIT            BUTTON_POWER
#define FRACTAL_UP              BUTTON_UP
#define FRACTAL_DOWN            BUTTON_DOWN
#define FRACTAL_LEFT            BUTTON_LEFT
#define FRACTAL_RIGHT           BUTTON_RIGHT
#define FRACTAL_ZOOM_IN         BUTTON_SELECT
#define FRACTAL_ZOOM_OUT        (BUTTON_POWER | BUTTON_SELECT)
#define FRACTAL_PRECISION_INC   (BUTTON_POWER | BUTTON_UP)
#define FRACTAL_PRECISION_DEC   (BUTTON_POWER | BUTTON_DOWN)
#define FRACTAL_RESET           (BUTTON_POWER | BUTTON_LEFT)

#elif CONFIG_KEYPAD == HM801_PAD
#define FRACTAL_QUIT            BUTTON_POWER
#define FRACTAL_UP              BUTTON_UP
#define FRACTAL_DOWN            BUTTON_DOWN
#define FRACTAL_LEFT            BUTTON_LEFT
#define FRACTAL_RIGHT           BUTTON_RIGHT
#define FRACTAL_ZOOM_IN         BUTTON_SELECT
#define FRACTAL_ZOOM_OUT        BUTTON_PLAY
#define FRACTAL_PRECISION_INC   BUTTON_NEXT
#define FRACTAL_PRECISION_DEC   BUTTON_PREV
#define FRACTAL_RESET           (BUTTON_POWER | BUTTON_PLAY)

#else
#error No keymap defined!
#endif

#ifdef HAVE_TOUCHSCREEN
#ifndef FRACTAL_UP
#define FRACTAL_UP              BUTTON_TOPMIDDLE
#endif
#ifndef FRACTAL_DOWN
#define FRACTAL_DOWN            BUTTON_BOTTOMMIDDLE
#endif
#ifndef FRACTAL_LEFT
#define FRACTAL_LEFT            BUTTON_MIDLEFT
#endif
#ifndef FRACTAL_RIGHT
#define FRACTAL_RIGHT           BUTTON_MIDRIGHT
#endif
#ifndef FRACTAL_ZOOM_IN
#define FRACTAL_ZOOM_IN_PRE     BUTTON_TOPRIGHT
#define FRACTAL_ZOOM_IN         (BUTTON_TOPRIGHT | BUTTON_REL)
#endif
#ifndef FRACTAL_ZOOM_OUT
#define FRACTAL_ZOOM_OUT_PRE    BUTTON_TOPLEFT
#define FRACTAL_ZOOM_OUT        (BUTTON_TOPLEFT | BUTTON_REL)
#endif
#ifndef FRACTAL_PRECISION_INC
#define FRACTAL_PRECISION_INC   BUTTON_BOTTOMRIGHT
#endif
#ifndef FRACTAL_PRECISION_DEC
#define FRACTAL_PRECISION_DEC   BUTTON_BOTTOMLEFT
#endif
#ifndef FRACTAL_RESET
#define FRACTAL_RESET           BUTTON_CENTER
#endif
#endif

#endif

