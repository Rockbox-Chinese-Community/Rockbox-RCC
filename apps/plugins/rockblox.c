/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2005 Eli Sherer
 *
 * Heavily modified for embedded use by Björn Stenberg (bjorn@haxx.se)
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
#include "lib/display_text.h"
#include "lib/helper.h"
#include "lib/highscore.h"
#include "lib/playback_control.h"
#include "lib/playergfx.h"
#include "lib/mylcd.h"



#if (CONFIG_KEYPAD == IPOD_4G_PAD) || \
    (CONFIG_KEYPAD == IPOD_3G_PAD) || \
    (CONFIG_KEYPAD == IPOD_1G2G_PAD)

#define ROCKBLOX_OFF           (BUTTON_MENU | BUTTON_SELECT)
#define ROCKBLOX_ROTATE_CCW    BUTTON_SCROLL_BACK
#define ROCKBLOX_ROTATE_CCW2   (BUTTON_MENU | BUTTON_REL)
#define ROCKBLOX_ROTATE_CW     BUTTON_SCROLL_FWD
#define ROCKBLOX_LEFT          BUTTON_LEFT
#define ROCKBLOX_RIGHT         BUTTON_RIGHT
#define ROCKBLOX_DOWN          BUTTON_PLAY
#define ROCKBLOX_RESTART       (BUTTON_SELECT | BUTTON_PLAY)
#define ROCKBLOX_DROP          (BUTTON_SELECT | BUTTON_REL)

#elif (CONFIG_KEYPAD == IRIVER_H100_PAD) || \
      (CONFIG_KEYPAD == IRIVER_H300_PAD)

#define ROCKBLOX_OFF           BUTTON_OFF
#define ROCKBLOX_ROTATE_CCW    BUTTON_UP
#define ROCKBLOX_ROTATE_CW     BUTTON_SELECT
#define ROCKBLOX_DOWN          BUTTON_DOWN
#define ROCKBLOX_LEFT          BUTTON_LEFT
#define ROCKBLOX_RIGHT         BUTTON_RIGHT
#define ROCKBLOX_DROP          BUTTON_MODE
#define ROCKBLOX_RESTART       BUTTON_ON

#define ROCKBLOX_RC_OFF       BUTTON_RC_STOP

#elif CONFIG_KEYPAD == RECORDER_PAD

#define ROCKBLOX_OFF           BUTTON_OFF
#define ROCKBLOX_ROTATE_CCW    BUTTON_UP
#define ROCKBLOX_ROTATE_CW     BUTTON_PLAY
#define ROCKBLOX_DOWN          BUTTON_DOWN
#define ROCKBLOX_LEFT          BUTTON_LEFT
#define ROCKBLOX_RIGHT         BUTTON_RIGHT
#define ROCKBLOX_DROP          BUTTON_ON
#define ROCKBLOX_RESTART       BUTTON_F1

#elif CONFIG_KEYPAD == ARCHOS_AV300_PAD

#define ROCKBLOX_OFF           BUTTON_OFF
#define ROCKBLOX_ROTATE_CCW    BUTTON_UP
#define ROCKBLOX_ROTATE_CW     BUTTON_SELECT
#define ROCKBLOX_DOWN          BUTTON_DOWN
#define ROCKBLOX_LEFT          BUTTON_LEFT
#define ROCKBLOX_RIGHT         BUTTON_RIGHT
#define ROCKBLOX_DROP          BUTTON_ON
#define ROCKBLOX_RESTART       BUTTON_F1

#elif CONFIG_KEYPAD == PLAYER_PAD

#define ROCKBLOX_OFF_PRE       BUTTON_STOP
#define ROCKBLOX_OFF           (BUTTON_STOP|BUTTON_REL)
#define ROCKBLOX_ROTATE_CCW    BUTTON_PLAY
#define ROCKBLOX_ROTATE_CW     (BUTTON_ON|BUTTON_PLAY)
#define ROCKBLOX_DOWN          BUTTON_MENU
#define ROCKBLOX_LEFT          BUTTON_LEFT
#define ROCKBLOX_RIGHT         BUTTON_RIGHT
#define ROCKBLOX_DROP_PRE      BUTTON_ON
#define ROCKBLOX_DROP          (BUTTON_ON|BUTTON_REL)
#define ROCKBLOX_RESTART       (BUTTON_STOP|BUTTON_MENU)

#elif CONFIG_KEYPAD == ONDIO_PAD

#define ROCKBLOX_OFF_PRE       BUTTON_OFF
#define ROCKBLOX_OFF           (BUTTON_OFF|BUTTON_REL)
#define ROCKBLOX_ROTATE_CCW    BUTTON_UP
#define ROCKBLOX_ROTATE_CW     (BUTTON_MENU|BUTTON_UP)
#define ROCKBLOX_DOWN          BUTTON_DOWN
#define ROCKBLOX_LEFT          BUTTON_LEFT
#define ROCKBLOX_RIGHT         BUTTON_RIGHT
#define ROCKBLOX_DROP_PRE      BUTTON_MENU
#define ROCKBLOX_DROP          (BUTTON_MENU|BUTTON_REL)
#define ROCKBLOX_RESTART       (BUTTON_OFF|BUTTON_MENU)

#elif CONFIG_KEYPAD == IAUDIO_X5M5_PAD

#define ROCKBLOX_OFF           BUTTON_POWER
#define ROCKBLOX_ROTATE_CCW    BUTTON_UP
#define ROCKBLOX_ROTATE_CW     BUTTON_SELECT
#define ROCKBLOX_DOWN          BUTTON_DOWN
#define ROCKBLOX_LEFT          BUTTON_LEFT
#define ROCKBLOX_RIGHT         BUTTON_RIGHT
#define ROCKBLOX_DROP          BUTTON_REC
#define ROCKBLOX_RESTART       BUTTON_PLAY

#elif CONFIG_KEYPAD == SANSA_E200_PAD

#define ROCKBLOX_OFF           BUTTON_POWER
#define ROCKBLOX_ROTATE_CCW    BUTTON_SCROLL_BACK
#define ROCKBLOX_ROTATE_CCW2   BUTTON_UP
#define ROCKBLOX_ROTATE_CW     BUTTON_SCROLL_FWD
#define ROCKBLOX_DOWN          BUTTON_DOWN
#define ROCKBLOX_LEFT          BUTTON_LEFT
#define ROCKBLOX_RIGHT         BUTTON_RIGHT
#define ROCKBLOX_DROP          BUTTON_SELECT
#define ROCKBLOX_RESTART       BUTTON_REC

#elif CONFIG_KEYPAD == SANSA_FUZE_PAD

#define ROCKBLOX_OFF           (BUTTON_HOME|BUTTON_REPEAT)
#define ROCKBLOX_ROTATE_CCW    BUTTON_SCROLL_BACK
#define ROCKBLOX_ROTATE_CCW2   BUTTON_UP
#define ROCKBLOX_ROTATE_CW     BUTTON_SCROLL_FWD
#define ROCKBLOX_DOWN          BUTTON_DOWN
#define ROCKBLOX_LEFT          BUTTON_LEFT
#define ROCKBLOX_RIGHT         BUTTON_RIGHT
#define ROCKBLOX_DROP          (BUTTON_SELECT | BUTTON_REL)
#define ROCKBLOX_RESTART       (BUTTON_SELECT | BUTTON_UP)


#elif CONFIG_KEYPAD == SANSA_C200_PAD

#define ROCKBLOX_OFF           BUTTON_POWER
#define ROCKBLOX_ROTATE_CCW    BUTTON_UP
#define ROCKBLOX_ROTATE_CCW2   BUTTON_VOL_DOWN
#define ROCKBLOX_ROTATE_CW     BUTTON_VOL_UP
#define ROCKBLOX_DOWN          BUTTON_DOWN
#define ROCKBLOX_LEFT          BUTTON_LEFT
#define ROCKBLOX_RIGHT         BUTTON_RIGHT
#define ROCKBLOX_DROP          BUTTON_SELECT
#define ROCKBLOX_RESTART       BUTTON_REC

#elif CONFIG_KEYPAD == SANSA_CLIP_PAD

#define ROCKBLOX_OFF           BUTTON_POWER
#define ROCKBLOX_ROTATE_CCW    BUTTON_UP
#define ROCKBLOX_ROTATE_CCW2   BUTTON_VOL_DOWN
#define ROCKBLOX_ROTATE_CW     BUTTON_VOL_UP
#define ROCKBLOX_DOWN          BUTTON_DOWN
#define ROCKBLOX_LEFT          BUTTON_LEFT
#define ROCKBLOX_RIGHT         BUTTON_RIGHT
#define ROCKBLOX_DROP          BUTTON_SELECT
#define ROCKBLOX_RESTART       BUTTON_HOME

#elif CONFIG_KEYPAD == SANSA_M200_PAD

#define ROCKBLOX_OFF           BUTTON_POWER
#define ROCKBLOX_ROTATE_CCW    BUTTON_UP
#define ROCKBLOX_ROTATE_CCW2   BUTTON_VOL_DOWN
#define ROCKBLOX_ROTATE_CW     BUTTON_VOL_UP
#define ROCKBLOX_DOWN          BUTTON_DOWN
#define ROCKBLOX_LEFT          BUTTON_LEFT
#define ROCKBLOX_RIGHT         BUTTON_RIGHT
#define ROCKBLOX_RESTART       (BUTTON_SELECT | BUTTON_UP)
#define ROCKBLOX_DROP          (BUTTON_SELECT | BUTTON_REL)

#elif CONFIG_KEYPAD == IRIVER_H10_PAD

#define ROCKBLOX_OFF           BUTTON_POWER
#define ROCKBLOX_ROTATE_CCW    BUTTON_SCROLL_UP
#define ROCKBLOX_ROTATE_CW     BUTTON_REW
#define ROCKBLOX_DOWN          BUTTON_SCROLL_DOWN
#define ROCKBLOX_LEFT          BUTTON_LEFT
#define ROCKBLOX_RIGHT         BUTTON_RIGHT
#define ROCKBLOX_DROP          BUTTON_FF
#define ROCKBLOX_RESTART       BUTTON_PLAY

#elif CONFIG_KEYPAD == GIGABEAT_PAD

#define ROCKBLOX_OFF           BUTTON_POWER
#define ROCKBLOX_ROTATE_CCW    BUTTON_VOL_DOWN
#define ROCKBLOX_ROTATE_CW     BUTTON_VOL_UP
#define ROCKBLOX_ROTATE        BUTTON_UP
#define ROCKBLOX_DOWN          BUTTON_DOWN
#define ROCKBLOX_LEFT          BUTTON_LEFT
#define ROCKBLOX_RIGHT         BUTTON_RIGHT
#define ROCKBLOX_DROP          BUTTON_SELECT
#define ROCKBLOX_RESTART       BUTTON_A

#elif CONFIG_KEYPAD == IRIVER_IFP7XX_PAD

#define ROCKBLOX_OFF           BUTTON_PLAY
#define ROCKBLOX_ROTATE_CCW    BUTTON_UP
#define ROCKBLOX_ROTATE_CW     BUTTON_SELECT
#define ROCKBLOX_DOWN          BUTTON_DOWN
#define ROCKBLOX_LEFT          BUTTON_LEFT
#define ROCKBLOX_RIGHT         BUTTON_RIGHT
#define ROCKBLOX_DROP          BUTTON_MODE
#define ROCKBLOX_RESTART       BUTTON_EQ

#elif CONFIG_KEYPAD == MROBE500_PAD
#define ROCKBLOX_OFF           BUTTON_POWER

#elif CONFIG_KEYPAD == GIGABEAT_S_PAD
#define ROCKBLOX_OFF           BUTTON_BACK
#define ROCKBLOX_ROTATE_CCW    BUTTON_VOL_DOWN
#define ROCKBLOX_ROTATE_CW     BUTTON_VOL_UP
#define ROCKBLOX_ROTATE        BUTTON_UP
#define ROCKBLOX_DOWN          BUTTON_DOWN
#define ROCKBLOX_LEFT          BUTTON_LEFT
#define ROCKBLOX_RIGHT         BUTTON_RIGHT
#define ROCKBLOX_DROP          BUTTON_SELECT
#define ROCKBLOX_RESTART       BUTTON_PLAY

#elif CONFIG_KEYPAD == MROBE100_PAD

#define ROCKBLOX_OFF           BUTTON_POWER
#define ROCKBLOX_ROTATE_CCW    BUTTON_MENU
#define ROCKBLOX_ROTATE_CW     BUTTON_PLAY
#define ROCKBLOX_ROTATE        BUTTON_UP
#define ROCKBLOX_DOWN          BUTTON_DOWN
#define ROCKBLOX_LEFT          BUTTON_LEFT
#define ROCKBLOX_RIGHT         BUTTON_RIGHT
#define ROCKBLOX_DROP          BUTTON_SELECT
#define ROCKBLOX_RESTART       BUTTON_DISPLAY

#elif CONFIG_KEYPAD == IAUDIO_M3_PAD

#define ROCKBLOX_OFF           BUTTON_RC_REC
#define ROCKBLOX_ROTATE_CCW    BUTTON_RC_VOL_DOWN
#define ROCKBLOX_ROTATE_CW     BUTTON_RC_VOL_UP
#define ROCKBLOX_DOWN          BUTTON_RC_MENU
#define ROCKBLOX_LEFT          BUTTON_RC_REW
#define ROCKBLOX_RIGHT         BUTTON_RC_FF
#define ROCKBLOX_DROP          BUTTON_RC_PLAY
#define ROCKBLOX_RESTART       BUTTON_RC_MODE

#elif CONFIG_KEYPAD == COWON_D2_PAD

#define ROCKBLOX_OFF           BUTTON_POWER
#define ROCKBLOX_RESTART       BUTTON_MENU

#elif CONFIG_KEYPAD == IAUDIO67_PAD

#define ROCKBLOX_OFF           BUTTON_POWER
#define ROCKBLOX_ROTATE_CCW    BUTTON_VOLDOWN
#define ROCKBLOX_ROTATE_CW     BUTTON_VOLUP
#define ROCKBLOX_DOWN          BUTTON_STOP
#define ROCKBLOX_LEFT          BUTTON_LEFT
#define ROCKBLOX_RIGHT         BUTTON_RIGHT
#define ROCKBLOX_DROP          BUTTON_PLAY
#define ROCKBLOX_RESTART       BUTTON_MENU

#elif CONFIG_KEYPAD == CREATIVEZVM_PAD
#define ROCKBLOX_OFF           BUTTON_BACK
#define ROCKBLOX_ROTATE_CCW    BUTTON_UP
#define ROCKBLOX_ROTATE_CW     BUTTON_PLAY
#define ROCKBLOX_DOWN          BUTTON_DOWN
#define ROCKBLOX_LEFT          BUTTON_LEFT
#define ROCKBLOX_RIGHT         BUTTON_RIGHT
#define ROCKBLOX_DROP          BUTTON_SELECT
#define ROCKBLOX_RESTART       BUTTON_CUSTOM

#elif CONFIG_KEYPAD == CREATIVE_ZENXFI3_PAD
#define ROCKBLOX_OFF           BUTTON_POWER
#define ROCKBLOX_ROTATE_CCW    BUTTON_VOL_DOWN
#define ROCKBLOX_ROTATE_CCW2   BUTTON_UP
#define ROCKBLOX_ROTATE_CW     BUTTON_VOL_UP
#define ROCKBLOX_DOWN          BUTTON_DOWN
#define ROCKBLOX_LEFT          BUTTON_BACK
#define ROCKBLOX_RIGHT         BUTTON_MENU
#define ROCKBLOX_DROP          (BUTTON_PLAY|BUTTON_REL)
#define ROCKBLOX_RESTART       (BUTTON_PLAY|BUTTON_REPEAT)

#elif CONFIG_KEYPAD == PHILIPS_HDD1630_PAD

#define ROCKBLOX_OFF           BUTTON_POWER
#define ROCKBLOX_ROTATE_CCW    BUTTON_VOL_DOWN
#define ROCKBLOX_ROTATE_CW     BUTTON_VOL_UP
#define ROCKBLOX_ROTATE        BUTTON_UP
#define ROCKBLOX_DOWN          BUTTON_DOWN
#define ROCKBLOX_LEFT          BUTTON_LEFT
#define ROCKBLOX_RIGHT         BUTTON_RIGHT
#define ROCKBLOX_DROP          BUTTON_SELECT
#define ROCKBLOX_RESTART       BUTTON_MENU

#elif CONFIG_KEYPAD == PHILIPS_HDD6330_PAD

#define ROCKBLOX_OFF           BUTTON_POWER
#define ROCKBLOX_ROTATE_CCW    BUTTON_PREV
#define ROCKBLOX_ROTATE_CW     BUTTON_NEXT
#define ROCKBLOX_ROTATE        BUTTON_UP
#define ROCKBLOX_DOWN          BUTTON_DOWN
#define ROCKBLOX_LEFT          BUTTON_LEFT
#define ROCKBLOX_RIGHT         BUTTON_RIGHT
#define ROCKBLOX_DROP          BUTTON_PLAY
#define ROCKBLOX_RESTART       BUTTON_MENU

# elif CONFIG_KEYPAD == PHILIPS_SA9200_PAD

#define ROCKBLOX_OFF           BUTTON_POWER
#define ROCKBLOX_ROTATE_CCW    BUTTON_VOL_DOWN
#define ROCKBLOX_ROTATE_CW     BUTTON_VOL_UP
#define ROCKBLOX_ROTATE        BUTTON_UP
#define ROCKBLOX_DOWN          BUTTON_DOWN
#define ROCKBLOX_LEFT          BUTTON_PREV
#define ROCKBLOX_RIGHT         BUTTON_NEXT
#define ROCKBLOX_DROP          BUTTON_PLAY
#define ROCKBLOX_RESTART       BUTTON_MENU

#elif CONFIG_KEYPAD == ONDAVX747_PAD
#define ROCKBLOX_OFF           BUTTON_POWER
#define ROCKBLOX_RESTART       BUTTON_MENU
#elif CONFIG_KEYPAD == ONDAVX777_PAD
#define ROCKBLOX_OFF           BUTTON_POWER

#elif CONFIG_KEYPAD == SAMSUNG_YH_PAD

#define ROCKBLOX_OFF           (BUTTON_REC|BUTTON_PLAY)
#define ROCKBLOX_ROTATE_CCW    BUTTON_UP
#define ROCKBLOX_ROTATE_CW     BUTTON_DOWN
#define ROCKBLOX_DOWN          BUTTON_REW
#define ROCKBLOX_LEFT          BUTTON_LEFT
#define ROCKBLOX_RIGHT         BUTTON_RIGHT
#define ROCKBLOX_DROP          BUTTON_FFWD
#define ROCKBLOX_RESTART       (BUTTON_REC|BUTTON_REW)

#elif CONFIG_KEYPAD == PBELL_VIBE500_PAD

#define ROCKBLOX_OFF           BUTTON_REC
#define ROCKBLOX_ROTATE_CCW    BUTTON_PLAY
#define ROCKBLOX_ROTATE_CW     BUTTON_MENU
#define ROCKBLOX_DOWN          BUTTON_DOWN
#define ROCKBLOX_LEFT          BUTTON_PREV
#define ROCKBLOX_RIGHT         BUTTON_NEXT
#define ROCKBLOX_DROP          BUTTON_OK
#define ROCKBLOX_RESTART       BUTTON_CANCEL

#elif CONFIG_KEYPAD == MPIO_HD200_PAD
#define ROCKBLOX_OFF           (BUTTON_REC|BUTTON_PLAY)
#define ROCKBLOX_ROTATE_CCW    BUTTON_REW
#define ROCKBLOX_ROTATE_CW     BUTTON_FF
#define ROCKBLOX_DOWN          BUTTON_FUNC
#define ROCKBLOX_LEFT          BUTTON_VOL_DOWN
#define ROCKBLOX_RIGHT         BUTTON_VOL_UP
#define ROCKBLOX_DROP          BUTTON_PLAY
#define ROCKBLOX_RESTART       BUTTON_REC

#elif CONFIG_KEYPAD == MPIO_HD300_PAD
#define ROCKBLOX_OFF           (BUTTON_MENU|BUTTON_REPEAT)
#define ROCKBLOX_ROTATE_CCW    BUTTON_UP
#define ROCKBLOX_ROTATE_CW     BUTTON_DOWN
#define ROCKBLOX_DOWN          (BUTTON_ENTER | BUTTON_REL)
#define ROCKBLOX_LEFT          BUTTON_REW
#define ROCKBLOX_RIGHT         BUTTON_FF
#define ROCKBLOX_DROP          BUTTON_PLAY
#define ROCKBLOX_RESTART       BUTTON_REC

#elif CONFIG_KEYPAD == SANSA_FUZEPLUS_PAD
#define ROCKBLOX_OFF           BUTTON_POWER
#define ROCKBLOX_ROTATE_CW     BUTTON_BOTTOMRIGHT
#define ROCKBLOX_ROTATE_CCW    BUTTON_BOTTOMLEFT
#define ROCKBLOX_DOWN          BUTTON_SELECT
#define ROCKBLOX_LEFT          BUTTON_LEFT
#define ROCKBLOX_RIGHT         BUTTON_RIGHT
#define ROCKBLOX_DROP_PRE      BUTTON_DOWN
#define ROCKBLOX_DROP          BUTTON_DOWN|BUTTON_REL
#define ROCKBLOX_ROTATE_CCW2   BUTTON_VOL_UP
#define ROCKBLOX_ROTATE_CW2    BUTTON_VOL_DOWN

#elif CONFIG_KEYPAD == SANSA_CONNECT_PAD
#define ROCKBLOX_OFF           BUTTON_POWER
#define ROCKBLOX_ROTATE_CW     BUTTON_NEXT
#define ROCKBLOX_ROTATE_CCW    BUTTON_PREV
#define ROCKBLOX_DOWN          BUTTON_DOWN
#define ROCKBLOX_LEFT          BUTTON_LEFT
#define ROCKBLOX_RIGHT         BUTTON_RIGHT
#define ROCKBLOX_DROP          BUTTON_SELECT
#define ROCKBLOX_RESTART       BUTTON_VOL_DOWN

#elif CONFIG_KEYPAD == SAMSUNG_YPR0_PAD

#define ROCKBLOX_OFF           BUTTON_BACK
#define ROCKBLOX_ROTATE_CCW    BUTTON_POWER
#define ROCKBLOX_ROTATE_CW     BUTTON_MENU
#define ROCKBLOX_ROTATE        BUTTON_UP
#define ROCKBLOX_DOWN          BUTTON_DOWN
#define ROCKBLOX_LEFT          BUTTON_LEFT
#define ROCKBLOX_RIGHT         BUTTON_RIGHT
#define ROCKBLOX_DROP          BUTTON_SELECT
#define ROCKBLOX_RESTART       BUTTON_USER

#elif CONFIG_KEYPAD == HM60X_PAD

#define ROCKBLOX_OFF           BUTTON_POWER
#define ROCKBLOX_ROTATE_CCW    (BUTTON_LEFT|BUTTON_POWER)
#define ROCKBLOX_ROTATE_CW     (BUTTON_RIGHT|BUTTON_POWER)
#define ROCKBLOX_ROTATE        BUTTON_UP
#define ROCKBLOX_DOWN          BUTTON_DOWN
#define ROCKBLOX_LEFT          BUTTON_LEFT
#define ROCKBLOX_RIGHT         BUTTON_RIGHT
#define ROCKBLOX_DROP          BUTTON_SELECT
#define ROCKBLOX_RESTART       (BUTTON_DOWN|BUTTON_POWER)

#elif CONFIG_KEYPAD == HM801_PAD

#define ROCKBLOX_OFF           BUTTON_POWER
#define ROCKBLOX_ROTATE_CCW    BUTTON_PREV
#define ROCKBLOX_ROTATE_CW     BUTTON_NEXT
#define ROCKBLOX_ROTATE        BUTTON_UP
#define ROCKBLOX_DOWN          BUTTON_DOWN
#define ROCKBLOX_LEFT          BUTTON_LEFT
#define ROCKBLOX_RIGHT         BUTTON_RIGHT
#define ROCKBLOX_DROP          BUTTON_SELECT
#define ROCKBLOX_RESTART       BUTTON_PLAY

#else
#error No keymap defined!
#endif

#ifdef HAVE_TOUCHSCREEN
#ifndef ROCKBLOX_OFF
#define ROCKBLOX_OFF           BUTTON_TOPLEFT
#endif
#ifdef ROCKBLOX_ROTATE_CCW
#define ROCKBLOX_ROTATE_CCW2  BUTTON_BOTTOMLEFT
#else
#define ROCKBLOX_ROTATE_CCW   BUTTON_BOTTOMLEFT
#endif
#ifdef ROCKBLOX_ROTATE_CW
#define ROCKBLOX_ROTATE_CW2   BUTTON_BOTTOMRIGHT
#else
#define ROCKBLOX_ROTATE_CW    BUTTON_BOTTOMRIGHT
#define ROCKBLOX_ROTATE_CW2   BUTTON_TOPMIDDLE
#endif
#ifndef ROCKBLOX_DOWN
#define ROCKBLOX_DOWN          BUTTON_BOTTOMMIDDLE
#endif
#ifndef ROCKBLOX_LEFT
#define ROCKBLOX_LEFT          BUTTON_MIDLEFT
#endif
#ifndef ROCKBLOX_RIGHT
#define ROCKBLOX_RIGHT         BUTTON_MIDRIGHT
#endif
#ifndef ROCKBLOX_DROP
#define ROCKBLOX_DROP          BUTTON_CENTER
#endif
#ifndef ROCKBLOX_RESTART
#define ROCKBLOX_RESTART       BUTTON_TOPRIGHT
#endif
#endif

#define BLOCKS_NUM  7
#define EMPTY_BLOCK 7

#define BOARD_WIDTH 10

#ifdef HAVE_LCD_BITMAP

#define BOARD_HEIGHT 20

#if (LCD_WIDTH == 640) && (LCD_HEIGHT == 480)

#define BLOCK_WIDTH 24
#define BLOCK_HEIGHT 24
#define BOARD_X 172
#define BOARD_Y 0
#define PREVIEW_X 24
#define PREVIEW_Y 22
#define LABEL_X 482
#define SCORE_Y 50
#define LEVEL_Y 140
#define LINES_Y 210

#elif (LCD_WIDTH == 480) && (LCD_HEIGHT == 640)

#define BLOCK_WIDTH 30
#define BLOCK_HEIGHT 30
#define BOARD_X 14
#define BOARD_Y 2
#define PREVIEW_X 342
#define PREVIEW_Y 482
#define LABEL_X 344
#define SCORE_Y 58
#define LEVEL_Y 142
#define LINES_Y 218

#elif (LCD_WIDTH == 320) && (LCD_HEIGHT == 240)

#define BLOCK_WIDTH 12
#define BLOCK_HEIGHT 12
#define BOARD_X 86
#define BOARD_Y 0
#define PREVIEW_X 12
#define PREVIEW_Y 11
#define LABEL_X 242
#define SCORE_Y 25
#define LEVEL_Y 70
#define LINES_Y 105

#elif (LCD_WIDTH == 240) && ((LCD_HEIGHT == 320) || (LCD_HEIGHT == 400))

#define BLOCK_WIDTH 15
#define BLOCK_HEIGHT 15
#define BOARD_X 7
#define BOARD_Y 1
#define PREVIEW_X 171
#define PREVIEW_Y 241
#define LABEL_X 172
#define SCORE_Y 29
#define LEVEL_Y 71
#define LINES_Y 109
#define HIGH_LABEL_X   172
#define HIGH_SCORE_Y   163
#define HIGH_LEVEL_Y   172

#elif (LCD_WIDTH == 220) && (LCD_HEIGHT == 176)

#define BLOCK_WIDTH 8
#define BLOCK_HEIGHT 8
#define BOARD_X 27
#define BOARD_Y 5
#define PREVIEW_X 158
#define PREVIEW_Y 130
#define LABEL_X 147
#define SCORE_Y 20
#define LEVEL_Y 65
#define LINES_Y 100

#elif (LCD_WIDTH == 176) && (LCD_HEIGHT == 132)

#define BLOCK_WIDTH 6
#define BLOCK_HEIGHT 6
#define BOARD_X 25
#define BOARD_Y 1
#define PREVIEW_X 126
#define PREVIEW_Y 102
#define LABEL_X 112
#define SCORE_Y 17
#define LEVEL_Y 49
#define LINES_Y 81

#elif (LCD_WIDTH == 176) && (LCD_HEIGHT == 220)

/* no room for the space in the highscore list */
#define HIGHSCORE_SPACE ""

#define BLOCK_WIDTH 10
#define BLOCK_HEIGHT 10
#define BOARD_X 6
#define BOARD_Y 10
#define PREVIEW_X 124
#define PREVIEW_Y 174
#define LABEL_X 117
#define SCORE_Y 18
#define LEVEL_Y 52
#define LINES_Y 85
#define HIGH_SCORE_Y 119
#define HIGH_LEVEL_Y 126
#define HIGH_LABEL_X 114

#elif (LCD_WIDTH == 160) && (LCD_HEIGHT == 128)


#define BLOCK_WIDTH 6
#define BLOCK_HEIGHT 6
#define BOARD_X 22
#define BOARD_Y 3
#define PREVIEW_X 114
#define PREVIEW_Y 100
#define LABEL_X 101
#define SCORE_Y 17
#define LEVEL_Y 49
#define LINES_Y 82

#elif (LCD_WIDTH == 138) && (LCD_HEIGHT == 110)

#define BLOCK_WIDTH 5
#define BLOCK_HEIGHT 5
#define BOARD_X 14
#define BOARD_Y 0
#define PREVIEW_X 98
#define PREVIEW_Y 88
#define LABEL_X 80
#define SCORE_Y 15
#define LEVEL_Y 45
#define LINES_Y 74

#elif (LCD_WIDTH == 132) && (LCD_HEIGHT == 80)

#define BLOCK_WIDTH 4
#define BLOCK_HEIGHT 4
#define BOARD_X 10
#define BOARD_Y 0
#define PREVIEW_X 89
#define PREVIEW_Y 61
#define LABEL_X 78
#define SCORE_Y 10
#define LEVEL_Y 30
#define LINES_Y 50

#elif (LCD_WIDTH == 128) && (LCD_HEIGHT == 128)

#define BLOCK_WIDTH 6
#define BLOCK_HEIGHT 6
#define BOARD_X 4
#define BOARD_Y 3
#define PREVIEW_X 84
#define PREVIEW_Y 100
#define LABEL_X 71
#define SCORE_Y 17
#define LEVEL_Y 49
#define LINES_Y 82

#elif (LCD_WIDTH == 128) && (LCD_HEIGHT == 160)

#define BLOCK_WIDTH 7
#define BLOCK_HEIGHT 7
#define BOARD_X 4
#define BOARD_Y 5
#define PREVIEW_X 88
#define PREVIEW_Y 111
#define LABEL_X 82
#define SCORE_Y (2+16)
#define LEVEL_Y (36+16)
#define LINES_Y (70+16)

#elif (LCD_WIDTH == 128) && (LCD_HEIGHT == 96)

#define BLOCK_WIDTH 4
#define BLOCK_HEIGHT 4
#define BOARD_X 14
#define BOARD_Y 2
#define PREVIEW_X 89
#define PREVIEW_Y 76
#define LABEL_X 70
#define SCORE_Y 14
#define LEVEL_Y 39
#define LINES_Y 64

#elif (LCD_WIDTH == 128) && (LCD_HEIGHT == 64)

#define BLOCK_WIDTH 3
#define BLOCK_HEIGHT 3
#define BOARD_X 9
#define BOARD_Y 3
#define PREVIEW_X 53
#define PREVIEW_Y 5
#define LABEL_X 70
#define SCORE_Y 32
#define LEVEL_Y 13
#define LINES_Y 51

#elif (LCD_WIDTH == 112) && (LCD_HEIGHT == 64)

#define BLOCK_WIDTH 4
#define BLOCK_HEIGHT 3
#define BOARD_X 9
#define BOARD_Y 3
#define PREVIEW_X 59
#define PREVIEW_Y 5
#define LABEL_X 59
#define SCORE_Y 32
#define LEVEL_Y 13
#define LEVEL_X 78
#define LINES_Y 51

#elif (LCD_WIDTH == 96) && (LCD_HEIGHT == 96)

#define BLOCK_WIDTH 4
#define BLOCK_HEIGHT 4
#define BOARD_X 5
#define BOARD_Y 4
#define PREVIEW_X 64
#define PREVIEW_Y 76
#define LABEL_X 59
#define SCORE_Y 15
#define SCORE_X 58
#define LEVEL_Y 39
#define LEVEL_X 58
#define LINES_Y 62
#define LINES_X 58
#endif

#ifndef LEVEL_X
#define LEVEL_X LABEL_X
#endif

#ifndef LINES_X
#define LINES_X LABEL_X
#endif

extern const fb_data rockblox_background[];

#else /* HAVE_LCD_CHARCELLS */

#define BOARD_HEIGHT 14

#define BLOCK_WIDTH  1
#define BLOCK_HEIGHT 1
#define BOARD_X      5
#define BOARD_Y      0
#define PREVIEW_X    15
#define PREVIEW_Y    1

#endif

#ifndef HIGHSCORE_SPACE
#define HIGHSCORE_SPACE " "
#endif
/* <<Explanation on Rockblox shapes>>

   %%
   %%  - O has 1 orientation

   %%   %
   %% %% - Z has 2 orientations
   %

   %% %
   %%  %% - S has 2 orientations
   %
   %
   %
   % %%%% - I has 2 orientations
   %

   %      %%
   %    %  % %%% - L has 4 orientations
   %% %%%  % %

    %     %%
    % %   %  %%% - J has 4 orientations
   %% %%% %    %

   %       % %%%
   %%  %  %%  %  - T has 4 orientations
   %  %%%  %
 */

/* c=current f=figure o=orientation n=next */
static struct _rockblox_status
{
    int gameover;
    int lines;
    int level;
    int score;
    int cx;
    int cy;
    int cf;
    int co;
    int nf;
    bool dropped;
    short board[BOARD_HEIGHT][BOARD_WIDTH]; /* 20 rows of 10 blocks */
} rockblox_status;

/* prototypes */
static void draw_next_block(void);
static void new_block(void);

#ifdef HAVE_SCROLLWHEEL
int wheel_events = 0, last_wheel_event = 0;
bool wheel_enabled = false;
#endif

static const short scoring[4] = {  /* scoring for each number of lines */
#if BOARD_HEIGHT == 20
    40 /* single */ , 100 /* double */ , 300 /* triple */ , 1200 /* rockblox */
#elif BOARD_HEIGHT == 14           /* Player special values */
    60 /* single */ , 150 /* double */ , 500 /* triple */ , 2000 /* rockblox */
#endif
};

struct figure
{
#if LCD_DEPTH >= 2
    unsigned short color[3];    /* color of figure (light,middle,shadow) */
#endif
    unsigned short max_or;      /* max orientations */
    signed short shapeX[4], shapeY[4];  /* implementation of figures */
}

/* array of figures */
figures[BLOCKS_NUM] = {
        /* O */
    {
#if LCD_DEPTH >= 16
        {LCD_RGBPACK (153, 255, 255), LCD_RGBPACK(0, 255, 255),
                LCD_RGBPACK(0,153,153)},
#elif LCD_DEPTH  == 2
        {LCD_WHITE, LCD_LIGHTGRAY, LCD_DARKGRAY},
#endif
        1,
        {-1, 0, -1, 0},
        {0, 0, 1, 1}
    },
        /* I */
    {
#if LCD_DEPTH >= 16
        {LCD_RGBPACK (255, 153, 128), LCD_RGBPACK (255, 0, 0),
                LCD_RGBPACK (153, 0, 0)},
#elif LCD_DEPTH  == 2
        {LCD_WHITE, LCD_LIGHTGRAY, LCD_DARKGRAY},
#endif
        2,
        {-2, -1, 0, 1},
        {0, 0, 0, 0}
    },
        /* 'Z' */
    {
#if LCD_DEPTH >= 16
        {LCD_RGBPACK (153, 255, 153), LCD_RGBPACK (0, 255, 0),
                LCD_RGBPACK (0, 153, 0)},
#elif LCD_DEPTH  == 2
        {LCD_WHITE, LCD_LIGHTGRAY, LCD_DARKGRAY},
#endif
        2,
        {0, 1, -1, 0},
        {0, 0, 1, 1}
    },
        /* 'S' */
    {
#if LCD_DEPTH >= 16
        {LCD_RGBPACK (153, 153, 255), LCD_RGBPACK (0, 0, 255),
                LCD_RGBPACK (0, 0, 153)},
#elif LCD_DEPTH  == 2
        {LCD_WHITE, LCD_LIGHTGRAY, LCD_DARKGRAY},
#endif
        2,
        {-1, 0, 0, 1},
        {0, 0, 1, 1}
    },
        /* 'L' */
    {
#if LCD_DEPTH >= 16
        {LCD_RGBPACK (255, 255, 153), LCD_RGBPACK (255, 255, 0),
                LCD_RGBPACK (153, 153, 0)},
#elif LCD_DEPTH  == 2
        {LCD_WHITE, LCD_LIGHTGRAY, LCD_DARKGRAY},
#endif
        4,
        {-1, 0, 1, 1},
        {0, 0, 0, 1}
    },
        /* 'J' */
    {
#if LCD_DEPTH >= 16
        {LCD_RGBPACK (255, 153, 255), LCD_RGBPACK (255, 0, 255),
                LCD_RGBPACK (153, 0, 153)},
#elif LCD_DEPTH  == 2
        {LCD_WHITE, LCD_LIGHTGRAY, LCD_DARKGRAY},
#endif
        4,
        {-1, 0, 1, -1},
        {0, 0, 0, 1}
    },
        /* 'T' */
    {
#if LCD_DEPTH >= 16
        {LCD_RGBPACK (204, 204, 204), LCD_RGBPACK (153, 153, 153),
                LCD_RGBPACK (85, 85, 85)},
#elif LCD_DEPTH  == 2
        {LCD_WHITE, LCD_LIGHTGRAY, LCD_DARKGRAY},
#endif
        4,
        {-1, 0, 1, 0},
        {0, 0, 0, 1}
    }
};
bool resume = false;
bool resume_file = false;

/* Rockbox File System only supports full filenames inc dir */
#define SCORE_FILE  PLUGIN_GAMES_DATA_DIR "/rockblox.score"
#define RESUME_FILE PLUGIN_GAMES_DATA_DIR "/rockblox.resume"
#define NUM_SCORES  5

/* Default High Scores... */
struct highscore highscores[NUM_SCORES];

/* get random number from (0) to (range-1) */
static int t_rand (int range)
{
    return rb->rand () % range;
}

static inline void show_game_over (void)
{
    rb->splash(HZ,"Game over!");
}

/* init the board array to have no blocks */
static void init_board (void)
{
    int i, j;
    for (i = 0; i < BOARD_WIDTH; i++)
        for (j = 0; j < BOARD_HEIGHT; j++)
            rockblox_status.board[j][i] = EMPTY_BLOCK;
}

/* show the score, level and lines */
static void show_details (void)
{
#ifdef HAVE_LCD_BITMAP
#if LCD_DEPTH >= 2
    rb->lcd_set_foreground (LCD_BLACK);
    rb->lcd_set_background (LCD_WHITE);
#endif
    rb->lcd_putsxyf (LABEL_X, SCORE_Y, "%d", rockblox_status.score);
    rb->lcd_putsxyf (LEVEL_X, LEVEL_Y, "%d", rockblox_status.level);
    rb->lcd_putsxyf (LINES_X, LINES_Y, "%d", rockblox_status.lines);
#else  /* HAVE_LCD_CHARCELLS */
    rb->lcd_putsf (5, 0, "L%d/%d", rockblox_status.level,
            rockblox_status.lines);
    rb->lcd_putsf (5, 1, "S%d", rockblox_status.score);
#endif
}

#ifdef HIGH_SCORE_Y
static void show_highscores (void)
{
    int i;

    for (i = 0; i<NUM_SCORES; i++)
        rb->lcd_putsxyf (HIGH_LABEL_X, HIGH_SCORE_Y + (10 * i),
            "%06d" HIGHSCORE_SPACE "L%1d", highscores[i].score, highscores[i].level);
}
#endif

static void load_game(void)
{
    int fd;

    resume = false;

    fd = rb->open(RESUME_FILE, O_RDONLY);
    if (fd < 0) return;

    if (rb->read(fd, &rockblox_status, sizeof(struct _rockblox_status))
            < (ssize_t)sizeof(struct _rockblox_status))
    {
        rb->splash(HZ/2, "Loading Rockblox resume info failed");
    } else {
        resume = true;
    }

    rb->close(fd);

    return;
}

static void dump_resume(void)
{
    int fd;

    fd = rb->open(RESUME_FILE, O_WRONLY|O_CREAT, 0666);
    if (fd < 0)
        goto fail;

    if (rb->write(fd, &rockblox_status, sizeof(struct _rockblox_status))
            <= 0)
    {
        rb->close(fd);
        goto fail;
    }
    rb->close(fd);
    return;

fail:
    rb->splash(HZ/2, "Writing Rockblox resume info failed");
    return;
}

static void init_rockblox (bool resume)
{
    char score_name[50];
    struct tm* tm;

    tm = rb->get_time();
    rb->snprintf(score_name, sizeof(score_name), "%04d%02d%02d %02d%02d%02d",
            tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
            tm->tm_hour, tm->tm_min, tm->tm_sec);

#ifdef HAVE_LCD_BITMAP
    rb->lcd_bitmap (rockblox_background, 0, 0, LCD_WIDTH, LCD_HEIGHT);
#else  /* HAVE_LCD_CHARCELLS */
    pgfx_display (0, 0);
    pgfx_display_block (3, 0, 3, 1);
    pgfx_display_block (4, 0, 3, 0);
    rb->lcd_puts(4, 1, " ");
    pgfx_clear_display();
    pgfx_fillrect (3, 0, 2, 14);
    pgfx_fillrect (15, 7, 2, 7);
    pgfx_update();
#endif
    if (!resume)
    {
        rockblox_status.level = 1;
        rockblox_status.lines = 0;
        rockblox_status.score = 0;
        rockblox_status.nf = t_rand(BLOCKS_NUM);
        init_board ();
        new_block ();
    }
    draw_next_block();

    show_details ();
#ifdef HIGH_SCORE_Y
    show_highscores ();
#endif
}

static inline int level_speed(int level)
{
#if BOARD_HEIGHT == 20
    return (5*HZ) / (level + 9);
#elif BOARD_HEIGHT == 14
    return (7*HZ) / (level + 9);
#endif
}

static int getRelativeX (int figure, int square, int orientation)
{
    switch (orientation) {
        case 0:
            return figures[figure].shapeX[square];
        case 1:
            return figures[figure].shapeY[square];
        case 2:
            return -figures[figure].shapeX[square];
        case 3:
            return -figures[figure].shapeY[square];
        default:
            return 0;
    }
}

static int getRelativeY (int figure, int square, int orientation)
{
    switch (orientation) {
        case 0:
            return figures[figure].shapeY[square];
        case 1:
            return -figures[figure].shapeX[square];
        case 2:
            return -figures[figure].shapeY[square];
        case 3:
            return figures[figure].shapeX[square];
        default:
            return 0;
    }
}

/* redraw the while board on the screen */
static void refresh_board (void)
{
    int i, j, x, y, block;

#if LCD_DEPTH >= 2
    rb->lcd_set_foreground (LCD_BLACK);
#elif LCD_DEPTH == 1
    mylcd_set_drawmode (DRMODE_SOLID | DRMODE_INVERSEVID);
#endif

    mylcd_fillrect (BOARD_X, BOARD_Y, BOARD_WIDTH * BLOCK_WIDTH,
                    BOARD_HEIGHT * BLOCK_HEIGHT);

#if LCD_DEPTH == 1
    mylcd_set_drawmode (DRMODE_SOLID);
#endif

    for (i = 0; i < BOARD_WIDTH; i++)
        for (j = 0; j < BOARD_HEIGHT; j++) {
            block = rockblox_status.board[j][i];
            if (block != EMPTY_BLOCK) {
#ifdef HAVE_LCD_BITMAP
#if LCD_DEPTH >= 2
                /* middle drawing */
                rb->lcd_set_foreground (figures[block].color[1]);
#endif
                rb->lcd_fillrect (BOARD_X + i * BLOCK_WIDTH,
                                  BOARD_Y + j * BLOCK_HEIGHT,
                                  BLOCK_WIDTH, BLOCK_HEIGHT);
#if LCD_DEPTH >= 2
                /* light drawing */
                rb->lcd_set_foreground (figures[block].color[0]);
#endif
                rb->lcd_vline (BOARD_X + i * BLOCK_WIDTH,
                               BOARD_Y + j * BLOCK_HEIGHT,
                               BOARD_Y + (j + 1) * BLOCK_HEIGHT - 2);
                rb->lcd_hline (BOARD_X + i * BLOCK_WIDTH,
                               BOARD_X + (i + 1) * BLOCK_WIDTH - 2,
                               BOARD_Y + j * BLOCK_HEIGHT);
#if LCD_DEPTH >= 2
                /* shadow drawing */
                rb->lcd_set_foreground (figures[block].color[2]);
#endif
                rb->lcd_vline (BOARD_X + (i + 1) * BLOCK_WIDTH - 1,
                               BOARD_Y + j * BLOCK_HEIGHT + 1,
                               BOARD_Y + (j + 1) * BLOCK_HEIGHT - 1);
                rb->lcd_hline (BOARD_X + i * BLOCK_WIDTH + 1,
                               BOARD_X + (i + 1) * BLOCK_WIDTH - 1,
                               BOARD_Y + (j + 1) * BLOCK_HEIGHT - 1);
#else  /* HAVE_LCD_CHARCELLS */
                pgfx_drawpixel (BOARD_X + i, BOARD_Y + j);
#endif
            }
        }

    for (i = 0; i < 4; i++) {
        x = getRelativeX (rockblox_status.cf, i, rockblox_status.co)
                + rockblox_status.cx;
        y = getRelativeY (rockblox_status.cf, i, rockblox_status.co)
                + rockblox_status.cy;
#ifdef HAVE_LCD_BITMAP
#if LCD_DEPTH >= 2
        /* middle drawing */
        rb->lcd_set_foreground (figures[rockblox_status.cf].color[1]);
#endif
        rb->lcd_fillrect (BOARD_X + x * BLOCK_WIDTH,
                          BOARD_Y + y * BLOCK_HEIGHT,
                          BLOCK_WIDTH, BLOCK_HEIGHT);
#if LCD_DEPTH >= 2
        /* light drawing */
        rb->lcd_set_foreground (figures[rockblox_status.cf].color[0]);
#endif
        rb->lcd_vline (BOARD_X + x * BLOCK_WIDTH, BOARD_Y + y * BLOCK_HEIGHT,
                       BOARD_Y + (y + 1) * BLOCK_HEIGHT - 2);
        rb->lcd_hline (BOARD_X + x * BLOCK_WIDTH,
                       BOARD_X + (x + 1) * BLOCK_WIDTH - 2,
                       BOARD_Y + y * BLOCK_HEIGHT);
#if LCD_DEPTH >= 2
        /* shadow drawing */
        rb->lcd_set_foreground (figures[rockblox_status.cf].color[2]);
#endif
        rb->lcd_vline (BOARD_X + (x + 1) * BLOCK_WIDTH - 1,
                       BOARD_Y + y * BLOCK_HEIGHT + 1,
                       BOARD_Y + (y + 1) * BLOCK_HEIGHT - 1);
        rb->lcd_hline (BOARD_X + x * BLOCK_WIDTH + 1,
                       BOARD_X + (x + 1) * BLOCK_WIDTH - 1,
                       BOARD_Y + (y + 1) * BLOCK_HEIGHT - 1);
#else /* HAVE_LCD_CHARCELLS */
        pgfx_drawpixel (BOARD_X + x, BOARD_Y + y);
#endif
    }
    mylcd_update ();
}

static bool canMoveTo (int x, int y, int newOrientation)
{
    int i, rx, ry;
    for (i = 0; i < 4; i++) {
        ry = getRelativeY (rockblox_status.cf, i, newOrientation) + y;
        rx = getRelativeX (rockblox_status.cf, i, newOrientation) + x;
        if ((rx < 0 || rx >= BOARD_WIDTH) ||
            (ry < 0 || ry >= BOARD_HEIGHT) ||
            (rockblox_status.board[ry][rx] != EMPTY_BLOCK))
            return false;
    }
    return true;
}

/* draws the preview of next block in the preview window */
static void draw_next_block (void)
{
    int i, rx, ry;
    /* clear preview window first */
#if LCD_DEPTH >= 2
    rb->lcd_set_foreground (LCD_BLACK);
#elif LCD_DEPTH == 1
    mylcd_set_drawmode (DRMODE_SOLID | DRMODE_INVERSEVID);
#endif

    /* 4x4 */
    mylcd_fillrect (PREVIEW_X, PREVIEW_Y, BLOCK_WIDTH * 4, BLOCK_HEIGHT * 4);

#if LCD_DEPTH == 1
    mylcd_set_drawmode (DRMODE_SOLID);
#endif

    /* draw the lightgray rectangles */
#if LCD_DEPTH >= 16
    rb->lcd_set_foreground (LCD_RGBPACK (40, 40, 40));
#elif LCD_DEPTH == 2
    rb->lcd_set_foreground (LCD_DARKGRAY);
#endif

#if LCD_DEPTH >= 2
    for (rx = 0; rx < 4; rx++)
        for (ry = 0; ry < 4; ry++)
            rb->lcd_drawrect (PREVIEW_X + rx * BLOCK_WIDTH,
                              PREVIEW_Y + ry * BLOCK_HEIGHT, BLOCK_WIDTH,
                              BLOCK_HEIGHT);
#endif

    /* draw the figure */
    for (i = 0; i < 4; i++) {
        rx = getRelativeX (rockblox_status.nf, i, 0) + 2;
        ry = getRelativeY (rockblox_status.nf, i, 0) + 2;
#ifdef HAVE_LCD_BITMAP
#if LCD_DEPTH >= 2
        rb->lcd_set_foreground (figures[rockblox_status.nf].color[1]);  /* middle drawing */
#endif
        rb->lcd_fillrect (PREVIEW_X + rx * BLOCK_WIDTH,
                          PREVIEW_Y + ry * BLOCK_HEIGHT,
                          BLOCK_WIDTH, BLOCK_HEIGHT);
#if LCD_DEPTH >= 2
        rb->lcd_set_foreground (figures[rockblox_status.nf].color[0]);  /* light drawing */
#endif
        rb->lcd_vline (PREVIEW_X + rx * BLOCK_WIDTH,
                       PREVIEW_Y + ry * BLOCK_HEIGHT,
                       PREVIEW_Y + (ry + 1) * BLOCK_HEIGHT - 2);
        rb->lcd_hline (PREVIEW_X + rx * BLOCK_WIDTH,
                       PREVIEW_X + (rx + 1) * BLOCK_WIDTH - 2,
                       PREVIEW_Y + ry * BLOCK_HEIGHT);
#if LCD_DEPTH >= 2
        rb->lcd_set_foreground (figures[rockblox_status.nf].color[2]);  /* shadow drawing */
#endif
        rb->lcd_vline (PREVIEW_X + (rx + 1) * BLOCK_WIDTH - 1,
                       PREVIEW_Y + ry * BLOCK_HEIGHT + 1,
                       PREVIEW_Y + (ry + 1) * BLOCK_HEIGHT - 1);
        rb->lcd_hline (PREVIEW_X + rx * BLOCK_WIDTH + 1,
                       PREVIEW_X + (rx + 1) * BLOCK_WIDTH - 1,
                       PREVIEW_Y + (ry + 1) * BLOCK_HEIGHT - 1);
#else /* HAVE_LCD_CHARCELLS */
        pgfx_drawpixel (PREVIEW_X + rx, PREVIEW_Y + ry);
#endif
    }

}

/* move the block to a relative location */
static void move_block (int x, int y, int o)
{
    if (!rockblox_status.dropped && canMoveTo (rockblox_status.cx + x, rockblox_status.cy + y, o)) {
        rockblox_status.cy += y;
        rockblox_status.cx += x;
        rockblox_status.co = o;
    }
}

/* try to add a new block to play with (return true if gameover) */
static void new_block (void)
{
    rockblox_status.cy = 1;
    rockblox_status.cx = 5;
    rockblox_status.cf = rockblox_status.nf;
    rockblox_status.co = 0;                     /* start at the same orientation all time */
    rockblox_status.nf = t_rand (BLOCKS_NUM);
    rockblox_status.gameover = !canMoveTo (rockblox_status.cx,
            rockblox_status.cy, rockblox_status.co);
    rockblox_status.dropped = false;

    draw_next_block ();
}


/* check for filled rockblox_status.lines and do what necessary */
static int check_lines (void)

{
    int i, j, y;
    int rockblox = 0;

    for (j = 0; j < BOARD_HEIGHT; j++) {
        for (i = 0; ((i < BOARD_WIDTH) &&
                (rockblox_status.board[j][i] != EMPTY_BLOCK)); i++);
        if (i == BOARD_WIDTH) { /* woo hoo, we have a line */
            rockblox++;
            for (y = j; y > 0; y--)
            {
                for (i = 0; i < BOARD_WIDTH; i++)
                {   /* fall line */
                    rockblox_status.board[y][i] = rockblox_status.board[y - 1][i];
                }
            }
        }
    }

    return rockblox;
}

/* moves down the figure and returns true if gameover */
static void move_down (void)
{
    int l, i, rx, ry;

    if (!canMoveTo (rockblox_status.cx, rockblox_status.cy + 1, rockblox_status.co)) {
        /* save figure to board */
        for (i = 0; i < 4; i++) {
            rx = getRelativeX (rockblox_status.cf, i, rockblox_status.co) + rockblox_status.cx;
            ry = getRelativeY (rockblox_status.cf, i, rockblox_status.co) + rockblox_status.cy;
            rockblox_status.board[ry][rx] = rockblox_status.cf;
        }
        /* check if formed some lines */
        l = check_lines ();
        if (l) {
            /* the original scoring from "http://en.wikipedia.org/wiki/Rockblox" */
            rockblox_status.score += scoring[l - 1] * rockblox_status.level;
            rockblox_status.lines += l;
            rockblox_status.level = (int) rockblox_status.lines / 10 + 1;
        }

        /* show details */
        show_details ();

        /* generate a new figure */
        new_block ();
    } else
        move_block (0, 1, rockblox_status.co);
}

static bool rockblox_help(void)
{
    static char *help_text[] = {
        "Rockblox", "", "Aim", "",
        "Make", "the", "falling", "blocks", "of",
        "different", "shapes", "form", "full", "rows.",
        "Whenever", "a", "row", "is", "completed,",
        "it", "will", "be", "cleared", "away",
        "and", "you", "gain", "points."
    };
    static struct style_text formation[]={
        { 0, TEXT_CENTER|TEXT_UNDERLINE },
        { 2, C_RED },
        LAST_STYLE_ITEM
    };

#ifdef HAVE_LCD_BITMAP
    rb->lcd_setfont(FONT_UI);
#endif
#ifdef HAVE_LCD_COLOR
    rb->lcd_set_background(LCD_BLACK);
    rb->lcd_set_foreground(LCD_WHITE);
#endif
    if (display_text(ARRAYLEN(help_text), help_text, formation, NULL, true))
        return true;
#ifdef HAVE_LCD_BITMAP
    rb->lcd_setfont(FONT_SYSFIXED);
#endif

    return false;
}

static int rockblox_menu_cb(int action, const struct menu_item_ex *this_item)
{
    int i = ((intptr_t)this_item);
    if(action == ACTION_REQUEST_MENUITEM
       && !resume && (i==0 || i==5))
        return ACTION_EXIT_MENUITEM;
    return action;
}

static int rockblox_menu(void)
{
    int selected = 0;

    MENUITEM_STRINGLIST(main_menu, "Rockblox Menu", rockblox_menu_cb,
                        "Resume Game", "Start New Game",
                        "Help", "High Scores", "Playback Control",
                        "Quit without Saving", "Quit");

    rb->button_clear_queue();
    while (true) {
        switch (rb->do_menu(&main_menu, &selected, NULL, false)) {
            case 0:
                if(resume_file)
                    rb->remove(RESUME_FILE);
                init_rockblox(true);
                return 0;
            case 1:
                init_rockblox(false);
                return 0;
            case 2:
                if (rockblox_help())
                    return 1;
                break;
            case 3:
                highscore_show(-1, highscores, NUM_SCORES, true);
                break;
            case 4:
                if (playback_control(NULL))
                    return 1;
                break;
            case 5:
                return 1;
            case 6:
                if (resume) {
                    rb->splash(HZ*1, "Saving game ...");
                    dump_resume();
                }
                return 1;
            case MENU_ATTACHED_USB:
                return 1;
            default:
                return 1;
                break;
        }
    }
}


static int rockblox_loop (void)
{
    int button;
#if defined(ROCKBLOX_OFF_PRE) || defined(ROCKBLOX_DROP_PRE)
    int lastbutton = BUTTON_NONE;
#endif
    long next_down_tick = *rb->current_tick + level_speed(rockblox_status.level);

    if (rockblox_menu()) {
        return 1;
    }
    resume = false;
    resume_file = false;

    while (1) {
#ifdef HAS_BUTTON_HOLD
        if (rb->button_hold ()) {
            /* Turn on backlight timeout (revert to settings) */
            backlight_use_settings();
            rb->splash(0, "Paused");
            while (rb->button_hold ())
                rb->sleep(HZ/10);

            /* Turn off backlight timeout */
            backlight_ignore_timeout();

            /* get rid of the splash text */
            rb->lcd_bitmap (rockblox_background, 0, 0, LCD_WIDTH, LCD_HEIGHT);
            show_details ();
#ifdef HIGH_SCORE_Y
            show_highscores ();
#endif
            draw_next_block ();
            refresh_board ();
        }
#endif

        button = rb->button_get_w_tmo (MAX(next_down_tick - *rb->current_tick, 1));
        switch (button) {
#ifdef ROCKBLOX_RC_OFF
            case ROCKBLOX_RC_OFF:
#endif
            case ROCKBLOX_OFF:
#ifdef ROCKBLOX_OFF_PRE
                if (lastbutton != ROCKBLOX_OFF_PRE)
                    break;
#endif
                resume = true;
                return 0;
                break;

#if defined(ROCKBLOX_ROTATE)
            case ROCKBLOX_ROTATE:
#endif
            case ROCKBLOX_ROTATE_CCW:
            case ROCKBLOX_ROTATE_CCW | BUTTON_REPEAT:
#ifdef HAVE_SCROLLWHEEL
                /* if the wheel is disabled, add an event to the stack. */
                if(wheel_enabled == false)
                    wheel_events++;

                /* if it's enabled, go ahead and rotate.. */
                if(wheel_enabled)
#endif
#ifdef ROCKBLOX_ROTATE_CCW2
                /* fallback */
            case ROCKBLOX_ROTATE_CCW2:
#endif
                move_block (0, 0, (rockblox_status.co + 1) % figures[rockblox_status.cf].max_or);
                break;

            case ROCKBLOX_ROTATE_CW:
            case ROCKBLOX_ROTATE_CW | BUTTON_REPEAT:
#ifdef HAVE_SCROLLWHEEL
                if(wheel_enabled == false)
                    wheel_events++;

                if(wheel_enabled)
#endif
#ifdef ROCKBLOX_ROTATE_CW2
                /* fallback */
            case ROCKBLOX_ROTATE_CW2:
#endif
                move_block (0, 0,
                            (rockblox_status.co + figures[rockblox_status.cf].max_or -
                             1) % figures[rockblox_status.cf].max_or);
                break;

            case ROCKBLOX_DOWN:
            case ROCKBLOX_DOWN | BUTTON_REPEAT:
                move_block (0, 1, rockblox_status.co);
                break;

            case ROCKBLOX_RIGHT:
            case ROCKBLOX_RIGHT | BUTTON_REPEAT:
                move_block (1, 0, rockblox_status.co);
                break;

            case ROCKBLOX_LEFT:
            case ROCKBLOX_LEFT | BUTTON_REPEAT:
                move_block (-1, 0, rockblox_status.co);
                break;

            case ROCKBLOX_DROP:
#ifdef ROCKBLOX_DROP_PRE
                if (lastbutton != ROCKBLOX_DROP_PRE)
                    break;
#endif
                while (canMoveTo (rockblox_status.cx, rockblox_status.cy + 1, rockblox_status.co))
                    move_block (0, 1, rockblox_status.co);
                rockblox_status.dropped = true;
                break;
#ifdef ROCKBLOX_RESTART
            case ROCKBLOX_RESTART:
                rb->splash (HZ * 1, "Restarting...");
                init_rockblox (false);
                break;
#endif

            default:
                if (rb->default_event_handler (button) == SYS_USB_CONNECTED)
                    return PLUGIN_USB_CONNECTED;
                break;
        }
#if defined(ROCKBLOX_OFF_PRE) || defined(ROCKBLOX_DROP_PRE)
        if (button != BUTTON_NONE)
            lastbutton = button;
#endif

#ifdef HAVE_SCROLLWHEEL
        /* check if we should enable the scroll wheel, if events
         * begin to stack up... */
        if(wheel_enabled == false)
        {
            /* stopped rotating the wheel, reset the count */
            if(wheel_events == last_wheel_event)
            {
                last_wheel_event = 0;
                wheel_events = 0;
            }
            /* rotated the wheel a while constantly, enable it. */
            else if(wheel_events > 3)
            {
                wheel_enabled = true;
            }

            /* this evens out the last event and the "current" event.
             * if we get an event next time through button reading, it will
             * remain ahead of last_event. if we don't, they'll end up equaling
             * each other.. thus, the scroll count will be reset. */
            if(wheel_enabled == false && wheel_events > last_wheel_event)
                last_wheel_event++;
        }
#endif

        if (TIME_AFTER(*rb->current_tick, next_down_tick)) {
            move_down ();
            next_down_tick += level_speed(rockblox_status.level);
            if (TIME_AFTER(*rb->current_tick, next_down_tick))
                /* restart time "raster" when we had to wait longer than usual
                 * (pause, game restart etc) */
                next_down_tick = *rb->current_tick + level_speed(rockblox_status.level);
        }

        if (rockblox_status.gameover) {
#if LCD_DEPTH >= 2
            rb->lcd_set_foreground (LCD_BLACK);
#endif
            show_game_over();
            resume = false;
            return 0;
        }

        refresh_board ();
    }

    return 0;
}

enum plugin_status plugin_start (const void *parameter)
{

    (void) parameter;

    rb->srand (*rb->current_tick);

    /* Load HighScore if any */
    highscore_load(SCORE_FILE, highscores, NUM_SCORES);

#if LCD_DEPTH > 1
    rb->lcd_set_backdrop(NULL);
#endif

#ifdef HAVE_LCD_BITMAP
    rb->lcd_setfont (FONT_SYSFIXED);
#else
    if (!pgfx_init(4, 2))
    {
        rb->splash(HZ*2, "Old LCD :(");
        return PLUGIN_OK;
    }
#endif
    /* Turn off backlight timeout */
    backlight_ignore_timeout();
    load_game();
    resume_file = resume;
    while(!rockblox_loop()) {
        if(!resume) {
            int position = highscore_update(rockblox_status.score,
                                            rockblox_status.level, "",
                                            highscores, NUM_SCORES);
            if (position != -1) {
                if (position == 0)
                    rb->splash(HZ*2, "New High Score");
                highscore_show(position, highscores, NUM_SCORES, true);
            }
        }
    }

#ifndef HAVE_LCD_BITMAP
    pgfx_release();
#endif
    /* Save user's HighScore */
    highscore_save(SCORE_FILE, highscores, NUM_SCORES);
    backlight_use_settings();

    return PLUGIN_OK;
}
