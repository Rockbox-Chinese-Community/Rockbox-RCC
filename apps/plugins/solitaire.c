/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2004-2007 Antoine Cellerier <dionoea @t videolan d.t org>
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
#include "lib/configfile.h"
#include "lib/display_text.h"
#include "button.h"
#include "lcd.h"

#define min(a,b) (a<b?a:b)

/**
 * Key definitions
 */

#if CONFIG_KEYPAD == RECORDER_PAD
#   define SOL_QUIT         BUTTON_OFF
#   define SOL_UP           BUTTON_UP
#   define SOL_DOWN         BUTTON_DOWN
#   define SOL_LEFT         BUTTON_LEFT
#   define SOL_RIGHT        BUTTON_RIGHT
#   define SOL_MOVE         BUTTON_ON
#   define SOL_DRAW         BUTTON_F1
#   define SOL_REM2CUR      BUTTON_PLAY
#   define SOL_CUR2STACK    BUTTON_F2
#   define SOL_REM2STACK    BUTTON_F3
#   define HK_MOVE         "ON"
#   define HK_DRAW         "F1"
#   define HK_REM2CUR      "PLAY"
#   define HK_CUR2STACK    "F2"
#   define HK_REM2STACK    "F3"

#elif CONFIG_KEYPAD == ARCHOS_AV300_PAD
#   define SOL_QUIT         BUTTON_OFF
#   define SOL_UP           BUTTON_UP
#   define SOL_DOWN         BUTTON_DOWN
#   define SOL_LEFT         BUTTON_LEFT
#   define SOL_RIGHT        BUTTON_RIGHT
#   define SOL_MOVE         BUTTON_ON
#   define SOL_DRAW         BUTTON_F1
#   define SOL_REM2CUR      BUTTON_SELECT
#   define SOL_CUR2STACK    BUTTON_F2
#   define SOL_REM2STACK    BUTTON_F3
#   define HK_MOVE         "ON"
#   define HK_DRAW         "F1"
#   define HK_REM2CUR      "SELECT"
#   define HK_CUR2STACK    "F2"
#   define HK_REM2STACK    "F3"

#elif CONFIG_KEYPAD == ONDIO_PAD
#   define SOL_QUIT         BUTTON_OFF
#   define SOL_UP_PRE       BUTTON_UP
#   define SOL_UP           (BUTTON_UP | BUTTON_REL)
#   define SOL_DOWN_PRE     BUTTON_DOWN
#   define SOL_DOWN         (BUTTON_DOWN | BUTTON_REL)
#   define SOL_LEFT_PRE     BUTTON_LEFT
#   define SOL_LEFT         (BUTTON_LEFT | BUTTON_REL)
#   define SOL_RIGHT_PRE    BUTTON_RIGHT
#   define SOL_RIGHT        (BUTTON_RIGHT | BUTTON_REL)
#   define SOL_MOVE_PRE     BUTTON_MENU
#   define SOL_MOVE         (BUTTON_MENU | BUTTON_REL)
#   define SOL_DRAW_PRE     BUTTON_MENU
#   define SOL_DRAW         (BUTTON_MENU | BUTTON_REPEAT)
#   define SOL_REM2CUR_PRE  BUTTON_DOWN
#   define SOL_REM2CUR      (BUTTON_DOWN | BUTTON_REPEAT)
#   define SOL_CUR2STACK_PRE BUTTON_UP
#   define SOL_CUR2STACK    (BUTTON_UP | BUTTON_REPEAT)
#   define SOL_REM2STACK_PRE BUTTON_RIGHT
#   define SOL_REM2STACK    (BUTTON_RIGHT | BUTTON_REPEAT)
#   define HK_MOVE         "MODE"
#   define HK_DRAW         "MODE.."
#   define HK_REM2CUR      "DOWN.."
#   define HK_CUR2STACK    "UP.."
#   define HK_REM2STACK    "RIGHT.."

#elif (CONFIG_KEYPAD == IRIVER_H100_PAD) || \
      (CONFIG_KEYPAD == IRIVER_H300_PAD)
#   define SOL_QUIT         BUTTON_OFF
#   define SOL_UP           BUTTON_UP
#   define SOL_DOWN         BUTTON_DOWN
#   define SOL_LEFT         BUTTON_LEFT
#   define SOL_RIGHT        BUTTON_RIGHT
#   define SOL_MOVE_PRE     BUTTON_SELECT
#   define SOL_MOVE         (BUTTON_SELECT | BUTTON_REL)
#   define SOL_DRAW         BUTTON_MODE
#   define SOL_REM2CUR      (BUTTON_LEFT | BUTTON_ON)
#   define SOL_CUR2STACK_PRE BUTTON_SELECT
#   define SOL_CUR2STACK    (BUTTON_SELECT | BUTTON_REPEAT)
#   define SOL_REM2STACK    (BUTTON_RIGHT | BUTTON_ON)
#   define SOL_REM          BUTTON_REC
#   define SOL_RC_QUIT      BUTTON_RC_STOP
#   define HK_MOVE         "NAVI"
#   define HK_DRAW         "A-B"
#   define HK_REM2CUR      "PLAY+LEFT"
#   define HK_CUR2STACK    "NAVI.."
#   define HK_REM2STACK    "PLAY+RIGHT"

#elif (CONFIG_KEYPAD == IPOD_4G_PAD) || (CONFIG_KEYPAD == IPOD_3G_PAD) || \
      (CONFIG_KEYPAD == IPOD_1G2G_PAD)
#   define SOL_QUIT         (BUTTON_SELECT | BUTTON_MENU)
#   define SOL_UP           BUTTON_SCROLL_BACK
#   define SOL_DOWN         BUTTON_SCROLL_FWD
#   define SOL_LEFT_PRE     BUTTON_LEFT
#   define SOL_LEFT         (BUTTON_LEFT | BUTTON_REL)
#   define SOL_RIGHT_PRE    BUTTON_RIGHT
#   define SOL_RIGHT        (BUTTON_RIGHT | BUTTON_REL)
#   define SOL_MOVE_PRE     BUTTON_SELECT
#   define SOL_MOVE         (BUTTON_SELECT | BUTTON_REL)
#   define SOL_DRAW_PRE     BUTTON_MENU
#   define SOL_DRAW         (BUTTON_MENU | BUTTON_REL)
#   define SOL_REM2CUR      BUTTON_PLAY
#   define SOL_CUR2STACK_PRE BUTTON_MENU
#   define SOL_CUR2STACK    (BUTTON_MENU | BUTTON_REPEAT)
#   define SOL_REM2STACK_PRE BUTTON_RIGHT
#   define SOL_REM2STACK    (BUTTON_RIGHT | BUTTON_REPEAT)
#   define HK_UD           "SCROLL U/D"
#   define HK_MOVE         "SELECT"
#   define HK_DRAW         "MENU"
#   define HK_REM2CUR      "PLAY"
#   define HK_CUR2STACK    "MENU.."
#   define HK_REM2STACK    "RIGHT.."

#elif (CONFIG_KEYPAD == IAUDIO_X5M5_PAD)
#   define SOL_QUIT          BUTTON_POWER
#   define SOL_UP            BUTTON_UP
#   define SOL_DOWN          BUTTON_DOWN
#   define SOL_LEFT          BUTTON_LEFT
#   define SOL_RIGHT         BUTTON_RIGHT
#   define SOL_MOVE_PRE      BUTTON_SELECT
#   define SOL_MOVE          (BUTTON_SELECT | BUTTON_REL)
#   define SOL_DRAW_PRE      BUTTON_PLAY
#   define SOL_DRAW          (BUTTON_PLAY | BUTTON_REL)
#   define SOL_REM2CUR_PRE   BUTTON_PLAY
#   define SOL_REM2CUR       (BUTTON_PLAY | BUTTON_REPEAT)
#   define SOL_CUR2STACK_PRE BUTTON_SELECT
#   define SOL_CUR2STACK     (BUTTON_SELECT | BUTTON_REPEAT)
#   define SOL_REM2STACK     BUTTON_REC
#   define HK_MOVE           "SELECT"
#   define HK_DRAW           "PLAY"
#   define HK_REM2CUR        "PLAY.."
#   define HK_CUR2STACK      "SELECT.."
#   define HK_REM2STACK      "REC"

#elif (CONFIG_KEYPAD == GIGABEAT_PAD)
#   define SOL_QUIT         BUTTON_POWER
#   define SOL_UP           BUTTON_UP
#   define SOL_DOWN         BUTTON_DOWN
#   define SOL_LEFT         BUTTON_LEFT
#   define SOL_RIGHT        BUTTON_RIGHT
#   define SOL_MOVE_PRE     BUTTON_SELECT
#   define SOL_MOVE         (BUTTON_SELECT | BUTTON_REL)
#   define SOL_DRAW         BUTTON_MENU
#   define SOL_REM2CUR      (BUTTON_LEFT | BUTTON_A)
#   define SOL_CUR2STACK    (BUTTON_SELECT | BUTTON_REPEAT)
#   define SOL_REM2STACK    (BUTTON_RIGHT | BUTTON_A)
#   define HK_MOVE         "SELECT"
#   define HK_DRAW         "MENU"
#   define HK_REM2CUR      "A+LEFT"
#   define HK_CUR2STACK    "SELECT.."
#   define HK_REM2STACK    "A+RIGHT"

#elif (CONFIG_KEYPAD == SANSA_E200_PAD)
#   define SOL_QUIT          BUTTON_POWER
#   define SOL_UP            BUTTON_UP
#   define SOL_DOWN          BUTTON_DOWN
#   define SOL_LEFT          BUTTON_SCROLL_BACK
#   define SOL_RIGHT         BUTTON_SCROLL_FWD
#   define SOL_MOVE          BUTTON_SELECT
#   define SOL_DRAW          BUTTON_REC
#   define SOL_REM2CUR       BUTTON_LEFT
#   define SOL_CUR2STACK_PRE BUTTON_REC
#   define SOL_CUR2STACK     (BUTTON_REC | BUTTON_RIGHT)
#   define SOL_REM2STACK     BUTTON_RIGHT
#   define HK_MOVE           "SELECT"
#   define HK_DRAW           "REC"
#   define HK_REM2CUR        "LEFT"
#   define HK_CUR2STACK      "DOUBLE SELECT"
#   define HK_REM2STACK      "RIGHT"

#elif (CONFIG_KEYPAD == SANSA_FUZE_PAD)
#   define SOL_QUIT          (BUTTON_HOME|BUTTON_REPEAT)
#   define SOL_UP            BUTTON_UP
#   define SOL_DOWN          BUTTON_DOWN
#   define SOL_LEFT          BUTTON_SCROLL_BACK
#   define SOL_RIGHT         BUTTON_SCROLL_FWD
#   define SOL_MOVE          (BUTTON_SELECT|BUTTON_REL)
#   define SOL_DRAW          (BUTTON_HOME|BUTTON_REL)
#   define SOL_REM2CUR       BUTTON_LEFT
#   define SOL_CUR2STACK_PRE (BUTTON_RIGHT | BUTTON_REPEAT)
#   define SOL_CUR2STACK     BUTTON_RIGHT
#   define SOL_REM2STACK     (BUTTON_LEFT|BUTTON_REPEAT)
#   define SOL_REM2STACK_PRE BUTTON_LEFT
#   define HK_MOVE           "SHORT SELECT"
#   define HK_DRAW           "SHORT HOME"
#   define HK_REM2CUR        "LEFT"
#   define HK_CUR2STACK      "DOUBLE SELECT"
#   define HK_REM2STACK      "LEFT"

#elif (CONFIG_KEYPAD == SANSA_C200_PAD)
#   define SOL_QUIT          BUTTON_POWER
#   define SOL_UP            BUTTON_UP
#   define SOL_DOWN          BUTTON_DOWN
#   define SOL_LEFT          BUTTON_LEFT
#   define SOL_RIGHT         BUTTON_RIGHT
#   define SOL_MOVE_PRE      BUTTON_SELECT
#   define SOL_MOVE          (BUTTON_SELECT | BUTTON_REL)
#   define SOL_DRAW          BUTTON_VOL_DOWN
#   define SOL_REM2CUR       BUTTON_REC
#   define SOL_CUR2STACK_PRE BUTTON_SELECT
#   define SOL_CUR2STACK     (BUTTON_SELECT | BUTTON_REPEAT)
#   define SOL_REM2STACK     BUTTON_VOL_UP
#   define HK_MOVE           "SELECT"
#   define HK_DRAW           "REC"
#   define HK_REM2CUR        "LEFT"
#   define HK_CUR2STACK      "DOUBLE SELECT"
#   define HK_REM2STACK      "RIGHT"

#elif CONFIG_KEYPAD == SANSA_CLIP_PAD
#   define SOL_QUIT          BUTTON_POWER
#   define SOL_UP            BUTTON_UP
#   define SOL_DOWN          BUTTON_DOWN
#   define SOL_LEFT          BUTTON_LEFT
#   define SOL_RIGHT         BUTTON_RIGHT
#   define SOL_MOVE_PRE      BUTTON_SELECT
#   define SOL_MOVE          (BUTTON_SELECT | BUTTON_REL)
#   define SOL_DRAW          BUTTON_HOME
#   define SOL_REM2CUR       BUTTON_VOL_DOWN
#   define SOL_CUR2STACK_PRE BUTTON_SELECT
#   define SOL_CUR2STACK     (BUTTON_SELECT | BUTTON_REPEAT)
#   define SOL_REM2STACK     BUTTON_VOL_UP
#   define HK_MOVE           "SELECT"
#   define HK_DRAW           "HOME"
#   define HK_REM2CUR        "LEFT"
#   define HK_CUR2STACK      "DOUBLE SELECT"
#   define HK_REM2STACK      "RIGHT"

#elif CONFIG_KEYPAD == SANSA_M200_PAD
#   define SOL_QUIT          BUTTON_POWER
#   define SOL_UP            BUTTON_UP
#   define SOL_DOWN          BUTTON_DOWN
#   define SOL_LEFT          BUTTON_LEFT
#   define SOL_RIGHT         BUTTON_RIGHT
#   define SOL_MOVE_PRE      BUTTON_SELECT
#   define SOL_MOVE          (BUTTON_SELECT | BUTTON_REL)
#   define SOL_DRAW          (BUTTON_SELECT | BUTTON_UP)
#   define SOL_REM2CUR       BUTTON_VOL_DOWN
#   define SOL_CUR2STACK_PRE BUTTON_SELECT
#   define SOL_CUR2STACK     (BUTTON_SELECT | BUTTON_REPEAT)
#   define SOL_REM2STACK     BUTTON_VOL_UP
#   define HK_MOVE           "SELECT"
#   define HK_DRAW           "SELECT + UP"
#   define HK_REM2CUR        "LEFT"
#   define HK_CUR2STACK      "DOUBLE SELECT"
#   define HK_REM2STACK      "RIGHT"

#elif (CONFIG_KEYPAD == IRIVER_H10_PAD)
#   define SOL_QUIT          BUTTON_POWER
#   define SOL_UP            BUTTON_SCROLL_UP
#   define SOL_DOWN          BUTTON_SCROLL_DOWN
#   define SOL_LEFT_PRE      BUTTON_LEFT
#   define SOL_LEFT          (BUTTON_LEFT | BUTTON_REL)
#   define SOL_RIGHT_PRE     BUTTON_RIGHT
#   define SOL_RIGHT         (BUTTON_RIGHT | BUTTON_REL)
#   define SOL_MOVE          BUTTON_PLAY
#   define SOL_DRAW_PRE      BUTTON_LEFT
#   define SOL_DRAW          (BUTTON_LEFT | BUTTON_REPEAT)
#   define SOL_REM2CUR       BUTTON_FF
#   define SOL_CUR2STACK     BUTTON_REW
#   define SOL_REM2STACK_PRE BUTTON_RIGHT
#   define SOL_REM2STACK     (BUTTON_RIGHT | BUTTON_REPEAT)
#   define HK_MOVE           "PLAY"
#   define HK_DRAW           "LEFT.."
#   define HK_REM2CUR        "FF"
#   define HK_CUR2STACK      "REW"
#   define HK_REM2STACK      "RIGHT.."

#elif (CONFIG_KEYPAD == GIGABEAT_S_PAD)
#   define SOL_QUIT         BUTTON_BACK
#   define SOL_UP           BUTTON_UP
#   define SOL_DOWN         BUTTON_DOWN
#   define SOL_LEFT         BUTTON_LEFT
#   define SOL_RIGHT        BUTTON_RIGHT
#   define SOL_MOVE_PRE     BUTTON_SELECT
#   define SOL_MOVE         (BUTTON_SELECT | BUTTON_REL)
#   define SOL_DRAW         BUTTON_MENU
#   define SOL_REM2CUR      (BUTTON_LEFT | BUTTON_SELECT)
#   define SOL_CUR2STACK    (BUTTON_SELECT | BUTTON_REPEAT)
#   define SOL_REM2STACK    (BUTTON_RIGHT | BUTTON_SELECT)
#   define HK_MOVE         "SELECT"
#   define HK_DRAW         "MENU"
#   define HK_REM2CUR      "SELECT+LEFT"
#   define HK_CUR2STACK    "SELECT.."
#   define HK_REM2STACK    "SELECT+RIGHT"

#elif (CONFIG_KEYPAD == MROBE100_PAD)
#   define SOL_QUIT         BUTTON_POWER
#   define SOL_UP           BUTTON_UP
#   define SOL_DOWN         BUTTON_DOWN
#   define SOL_LEFT         BUTTON_LEFT
#   define SOL_RIGHT        BUTTON_RIGHT
#   define SOL_MOVE_PRE     BUTTON_SELECT
#   define SOL_MOVE         (BUTTON_SELECT | BUTTON_REL)
#   define SOL_DRAW         BUTTON_MENU
#   define SOL_REM2CUR      (BUTTON_LEFT | BUTTON_DISPLAY)
#   define SOL_CUR2STACK    (BUTTON_SELECT | BUTTON_REPEAT)
#   define SOL_REM2STACK    (BUTTON_RIGHT | BUTTON_DISPLAY)
#   define HK_MOVE         "SELECT"
#   define HK_DRAW         "MENU"
#   define HK_REM2CUR      "DISPLAY+LEFT"
#   define HK_CUR2STACK    "SELECT.."
#   define HK_REM2STACK    "DISPLAY+RIGHT"

#elif CONFIG_KEYPAD == IAUDIO_M3_PAD
#   define SOL_QUIT         BUTTON_RC_REC
#   define SOL_UP           BUTTON_RC_VOL_UP
#   define SOL_DOWN         BUTTON_RC_VOL_DOWN
#   define SOL_LEFT         BUTTON_RC_REW
#   define SOL_RIGHT        BUTTON_RC_FF
#   define SOL_MOVE         BUTTON_RC_PLAY
#   define SOL_DRAW_PRE     BUTTON_RC_MENU
#   define SOL_DRAW         (BUTTON_RC_MENU | BUTTON_REL)
#   define SOL_REM2CUR_PRE  BUTTON_RC_MENU
#   define SOL_REM2CUR      (BUTTON_RC_MENU | BUTTON_REPEAT)
#   define SOL_CUR2STACK_PRE BUTTON_RC_MODE
#   define SOL_CUR2STACK    (BUTTON_RC_MODE | BUTTON_REL)
#   define SOL_REM2STACK_PRE BUTTON_RC_MODE
#   define SOL_REM2STACK    (BUTTON_RC_MODE | BUTTON_REPEAT)
#   define HK_LR           "REW/FF"
#   define HK_UD           "VOL UP/DOWN"
#   define HK_MOVE         "PLAY"
#   define HK_DRAW         "MENU"
#   define HK_REM2CUR      "MENU.."
#   define HK_CUR2STACK    "MODE"
#   define HK_REM2STACK    "MODE.."

#elif (CONFIG_KEYPAD == COWON_D2_PAD)
#   define SOL_QUIT          BUTTON_POWER

#elif CONFIG_KEYPAD == CREATIVEZVM_PAD
#   define SOL_QUIT         BUTTON_BACK
#   define SOL_UP           BUTTON_UP
#   define SOL_DOWN         BUTTON_DOWN
#   define SOL_LEFT         BUTTON_LEFT
#   define SOL_RIGHT        BUTTON_RIGHT
#   define SOL_MOVE_PRE     BUTTON_SELECT
#   define SOL_MOVE         (BUTTON_SELECT | BUTTON_REL)
#   define SOL_DRAW         BUTTON_MENU
#   define SOL_REM2CUR      (BUTTON_LEFT | BUTTON_SELECT)
#   define SOL_CUR2STACK    (BUTTON_SELECT | BUTTON_REPEAT)
#   define SOL_REM2STACK    (BUTTON_RIGHT | BUTTON_SELECT)
#   define HK_MOVE         "MIDDLE"
#   define HK_DRAW         "MENU"
#   define HK_REM2CUR      "PLAY+LEFT"
#   define HK_CUR2STACK    "PLAY.."
#   define HK_REM2STACK    "PLAY+RIGHT"

#elif (CONFIG_KEYPAD == CREATIVE_ZENXFI3_PAD)
#   define SOL_QUIT          BUTTON_POWER
#   define SOL_UP            BUTTON_UP
#   define SOL_DOWN          BUTTON_DOWN
#   define SOL_LEFT          BUTTON_BACK
#   define SOL_RIGHT         BUTTON_MENU
#   define SOL_MOVE          (BUTTON_PLAY|BUTTON_REL)
#   define SOL_DRAW          (BUTTON_PLAY|BUTTON_REPEAT)
#   define SOL_REM2CUR       BUTTON_VOL_DOWN
#   define SOL_CUR2STACK_PRE (BUTTON_VOL_UP | BUTTON_REPEAT)
#   define SOL_CUR2STACK     BUTTON_VOL_UP
#   define SOL_REM2STACK     (BUTTON_VOL_DOWN|BUTTON_REPEAT)
#   define SOL_REM2STACK_PRE BUTTON_VOL_DOWN

#   define HK_MOVE           "SHORT PLAY"
#   define HK_DRAW           "LONG PLAY"
#   define HK_REM2CUR        "VOLUME-"
#   define HK_CUR2STACK      "VOLUME+"
#   define HK_REM2STACK      "LONG VOLUME-"

#elif CONFIG_KEYPAD == PHILIPS_HDD1630_PAD
#   define SOL_QUIT         BUTTON_POWER
#   define SOL_UP           BUTTON_UP
#   define SOL_DOWN         BUTTON_DOWN
#   define SOL_LEFT         BUTTON_LEFT
#   define SOL_RIGHT        BUTTON_RIGHT
#   define SOL_MOVE_PRE     BUTTON_SELECT
#   define SOL_MOVE         (BUTTON_SELECT | BUTTON_REL)
#   define SOL_DRAW         BUTTON_MENU
#   define SOL_REM2CUR      (BUTTON_LEFT | BUTTON_VIEW)
#   define SOL_CUR2STACK    (BUTTON_SELECT | BUTTON_REPEAT)
#   define SOL_REM2STACK    (BUTTON_RIGHT | BUTTON_VIEW)
#   define HK_MOVE          "SELECT"
#   define HK_DRAW          "MENU"
#   define HK_REM2CUR       "VIEW+LEFT"
#   define HK_CUR2STACK     "SELECT.."
#   define HK_REM2STACK     "VIEW+RIGHT"

#elif CONFIG_KEYPAD == PHILIPS_HDD6330_PAD
#   define SOL_QUIT          BUTTON_POWER
#   define SOL_UP            BUTTON_UP
#   define SOL_DOWN          BUTTON_DOWN
#   define SOL_LEFT          BUTTON_PREV
#   define SOL_RIGHT         BUTTON_NEXT
#   define SOL_MOVE_PRE      BUTTON_PLAY
#   define SOL_MOVE          (BUTTON_PLAY | BUTTON_REL)
#   define SOL_DRAW          BUTTON_MENU
#   define SOL_REM2CUR       BUTTON_LEFT
#   define SOL_CUR2STACK     (BUTTON_PLAY | BUTTON_REPEAT)
#   define SOL_REM2STACK     BUTTON_RIGHT
#   define HK_MOVE           "PLAY"
#   define HK_DRAW           "MENU"
#   define HK_REM2CUR        "LEFT"
#   define HK_CUR2STACK      "PLAY.."
#   define HK_REM2STACK      "RIGHT"

#elif CONFIG_KEYPAD == PHILIPS_SA9200_PAD
#   define SOL_QUIT          BUTTON_POWER
#   define SOL_UP            BUTTON_UP
#   define SOL_DOWN          BUTTON_DOWN
#   define SOL_LEFT          BUTTON_PREV
#   define SOL_RIGHT         BUTTON_NEXT
#   define SOL_MOVE_PRE      BUTTON_PLAY
#   define SOL_MOVE          (BUTTON_PLAY | BUTTON_REL)
#   define SOL_DRAW          BUTTON_MENU
#   define SOL_REM2CUR       BUTTON_LEFT
#   define SOL_CUR2STACK     (BUTTON_PLAY | BUTTON_REPEAT)
#   define SOL_REM2STACK     BUTTON_RIGHT
#   define HK_MOVE           "PLAY"
#   define HK_DRAW           "MENU"
#   define HK_REM2CUR        "LEFT"
#   define HK_CUR2STACK      "PLAY..."
#   define HK_REM2STACK      "RIGHT"

#elif (CONFIG_KEYPAD == ONDAVX747_PAD) || \
(CONFIG_KEYPAD == ONDAVX777_PAD) || \
CONFIG_KEYPAD == MROBE500_PAD
#   define SOL_QUIT          BUTTON_POWER

#elif (CONFIG_KEYPAD == SAMSUNG_YH_PAD)
#   define SOL_QUIT          BUTTON_REC
#   define SOL_UP            BUTTON_UP
#   define SOL_DOWN          BUTTON_DOWN
#   define SOL_LEFT_PRE      BUTTON_LEFT
#   define SOL_LEFT          (BUTTON_LEFT | BUTTON_REW)
#   define SOL_RIGHT_PRE     BUTTON_RIGHT
#   define SOL_RIGHT         (BUTTON_RIGHT | BUTTON_REW)
#   define SOL_MOVE          BUTTON_PLAY
#   define SOL_DRAW_PRE      (BUTTON_PLAY | BUTTON_RIGHT)
#   define SOL_DRAW          (BUTTON_PLAY | BUTTON_LEFT)
#   define SOL_REM2CUR       BUTTON_FFWD
#   define SOL_CUR2STACK     BUTTON_REW
#   define SOL_REM2STACK_PRE (BUTTON_FFWD | BUTTON_RIGHT)
#   define SOL_REM2STACK     (BUTTON_FFWD | BUTTON_LEFT)
#   define HK_MOVE           "PLAY"
#   define HK_DRAW           "REW.."
#   define HK_REM2CUR        "FFWD"
#   define HK_CUR2STACK      "REW"
#   define HK_REM2STACK      "FFWD.."

#elif CONFIG_KEYPAD == PBELL_VIBE500_PAD
#   define SOL_QUIT          BUTTON_REC
#   define SOL_UP            BUTTON_UP
#   define SOL_DOWN          BUTTON_DOWN
#   define SOL_LEFT          BUTTON_PREV
#   define SOL_RIGHT         BUTTON_NEXT
#   define SOL_MOVE_PRE      BUTTON_OK
#   define SOL_MOVE          (BUTTON_OK | BUTTON_REL)
#   define SOL_DRAW          BUTTON_MENU
#   define SOL_REM2CUR       BUTTON_CANCEL
#   define SOL_CUR2STACK     BUTTON_PLAY
#   define SOL_REM2STACK     (BUTTON_PLAY | BUTTON_REPEAT)
#   define HK_MOVE           "OK"
#   define HK_DRAW           "MENU"
#   define HK_REM2CUR        "CANCEL"
#   define HK_CUR2STACK      "PLAY"
#   define HK_REM2STACK      "PLAY..."

#elif CONFIG_KEYPAD == MPIO_HD200_PAD
#   define SOL_QUIT         (BUTTON_REC | BUTTON_PLAY)
#   define SOL_UP           BUTTON_REW
#   define SOL_DOWN         BUTTON_FF
#   define SOL_LEFT         BUTTON_VOL_DOWN
#   define SOL_RIGHT        BUTTON_VOL_UP
#   define SOL_MOVE_PRE     BUTTON_FUNC
#   define SOL_MOVE         (BUTTON_FUNC | BUTTON_REL)
#   define SOL_DRAW         BUTTON_REC
#   define SOL_REM2CUR      (BUTTON_REC | BUTTON_REPEAT)
#   define SOL_CUR2STACK    BUTTON_PLAY
#   define SOL_REM2STACK    (BUTTON_PLAY | BUTTON_REPEAT)
#   define HK_MOVE         "FUNC"
#   define HK_DRAW         "REC"
#   define HK_REM2CUR      "REC.."
#   define HK_CUR2STACK    "PLAY"
#   define HK_REM2STACK    "PLAY...."

#elif CONFIG_KEYPAD == MPIO_HD300_PAD
#   define SOL_QUIT         (BUTTON_MENU | BUTTON_REPEAT)
#   define SOL_UP           BUTTON_UP
#   define SOL_DOWN         BUTTON_DOWN
#   define SOL_LEFT         BUTTON_REW
#   define SOL_RIGHT        BUTTON_FF
#   define SOL_MOVE_PRE     BUTTON_ENTER
#   define SOL_MOVE         (BUTTON_ENTER | BUTTON_REL)
#   define SOL_DRAW         BUTTON_MENU
#   define SOL_REM2CUR      (BUTTON_PLAY | BUTTON_REL)
#   define SOL_CUR2STACK    BUTTON_REC
#   define SOL_REM2STACK    (BUTTON_PLAY | BUTTON_REPEAT)
#   define HK_MOVE         "ENTER"
#   define HK_DRAW         "MENU"
#   define HK_REM2CUR      "PLAY"
#   define HK_CUR2STACK    "ENTER..."
#   define HK_REM2STACK    "PLAY...."

#elif CONFIG_KEYPAD == SANSA_FUZEPLUS_PAD
#   define SOL_QUIT          BUTTON_POWER
#   define SOL_UP            BUTTON_UP
#   define SOL_DOWN          BUTTON_DOWN
#   define SOL_LEFT          BUTTON_LEFT
#   define SOL_RIGHT         BUTTON_RIGHT
#   define SOL_MOVE          BUTTON_SELECT
#   define SOL_DRAW          BUTTON_BACK
#   define SOL_REM2CUR       BUTTON_BOTTOMLEFT
#   define SOL_CUR2STACK     BUTTON_PLAYPAUSE|BUTTON_REL
#   define SOL_REM2STACK     BUTTON_PLAYPAUSE|BUTTON_REPEAT
#   define HK_MOVE           "SELECT"
#   define HK_DRAW           "BACK"
#   define HK_REM2CUR        "BOTTOM-LEFT"
#   define HK_CUR2STACK      "PLAY-PAUSE"
#   define HK_REM2STACK      "BOTTOM-RIGHT"

#elif (CONFIG_KEYPAD == SANSA_CONNECT_PAD)
#   define SOL_QUIT          BUTTON_POWER
#   define SOL_UP            BUTTON_UP
#   define SOL_DOWN          BUTTON_DOWN
#   define SOL_LEFT          BUTTON_SCROLL_BACK
#   define SOL_RIGHT         BUTTON_SCROLL_FWD
#   define SOL_MOVE          BUTTON_SELECT
#   define SOL_DRAW          BUTTON_VOL_UP
#   define SOL_REM2CUR       BUTTON_LEFT
#   define SOL_CUR2STACK_PRE BUTTON_VOL_DOWN
#   define SOL_CUR2STACK     BUTTON_NEXT
#   define SOL_REM2STACK     BUTTON_PREV
#   define HK_MOVE           "SELECT"
#   define HK_DRAW           "Vol+"
#   define HK_REM2CUR        "LEFT"
#   define HK_CUR2STACK      "NEXT"
#   define HK_REM2STACK      "PREV"

#elif (CONFIG_KEYPAD == SAMSUNG_YPR0_PAD)
#   define SOL_QUIT         BUTTON_BACK
#   define SOL_UP           BUTTON_UP
#   define SOL_DOWN         BUTTON_DOWN
#   define SOL_LEFT         BUTTON_LEFT
#   define SOL_RIGHT        BUTTON_RIGHT
#   define SOL_MOVE_PRE     BUTTON_SELECT
#   define SOL_MOVE         (BUTTON_SELECT | BUTTON_REL)
#   define SOL_DRAW         BUTTON_MENU
#   define SOL_REM2CUR      (BUTTON_USER | BUTTON_REPEAT)
#   define SOL_CUR2STACK    (BUTTON_SELECT | BUTTON_REPEAT)
#   define SOL_REM2STACK    BUTTON_POWER
#   define HK_MOVE         "Select"
#   define HK_DRAW         "Menu"
#   define HK_REM2CUR      "Long User"
#   define HK_CUR2STACK    "Long Select.."
#   define HK_REM2STACK    "Power"

#elif (CONFIG_KEYPAD == HM60X_PAD)
#   define SOL_QUIT         BUTTON_POWER
#   define SOL_UP           BUTTON_UP
#   define SOL_DOWN         BUTTON_DOWN
#   define SOL_LEFT         BUTTON_LEFT
#   define SOL_RIGHT        BUTTON_RIGHT
#   define SOL_MOVE_PRE     BUTTON_SELECT
#   define SOL_MOVE         (BUTTON_POWER | BUTTON_SELECT)
#   define SOL_DRAW         (BUTTON_POWER | BUTTON_UP)
#   define SOL_REM2CUR      (BUTTON_POWER | BUTTON_DOWN)
#   define SOL_CUR2STACK    (BUTTON_POWER | BUTTON_LEFT)
#   define SOL_REM2STACK    (BUTTON_POWER | BUTTON_RIGHT)
#   define HK_MOVE         "SELECT + POWER"
#   define HK_DRAW         "UP + POWER"
#   define HK_REM2CUR      "DOWN + POWER"
#   define HK_CUR2STACK    "LEFT + POWER"
#   define HK_REM2STACK    "RIGHT + POWER"

#elif (CONFIG_KEYPAD == HM801_PAD)
#   define SOL_QUIT         BUTTON_POWER
#   define SOL_UP           BUTTON_UP
#   define SOL_DOWN         BUTTON_DOWN
#   define SOL_LEFT         BUTTON_LEFT
#   define SOL_RIGHT        BUTTON_RIGHT
#   define SOL_MOVE_PRE     BUTTON_PREV
#   define SOL_MOVE         BUTTON_NEXT
#   define SOL_DRAW         BUTTON_PLAY
#   define SOL_REM2CUR      BUTTON_SELECT
#   define SOL_CUR2STACK    (BUTTON_POWER | BUTTON_LEFT)
#   define SOL_REM2STACK    (BUTTON_POWER | BUTTON_RIGHT)
#   define HK_MOVE         "PREV"
#   define HK_DRAW         "PLAY"
#   define HK_REM2CUR      "SELECT"
#   define HK_CUR2STACK    "POWER + LEFT"
#   define HK_REM2STACK    "POWER + RIGHT"

#else
#error No keymap defined!
#endif

#ifdef HAVE_TOUCHSCREEN
//#ifndef SOL_QUIT
//#   define SOL_QUIT          BUTTON_TOPLEFT
//endif
#ifndef SOL_UP
#   define SOL_UP            BUTTON_TOPMIDDLE
#endif
#ifndef SOL_DOWN
#   define SOL_DOWN          BUTTON_BOTTOMMIDDLE
#endif
#ifndef SOL_LEFT
#   define SOL_LEFT          BUTTON_MIDLEFT
#endif
#ifndef SOL_RIGHT
#   define SOL_RIGHT         BUTTON_MIDRIGHT
#endif
#ifndef SOL_MOVE
#   define SOL_MOVE          BUTTON_CENTER
#   define HK_MOVE           "CENTRE"
#endif
#ifndef SOL_DRAW
#   define SOL_DRAW          BUTTON_TOPLEFT
#   define HK_DRAW           "TOPLEFT"
#endif
#ifndef SOL_REM2CUR
#   define SOL_REM2CUR       BUTTON_TOPRIGHT
#   define HK_REM2CUR        "TOPRIGHT"
#endif
#ifndef SOL_CUR2STACK
#   define SOL_CUR2STACK     BUTTON_BOTTOMLEFT
#   define HK_CUR2STACK      "BOTTOMLEFT"
#endif
#ifndef SOL_REM2STACK
#   define SOL_REM2STACK     BUTTON_BOTTOMRIGHT
#   define HK_REM2STACK      "BOTTOMRIGHT"
#endif

#endif

#ifndef HK_LR
#   define HK_LR "LEFT/RIGHT"
#endif
#ifndef HK_UD
#   define HK_UD "UP/DOWN"
#endif

/**
 * Misc constants, graphics and other defines
 */

#include "pluginbitmaps/card_back.h"
#include "pluginbitmaps/card_deck.h"
#include "pluginbitmaps/solitaire_suitsi.h"

#define CARD_GFX_WIDTH  BMPWIDTH_card_back
#define CARD_GFX_HEIGHT BMPHEIGHT_card_back
#define CARD_WIDTH      (BMPWIDTH_card_back+2)
#define CARD_HEIGHT     (BMPHEIGHT_card_back+2)

#if LCD_WIDTH >= 640
#   define MARGIN 4
#   define LARGE_CARD
#   define SYMBOL_HEIGHT 24

#elif LCD_WIDTH >= 320
#   define MARGIN 4
#   define LARGE_CARD
#   define SYMBOL_HEIGHT 12
#elif LCD_WIDTH >= 220
#   define MARGIN 3
#   define LARGE_CARD
#   define SYMBOL_HEIGHT 12
#elif LCD_WIDTH >= 160
#   define MARGIN 2
#   define SYMBOL_HEIGHT 11
#elif LCD_WIDTH >= 128
#   define MARGIN 1
#   define SYMBOL_HEIGHT 10
#else
#   define MARGIN 0
#   define SYMBOL_HEIGHT 8
#endif

#define CARD_START (CARD_HEIGHT+2*MARGIN+1)

/* background color */
#ifdef HAVE_LCD_COLOR
#   define BACKGROUND_COLOR LCD_RGBPACK(0,157,0)
#   define FRAME_COLOR      LCD_RGBPACK(23,119,218)
#elif LCD_DEPTH > 1
#   define BACKGROUND_COLOR LCD_WHITE
#   define FRAME_COLOR      LCD_BLACK
#endif


#define CONFIG_FILENAME "sol.cfg"

#define NOT_A_CARD -1

/* number of cards per suit */
#define CARDS_PER_SUIT 13

/* number of suits */
#define SUITS 4

#define NUM_CARDS ( CARDS_PER_SUIT * SUITS )

/* number of columns */
#define COL_NUM 7

/* pseudo column numbers to be used for cursor coordinates */
/* columns COL_NUM to COL_NUM + SUITS - 1 correspond to the final stacks */
#define STACKS_COL COL_NUM
/* column COL_NUM + SUITS corresponds to the remains' stack */
#define REM_COL (STACKS_COL + SUITS)

#define NOT_A_COL -1

#if defined(SOL_LEFT_PRE)      || defined(SOL_RIGHT_PRE)     || \
    defined(SOL_DOWN_PRE)      || defined(SOL_UP_PRE)        || \
    defined(SOL_CUR2STACK_PRE) || defined(SOL_MOVE_PRE)      || \
    defined(SOL_REM2CUR_PRE)   || defined(SOL_REM2STACK_PRE) || \
    defined(SOL_DRAW_PRE)
#   define NEED_LASTBUTTON_VAR
#endif

typedef struct
{
    signed char suit;
    signed char num;
    bool known : 1;
    bool used : 1; /* this is what is used when dealing cards */
    signed char next;
} card_t;


/**
 * LCD card drawing routines
 */

static void draw_cursor( int x, int y )
{
    rb->lcd_set_drawmode( DRMODE_COMPLEMENT );
    rb->lcd_fillrect( x+1, y+1, CARD_GFX_WIDTH, CARD_GFX_HEIGHT );
#ifdef LARGE_CARD
    rb->lcd_drawpixel( x+1, y+1 );
    rb->lcd_drawpixel( x+1, y+CARD_HEIGHT-2 );
    rb->lcd_drawpixel( x+CARD_WIDTH-2, y+1 );
    rb->lcd_drawpixel( x+CARD_WIDTH-2, y+CARD_HEIGHT-2 );
#endif
    rb->lcd_set_drawmode( DRMODE_SOLID );
}

/* Draw a card's border, select it if it's selected and draw the cursor
 * if the cursor is currently over the card */
static void draw_card_ext( int x, int y, bool selected, bool cursor )
{
#if LCD_DEPTH > 1
    int oldfg = rb->lcd_get_foreground();

    rb->lcd_set_foreground( LCD_BLACK );
#endif
#ifdef LARGE_CARD
    rb->lcd_hline( x+2, x+CARD_WIDTH-3, y );
    rb->lcd_hline( x+2, x+CARD_WIDTH-3, y+CARD_HEIGHT-1 );
    rb->lcd_vline( x, y+2, y+CARD_HEIGHT-3 );
    rb->lcd_vline( x+CARD_WIDTH-1, y+2, y+CARD_HEIGHT-3 );
    rb->lcd_drawpixel( x+1, y+1 );
    rb->lcd_drawpixel( x+1, y+CARD_HEIGHT-2 );
    rb->lcd_drawpixel( x+CARD_WIDTH-2, y+1 );
    rb->lcd_drawpixel( x+CARD_WIDTH-2, y+CARD_HEIGHT-2 );
#else
    rb->lcd_hline( x+1, x+CARD_WIDTH-2, y );
    rb->lcd_hline( x+1, x+CARD_WIDTH-2, y+CARD_HEIGHT-1 );
    rb->lcd_vline( x, y+1, y+CARD_HEIGHT-2 );
    rb->lcd_vline( x+CARD_WIDTH-1, y+1, y+CARD_HEIGHT-2 );
#endif

    if( selected )
    {
#if LCD_DEPTH > 1
        rb->lcd_set_foreground( FRAME_COLOR );
#endif
        rb->lcd_drawrect( x+1, y+1, CARD_WIDTH-2, CARD_HEIGHT-2 );
#ifdef LARGE_CARD
        rb->lcd_drawrect( x+2, y+2, CARD_WIDTH-4, CARD_HEIGHT-4 );
#endif
    }
#if LCD_DEPTH > 1
    rb->lcd_set_foreground( oldfg );
#endif

    if( cursor )
    {
        draw_cursor( x, y );
    }
}

/* Draw a card's inner graphics */
static void draw_card( card_t *card, int x, int y,
                       bool selected, bool cursor )
{
    if( card->known )
    {
        rb->lcd_bitmap_part( card_deck, CARD_GFX_WIDTH * card->num,
                             CARD_GFX_HEIGHT * card->suit, 
                             STRIDE(SCREEN_MAIN, 
                                    BMPWIDTH_card_deck, BMPHEIGHT_card_deck),
                             x+1, y+1, CARD_GFX_WIDTH, CARD_GFX_HEIGHT );
    }
    else
    {
        rb->lcd_bitmap( card_back, x+1, y+1,
                        CARD_GFX_WIDTH, CARD_GFX_HEIGHT );
    }
    draw_card_ext( x, y, selected, cursor );
}

/* Draw an empty stack */
static void draw_empty_stack( int s, int x, int y, bool cursor )
{
    rb->lcd_bitmap_part( solitaire_suitsi, 0,
                 CARD_GFX_HEIGHT * s, 
                 STRIDE( SCREEN_MAIN,
                         BMPWIDTH_solitaire_suitsi, BMPHEIGHT_solitaire_suitsi),
                 x+1, y+1, CARD_GFX_WIDTH, CARD_GFX_HEIGHT );

    draw_card_ext( x, y, false, cursor );
}

/* Help */
static bool solitaire_help( void )
{
    static char* help_text[] = {
        "Solitaire", "", "Controls", "",
        HK_LR   ":", "Move", "the", "cursor", "to", "the",
            "previous/", "next", "column.", "",
        HK_UD   ":", "Move", "the", "cursor", "up/", "down", "in", "the",
            "column.", "",
        HK_MOVE ":", "Select", "cards,", "move", "cards...", "",
        HK_DRAW ":", "Deselect", "a", "card", "if", "it", "was", "selected.",
            "Else", "draw", "new", "card(s)", "from", "the", "remains",
            "stack.", "", "",
        "Shortcuts", "",
        HK_REM2CUR   ":", "Put", "the", "card", "on", "top", "of", "the",
            "remains", "stack", "on", "top", "of", "the", "cursor.", "",
        HK_CUR2STACK ":", "Put", "the", "card", "under", "the", "cursor",
            "on", "one", "of", "the", "4", "final", "stacks.", "",
        HK_REM2STACK ":", "Put", "the", "card", "on", "top", "of", "the",
            "remains", "stack", "on", "one", "of", "the", "4", "final",
            "stacks."
    };
    static struct style_text formation[]={
        { 0, TEXT_CENTER|TEXT_UNDERLINE },
        { 2, C_RED },
        { 48, C_RED },
        LAST_STYLE_ITEM
    };
    if (display_text(ARRAYLEN(help_text), help_text, formation, NULL, true))
        return true;
    return false;
}

/**
 * Custom menu / options
 */

#define CFGFILE_VERSION 0

struct sol_config {
    int draw_type;
};

struct sol_config sol_disk = {0};
struct sol_config sol;

static struct configdata config[] = {
   { TYPE_INT, 0, 1, { .int_p = &sol_disk.draw_type }, "draw_type", NULL }
};

static const struct opt_items drawcards[2] = {
    { "Draw Three Cards", -1 },
    { "Draw One Card", -1 },
};

void solitaire_init(void);

/* menu return codes */
enum { MENU_RESUME, MENU_SAVE_AND_QUIT, MENU_QUIT, MENU_USB };

static bool _ingame;
static int solitaire_menu_cb(int action, const struct menu_item_ex *this_item)
{
    int i = (intptr_t)this_item;
    if( action == ACTION_REQUEST_MENUITEM )
    {
        if((!_ingame && (i==0 || i==5)) || ( _ingame && i==2 ))
            return ACTION_EXIT_MENUITEM;
    }
    return action;
}

static int solitaire_menu(bool in_game)
{
    int selected = 0;
    int result = -1;

    MENUITEM_STRINGLIST(menu, "Solitaire Menu", solitaire_menu_cb,
                        "Resume Game", "Start New Game",
                        "Draw Cards Option",
                        "Help", "Playback Control",
                        "Save and Quit", "Quit");
    _ingame = in_game;

    while (result < 0)
    {
        switch (rb->do_menu(&menu, &selected, NULL, false))
        {
            default:
                result = MENU_RESUME;
                break;

            case MENU_ATTACHED_USB:
                result = MENU_USB;
                break;

            case 0:
                result = MENU_RESUME;
                break;

            case 1:
                solitaire_init();
                result = MENU_RESUME;
                break;

            case 2:
                if (rb->set_option("Draw Cards Option", &sol.draw_type,
                                   INT, drawcards, 2, NULL))
                    result = MENU_USB;
                break;

            case 3:
                if (solitaire_help())
                    result = MENU_USB;
                break;

            case 4:
                 playback_control(NULL);
                 break;

            case 5:
                result = MENU_SAVE_AND_QUIT;
                break;

            case 6:
                result = MENU_QUIT;
                break;
        }
    }
    return result;
}

/**
 * Global variables
 */

/* player's cursor */
int cur_card;
/* player's cursor column num */
int cur_col;

/* selected card */
int sel_card;

/* the deck */
card_t deck[ NUM_CARDS ];

/* the remaining cards */
/* first card of the remains' stack */
int rem;
/* upper visible card from the remains' stack */
int cur_rem;
/* number of cards drawn from the remains stack - 1 */
int count_rem;
/* number of cards per draw of the remains' stack */
int cards_per_draw;

/* the 7 game columns */
int cols[COL_NUM];
/* the 4 final stacks */
int stacks[SUITS];

/**
 * Card handling routines
 */

static int next_random_card( card_t *deck )
{
    int i,r;

    r = rb->rand()%(NUM_CARDS)+1;
    i = 0;

    while( r>0 )
    {
        i = (i + 1)%(NUM_CARDS);
        if( !deck[i].used ) r--;
    }

    deck[i].used = true;

    return i;
}


/* initialize the game */
void solitaire_init( void )
{

    int c;
    int i, j;

    /* number of cards that are drawn on the remains' stack (by pressing F2) */
    if( sol.draw_type == 0 )
    {
        cards_per_draw = 3;
    }
    else
    {
        cards_per_draw = 1;
    }

    /* init deck */
    for( i=0; i<SUITS; i++ )
    {
        for( j=0; j<CARDS_PER_SUIT; j++ )
        {
#define card deck[i*CARDS_PER_SUIT+j]
            card.suit = i;
            card.num = j;
            card.known = true;
            card.used = false;
            card.next = NOT_A_CARD;
#undef card
        }
    }

    /* deal the cards ... */
    /* ... in the columns */
    for( i=0; i<COL_NUM; i++ )
    {
        c = NOT_A_CARD;
        for( j=0; j<=i; j++ )
        {
            if( c == NOT_A_CARD )
            {
                cols[i] = next_random_card( deck );
                c = cols[i];
            }
            else
            {
                deck[c].next = next_random_card( deck );
                c = deck[c].next;
            }
            if( j < i )
                deck[c].known = false;
        }
    }

    /* ... shuffle what's left of the deck */
    rem = next_random_card(deck);
    c = rem;

    for( i=1; i < NUM_CARDS - COL_NUM * (COL_NUM + 1)/2; i++ )
    {
        deck[c].next = next_random_card( deck );
        c = deck[c].next;
    }

    /* we now finished dealing the cards. The game can start ! (at last) */

    /* init the stack */
    for( i = 0; i<SUITS; i++ )
    {
        stacks[i] = NOT_A_CARD;
    }

    /* the cursor starts on upper left card */
    cur_card = cols[0];
    cur_col = 0;

    /* no card is selected */
    sel_card = NOT_A_CARD;

    /* init the remainder */
    cur_rem = NOT_A_CARD;

    count_rem = -1;
}

/* find the column number in which 'card' can be found */
static int find_card_col( int card )
{
    int i;
    int c;

    if( card == NOT_A_CARD ) return NOT_A_COL;

    for( i=0; i<COL_NUM; i++ )
    {
        c = cols[i];
        while( c != NOT_A_CARD )
        {
            if( c == card ) return i;
            c = deck[c].next;
        }
    }

    for( i=0; i<SUITS; i++ )
    {
        c = stacks[i];
        while( c != NOT_A_CARD )
        {
            if( c == card ) return STACKS_COL + i;
            c = deck[c].next;
        }
    }

    return REM_COL;
}

/* find the card preceding 'card' */
/* if it doesn't exist, return NOT_A_CARD */
static int find_prev_card( int card ){
    int i;

    for( i=0; i < NUM_CARDS; i++ )
    {
        if( deck[i].next == card ) return i;
    }

    return NOT_A_CARD;
}

/* find the last card of a given column */
static int find_last_card( int col )
{
    int c;

    if( col < COL_NUM )
    {
        c = cols[col];
    }
    else if( col < REM_COL )
    {
        c = stacks[col - STACKS_COL];
    }
    else
    {
        c = cur_rem;
    }

    if(c == NOT_A_CARD)
        return c;
    else
    {
        while(deck[c].next != NOT_A_CARD)
            c = deck[c].next;
        return c;
    }
}

enum move { MOVE_OK, MOVE_NOT_OK };

static enum move move_card( int dest_col, int src_card )
{
    /* the column on which to take src_card */
    int src_col;

    /* the last card of dest_col */
    int dest_card;

    /* the card under src_card */
    int src_card_prev;

    /* you can't move no card (at least, it doesn't have any consequence) */
    if( src_card == NOT_A_CARD ) return MOVE_NOT_OK;
    /* you can't put a card back on the remains' stack */
    if( dest_col == REM_COL ) return MOVE_NOT_OK;
    /* you can't move an unknown card */
    if( !deck[src_card].known ) return MOVE_NOT_OK;

    src_col = find_card_col( src_card );
    dest_card = find_last_card( dest_col );
    src_card_prev = find_prev_card( src_card );

    /* you can't move more than one card at a time from the final stack */
    /* to the rest of the game */
    if( src_col >= COL_NUM && src_col < REM_COL
       && deck[src_card].next != NOT_A_CARD )
    {
        return MOVE_NOT_OK;
    }

    /* if we (that means dest) are on one of the 7 columns ... */
    if( dest_col < COL_NUM )
    {
        /* ... check is we are on an empty color and that the src is a king */
        if( dest_card == NOT_A_CARD
            && deck[src_card].num == CARDS_PER_SUIT - 1 )
        {
            /* this is a winning combination */
            cols[dest_col] = src_card;
        }
        /* ... or check if the cards follow one another and have
         * different colorsuit */
        else if(( deck[dest_card].suit + deck[src_card].suit)%2==1
                  && deck[dest_card].num == deck[src_card].num + 1 )
        {
            /* this is a winning combination */
            deck[dest_card].next = src_card;
        }
        /* ... or, humpf, well that's not good news */
        else
        {
            /* this is not a winning combination */
            return MOVE_NOT_OK;
        }
    }
    /* if we are on one of the 4 final stacks ... */
    else if( dest_col < REM_COL )
    {
        /* ... check if we are on an empty stack... */
        if( dest_card == NOT_A_CARD )
        {
            /* ... and the src is an ace and this is the correct final stack */
            if( deck[src_card].num == 0
                && deck[src_card].suit == dest_col - STACKS_COL )
            {
                /* this is a winning combination */
                stacks[dest_col - STACKS_COL] = src_card;
            }
            else
            {
                /* this is not a winning combination */
                return MOVE_NOT_OK;
            }
        }
        else /* non-empty stack */
        {
            /* ...check if the cards follow one another, have the same suit and
             * {that src has no .next element or is from the remains' stack} */
            if( deck[dest_card].suit == deck[src_card].suit
                && deck[dest_card].num + 1 == deck[src_card].num
                && (deck[src_card].next == NOT_A_CARD || src_col == REM_COL) )
            {
                /* this is a winning combination */
                deck[dest_card].next = src_card;
            }
            /* ... or, well that's not good news */
            else
            {
                /* this is not a winning combination */
                return MOVE_NOT_OK;
            }
        }
    }
    /* if we are on the remains' stack */
    else
    {
        /* you can't move a card back to the remains' stack */
        return MOVE_NOT_OK;
    }

    /* if the src card is from the remains' stack, we don't want to take
     * the following cards */
    if( src_col == REM_COL )
    {
        /* if src card is the first card from the stack */
        if( src_card_prev == NOT_A_CARD )
        {
            rem = deck[src_card].next;
        }
        /* if src card is not the first card from the stack */
        else
        {
            deck[src_card_prev].next = deck[src_card].next;
        }
        deck[src_card].next = NOT_A_CARD;
        cur_rem = src_card_prev;
        count_rem--;
    }
    /* if the src card is from somewhere else, just take everything */
    else
    {
        if( src_card_prev == NOT_A_CARD )
        {
            if( src_col < COL_NUM )
            {
                cols[src_col] = NOT_A_CARD;
            }
            else
            {
                stacks[src_col - STACKS_COL] = NOT_A_CARD;
            }
        }
        else
        {
            deck[src_card_prev].next = NOT_A_CARD;
            deck[src_card_prev].known = true;
        }
    }
    sel_card = NOT_A_CARD;
    /* tada ! */
    return MOVE_OK;
}

enum { SOLITAIRE_WIN, SOLITAIRE_SAVE_AND_QUIT, SOLITAIRE_QUIT, SOLITAIRE_USB };

/**
 * Bouncing cards at the end of the game
 */

#define BC_ACCEL   ((1<<16)*LCD_HEIGHT/128)
#define BC_MYSPEED (6*BC_ACCEL)
#define BC_MXSPEED (6*LCD_HEIGHT/128)

static int bouncing_cards( void )
{
    int i, j, x, vx, y, fp_y, fp_vy, button;

    /* flush the button queue */
    while( ( button = rb->button_get( false ) ) != BUTTON_NONE )
    {
        if( rb->default_event_handler( button ) == SYS_USB_CONNECTED )
            return SOLITAIRE_USB;
    }

    /* fun stuff :) */
    for( i = CARDS_PER_SUIT-1; i>=0; i-- )
    {
        for( j = 0; j < SUITS; j++ )
        {
            x = LCD_WIDTH-(CARD_WIDTH*4+4+MARGIN)+CARD_WIDTH*j+j+1;
            fp_y = MARGIN<<16;

#if LCD_WIDTH > 200
            vx = rb->rand() % (4*BC_MXSPEED/3-2) - BC_MXSPEED;
            if( vx >= -1 )
                vx += 3;
#else
            vx = rb->rand() % (4*BC_MXSPEED/3) - BC_MXSPEED;
            if( vx >= 0 )
                vx++;
#endif

            fp_vy = -rb->rand() % BC_MYSPEED;

            while( x < LCD_WIDTH && x + CARD_WIDTH > 0 )
            {
                fp_vy += BC_ACCEL;
                x += vx;
                fp_y += fp_vy;
                if( fp_y >= (LCD_HEIGHT-CARD_HEIGHT) << 16 )
                {
                    fp_vy = -fp_vy*4/5;
                    fp_y = (LCD_HEIGHT-CARD_HEIGHT) << 16;
                }
                y = fp_y >> 16;
                draw_card( &deck[j*CARDS_PER_SUIT+i], x, y,
                           false, false );
                rb->lcd_update_rect( x<0?0:x, y<0?0:y,
                                     CARD_WIDTH, CARD_HEIGHT );

                button = rb->button_get_w_tmo( 2 );
                if( rb->default_event_handler( button ) == SYS_USB_CONNECTED )
                    return SOLITAIRE_USB;
                if( button == SOL_QUIT || button == SOL_MOVE )
                    return SOLITAIRE_WIN;
            }
        }
    }
    return SOLITAIRE_WIN;
}

/**
 * Game save/load routines
 */
static void get_save_filename( char *buf )
{
#ifdef APPLICATION
    rb->snprintf(buf, sizeof(buf), PLUGIN_DATA_DIR "/sol.save");
#else
    char *s;
    rb->strcpy( buf, rb->plugin_get_current_filename() );
    s = rb->strrchr( buf, '/' ) + 1;
    *s = '\0';
    rb->strcat( s, "sol.save" );
#endif
}

static int open_save_file( int flags )
{
    char buf[MAX_PATH];
    get_save_filename( buf );
    return rb->open( buf, flags, 0666);
}

static void delete_save_file( void )
{
    char buf[MAX_PATH];
    get_save_filename( buf );
    rb->remove( buf );
}

#ifdef write
#   undef write
#endif
static int save_write( int fd, const void *buf, size_t count, int *checksum )
{
    size_t i;
    if( rb->write( fd, buf, count ) < (ssize_t)count )
        return 1;
    for( i = 0; i < count; i++ )
        *checksum += (int)(((const char *)buf)[i]);
    return 0;
}

#ifdef read
#   undef read
#endif
static int save_read( int fd, void *buf, size_t count, int *checksum )
{
    size_t i;
    if( rb->read( fd, buf, count ) < (ssize_t)count )
        return 1;
    for( i = 0; i < count; i++ )
        *checksum -= (int)(((const char *)buf)[i]);
    return 0;
}

static int save_game( void )
{
    int fd = open_save_file( O_CREAT|O_WRONLY|O_TRUNC );
    int checksum = 42;
    if( fd < 0 )
        return -1;
    if(    save_write( fd, &cur_card, sizeof( int ), &checksum )
        || save_write( fd, &cur_col, sizeof( int ), &checksum )
        || save_write( fd, &sel_card, sizeof( int ), &checksum )
        || save_write( fd, deck, NUM_CARDS * sizeof( card_t ), &checksum )
        || save_write( fd, &rem, sizeof( int ), &checksum )
        || save_write( fd, &cur_rem, sizeof( int ), &checksum )
        || save_write( fd, &count_rem, sizeof( int ), &checksum )
        || save_write( fd, &cards_per_draw, sizeof( int ), &checksum )
        || save_write( fd, cols, COL_NUM * sizeof( int ), &checksum )
        || save_write( fd, stacks, SUITS * sizeof( int ), &checksum )
        || ( rb->write( fd, &checksum, sizeof( int ) ) < (ssize_t)(sizeof( int ) ) ) )
    {
        rb->close( fd );
        rb->splash( 2*HZ, "Error while saving game. Aborting." );
        return -2;
    }
    rb->close( fd );
    return 0;
}

static int load_game( void )
{
    int checksum, retval;
    
    int fd = open_save_file( O_RDONLY );
    if( fd < 0 )
        return -1;
    
    retval = 0; /* Assume good case */
    if(    ( rb->lseek( fd, -sizeof( int ), SEEK_END ) == -((ssize_t)sizeof( int ))-1 )
        || ( rb->read( fd, &checksum, sizeof( int ) ) < ((ssize_t)sizeof( int )) )
        || ( rb->lseek( fd, 0, SEEK_SET ) == -1 )
        || save_read( fd, &cur_card, sizeof( int ), &checksum )
        || save_read( fd, &cur_col, sizeof( int ), &checksum )
        || save_read( fd, &sel_card, sizeof( int ), &checksum )
        || save_read( fd, deck, NUM_CARDS * sizeof( card_t ), &checksum )
        || save_read( fd, &rem, sizeof( int ), &checksum )
        || save_read( fd, &cur_rem, sizeof( int ), &checksum )
        || save_read( fd, &count_rem, sizeof( int ), &checksum )
        || save_read( fd, &cards_per_draw, sizeof( int ), &checksum )
        || save_read( fd, cols, COL_NUM * sizeof( int ), &checksum )
        || save_read( fd, stacks, SUITS * sizeof( int ), &checksum ) )
    {
        rb->splash( 2*HZ, "Error while loading saved game. Aborting." );
        retval = -2;
    }
    else if( checksum != 42 )
    {
        rb->splash( 2*HZ, "Save file was corrupted. Aborting." );
        retval = -3;
    }
    
    rb->close( fd );
    delete_save_file();
    return retval;
}

/**
 * The main game loop
 *
 * If skipmenu is defined to SOLITAIRE_QUIT, the menu will be skipped and
 * game will resume.
 */

static int solitaire( int skipmenu )
{

    int i,j;
    int button;
#ifdef NEED_LASTBUTTON_VAR
    int lastbutton = 0;
#endif
    int c,h,prevcard;
    int biggest_col_length;

    rb->srand( *rb->current_tick );
    if( skipmenu != SOLITAIRE_QUIT )
    {
        switch( solitaire_menu(false) )
        {
            case MENU_QUIT:
                return SOLITAIRE_QUIT;

            case MENU_USB:
                return SOLITAIRE_USB;
        }
        solitaire_init();
    }

    while( true )
    {
        rb->lcd_clear_display();

        /* get the biggest column length so that display can be "optimized" */
        biggest_col_length = 0;

        for(i=0;i<COL_NUM;i++)
        {
            j = 0;
            c = cols[i];

            if( c != NOT_A_CARD )
            {
                while( true )
                {
                    /* don't include the last card in the column length. */
                    if( deck[c].next == NOT_A_CARD )
                    {
                        break;  /* no successor: get outta here. */
                    }
                    else
                    {
                        if( deck[c].known )
                            j += 2;
                        else
                            j++;
                    }
                    c = deck[c].next;
                }
                /* make column distinguishable from an empty column,
                 * and avoid division by zero while displaying */
                if( j == 0 )
                    j = 1;
            }
            if( j > biggest_col_length )
                biggest_col_length = j;
        }

        /* check if there are cards remaining in the game. */
        /* if there aren't any, that means you won :) */
        if( biggest_col_length == 0 && rem == NOT_A_CARD )
        {
            rb->lcd_update();
            rb->splash( HZ, "You Won :)" );
            return bouncing_cards();
        }

        /* draw the columns */
        for( i = 0; i < COL_NUM; i++ )
        {
            c = cols[i];
            j = CARD_START;
            while( true )
            {
                if( c == NOT_A_CARD )
                {
                    /* draw the cursor on empty columns */
                    if( cur_col == i )
                    {
                        draw_cursor( MARGIN + i * (CARD_WIDTH
                                     +(LCD_WIDTH-COL_NUM*CARD_WIDTH-2*MARGIN)/(COL_NUM-1)),
                                     j );
                    }
                    break;
                }

                draw_card( &deck[c], MARGIN + i * (CARD_WIDTH
                           +(LCD_WIDTH-COL_NUM*CARD_WIDTH-2*MARGIN)/(COL_NUM-1)),
                           j, c == sel_card, c == cur_card );

                h = c;
                c = deck[c].next;
                if( c == NOT_A_CARD ) break;

                /* This is where we change the spacing between cards so that
                 * they don't overflow out of the LCD */
                if( h == cur_card )
                    j += SYMBOL_HEIGHT;
                else if( deck[h].known )
                    j += min( SYMBOL_HEIGHT,
                              2 * (LCD_HEIGHT - CARD_START - CARD_HEIGHT - MARGIN)
                               / biggest_col_length );
                else
                    j += min( SYMBOL_HEIGHT / 2,
                              (LCD_HEIGHT - CARD_START - CARD_HEIGHT - MARGIN)
                               / biggest_col_length );
            }
        }

        /* draw the stacks */
        for( i=0; i<SUITS; i++ )
        {
            c = find_last_card( STACKS_COL + i );

            if( c != NOT_A_CARD )
            {
                draw_card( &deck[c],
                           LCD_WIDTH-(CARD_WIDTH*4+4+MARGIN)+CARD_WIDTH*i+i+1,
                           MARGIN,
                           c == sel_card, cur_col == STACKS_COL + i );
            }
            else
            {
                draw_empty_stack( i,
                           LCD_WIDTH-(CARD_WIDTH*4+4+MARGIN)+CARD_WIDTH*i+i+1,
                           MARGIN, cur_col == STACKS_COL + i );
            }
        }

        /* draw the remains */
        if( rem != NOT_A_CARD &&
            ( cur_rem == NOT_A_CARD || deck[cur_rem].next != NOT_A_CARD ) )
        {
            /* gruik ! (we want to display a card back) */
            deck[rem].known = false;
            draw_card( &deck[rem], MARGIN, MARGIN, false, false );
            deck[rem].known = true;
        }

        if( rem != NOT_A_CARD && cur_rem != NOT_A_CARD )
        {
            if( count_rem < 0 )
            {
                prevcard = rem;
                count_rem = 0;
                while( prevcard != cur_rem && count_rem < cards_per_draw-1 )
                {
                    prevcard = deck[prevcard].next;
                    count_rem++;
                }
            }
            prevcard = cur_rem;
            j = CARD_WIDTH+2*MARGIN+1;
            for( i = 0; i < count_rem; i++ )
                prevcard = find_prev_card(prevcard);
            for( i = 0; i <= count_rem; i++ )
            {
                draw_card( &deck[prevcard], j,
                           MARGIN, sel_card == prevcard,
                           cur_card == prevcard );
                prevcard = deck[prevcard].next;
                j += CARD_WIDTH/2;
            }
        }
        if( ( cur_rem == NOT_A_CARD || rem == NOT_A_CARD )
              && cur_col == REM_COL )
        {
            draw_cursor( CARD_WIDTH+2*MARGIN+1, MARGIN );
        }

        rb->lcd_update();

        /* what to do when a key is pressed ... */
        button = rb->button_get( true );
#if (CONFIG_KEYPAD == SANSA_E200_PAD) || (CONFIG_KEYPAD == SANSA_FUZE_PAD)
        if (button&(BUTTON_SCROLL_BACK|BUTTON_SCROLL_FWD))
            button = button & (~BUTTON_REPEAT);
#endif
        switch( button )
        {
            /* move cursor to the last card of the previous column
             * or to the previous final stack
             * or to the remains stack */
            case SOL_RIGHT:
#ifdef SOL_RIGHT_PRE
                if( lastbutton != SOL_RIGHT_PRE )
                    break;
#endif
                if( cur_col >= COL_NUM )
                {
                    cur_col = 0;
                }
                else if( cur_col == COL_NUM - 1 )
                {
                    cur_col = REM_COL;
                }
                else
                {
                    cur_col = (cur_col+1)%(REM_COL+1);
                }
                if(cur_col == REM_COL)
                {
                    cur_card = cur_rem;
                    break;
                }
                cur_card  = find_last_card( cur_col );
                break;

            /* move cursor to the last card of the next column
             * or to the next final stack
             * or to the remains stack */
            case SOL_LEFT:
#ifdef SOL_LEFT_PRE
                if( lastbutton != SOL_LEFT_PRE )
                    break;
#endif
                if( cur_col == 0 )
                {
                    cur_col = REM_COL;
                }
                else if( cur_col >= COL_NUM )
                {
                    cur_col = COL_NUM - 1;
                }
                else
                {
                    cur_col = (cur_col + REM_COL)%(REM_COL+1);
                }
                if( cur_col == REM_COL )
                {
                    cur_card = cur_rem;
                    break;
                }
                cur_card = find_last_card( cur_col );
                break;

            /* move cursor to card that's bellow */
            case SOL_DOWN:
#ifdef SOL_DOWN_PRE
                if( lastbutton != SOL_DOWN_PRE )
                    break;
#else
            case SOL_DOWN|BUTTON_REPEAT:
#endif
                if( cur_col >= COL_NUM )
                {
                    cur_col = (cur_col - COL_NUM + 1)%(SUITS + 1) + COL_NUM;
                    if( cur_col == REM_COL )
                    {
                        cur_card = cur_rem;
                    }
                    else
                    {
                        cur_card = find_last_card( cur_col );
                    }
                    break;
                }
                if( cur_card == NOT_A_CARD ) break;
                if( deck[cur_card].next != NOT_A_CARD )
                {
                    cur_card = deck[cur_card].next;
                }
                else
                {
                    cur_card = cols[cur_col];
                    while( !deck[ cur_card].known
                           && deck[cur_card].next != NOT_A_CARD )
                    {
                        cur_card = deck[cur_card].next;
                    }
                }
                break;

            /* move cursor to card that's above */
            case SOL_UP:
#ifdef SOL_UP_PRE
                if( lastbutton != SOL_UP_PRE )
                    break;
#else
            case SOL_UP|BUTTON_REPEAT:
#endif
                if( cur_col >= COL_NUM )
                {
                    cur_col = (cur_col - COL_NUM + SUITS)%(SUITS + 1) + COL_NUM;
                    if( cur_col == REM_COL )
                    {
                        cur_card = cur_rem;
                    }
                    else
                    {
                        cur_card = find_last_card( cur_col );
                    }
                    break;
                }
                if( cur_card == NOT_A_CARD ) break;
                do {
                    cur_card = find_prev_card( cur_card );
                    if( cur_card == NOT_A_CARD )
                    {
                        cur_card = find_last_card( cur_col );
                    }
                } while(    deck[cur_card].next != NOT_A_CARD
                         && !deck[cur_card].known );
                break;

            /* Try to put card under cursor on one of the stacks */
            case SOL_CUR2STACK:
#ifdef SOL_CUR2STACK_PRE
                if( lastbutton != SOL_CUR2STACK_PRE )
                    break;
#endif
                move_card( deck[cur_card].suit + STACKS_COL, cur_card );
                break;

            /* Move cards arround, Uncover cards, ... */
            case SOL_MOVE:
#ifdef SOL_MOVE_PRE
                if( lastbutton != SOL_MOVE_PRE )
                    break;
#endif

                if( sel_card == NOT_A_CARD )
                {
                    if( cur_card != NOT_A_CARD )
                    {
                        if(    deck[cur_card].next == NOT_A_CARD
                            && !deck[cur_card].known )
                        {
                            /* reveal a hidden card */
                            deck[cur_card].known = true;
                        }
                        else if( cur_col == REM_COL && cur_rem == NOT_A_CARD )
                        {
                               break;
                        }
                        else
                        {
                            /* select a card */
                            sel_card = cur_card;
                        }
                    }
                }
                else if( sel_card == cur_card )
                {
                    /* unselect card or try putting card on
                     * one of the 4 stacks */
                    if( move_card( deck[sel_card].suit + COL_NUM, sel_card )
                        == MOVE_OK && cur_col == REM_COL )
                    {
                        cur_card = cur_rem;
                    }
                    sel_card = NOT_A_CARD;
                }
                else
                {
                    /* try moving cards */
                    /* The flexible move must not be used from the remains stack. */
                    if (find_card_col(sel_card) == REM_COL)
                    {
                        if (move_card( cur_col, sel_card ) == MOVE_NOT_OK)
                            sel_card = NOT_A_CARD;
                    }
                    else
                    {
                        do {
                            if (move_card( cur_col, sel_card) == MOVE_OK)
                                break;
                            sel_card = find_prev_card(sel_card);
                        } while (sel_card != NOT_A_CARD);
                    }
                }
                break;

            /* If the card on the top of the remains can be put where
             * the cursor is, go ahead */
            case SOL_REM2CUR:
#ifdef SOL_REM2CUR_PRE
                if( lastbutton != SOL_REM2CUR_PRE )
                    break;
#endif
                move_card( cur_col, cur_rem );
                break;

            /* If the card on top of the remains can be put on one
             * of the stacks, do so */
            case SOL_REM2STACK:
#ifdef SOL_REM2STACK_PRE
                if( lastbutton != SOL_REM2STACK_PRE )
                    break;
#endif
                move_card( deck[cur_rem].suit + COL_NUM, cur_rem );
                break;

#ifdef SOL_REM
            case SOL_REM:
                if( sel_card != NOT_A_CARD )
                {
                    /* unselect selected card */
                    sel_card = NOT_A_CARD;
                    break;
                }
                if( rem != NOT_A_CARD && cur_rem != NOT_A_CARD )
                {
                    sel_card = cur_rem;
                    break;
                }
                break;
#endif

            /* unselect selected card or ...
             * draw new cards from the remains of the deck */
            case SOL_DRAW:
#ifdef SOL_DRAW_PRE
                if( lastbutton != SOL_DRAW_PRE )
                    break;
#endif
                if( sel_card != NOT_A_CARD )
                {
                    /* unselect selected card */
                    sel_card = NOT_A_CARD;
                    break;
                }
                if( rem != NOT_A_CARD )
                {
                    int cur_rem_old = cur_rem;
                    count_rem = -1;
                    /* draw new cards form the remains of the deck */
                    if( cur_rem == NOT_A_CARD )
                    {
                        /*if the cursor card is null*/
                        cur_rem = rem;
                        i = cards_per_draw - 1;
                        count_rem++;
                    }
                    else
                    {
                        i = cards_per_draw;
                    }

                    while( i > 0 && deck[cur_rem].next != NOT_A_CARD )
                    {
                        cur_rem = deck[cur_rem].next;
                        i--;
                        count_rem++;
                    }
                    /* test if any cards are really left on
                     * the remains' stack */
                    if( i == cards_per_draw )
                    {
                        cur_rem = NOT_A_CARD;
                        count_rem = -1;
                    }
                    /* if cursor was on remains' stack when new cards were
                     * drawn, put cursor on top of remains' stack */
                    if( cur_col == REM_COL && cur_card == cur_rem_old )
                    {
                        cur_card = cur_rem;
                        sel_card = NOT_A_CARD;
                    }
                }
                break;

            /* Show the menu */
#ifdef SOL_RC_QUIT
            case SOL_RC_QUIT:
#endif
            case SOL_QUIT:
                switch( solitaire_menu(true) )
                {
                    case MENU_SAVE_AND_QUIT:
                        return SOLITAIRE_SAVE_AND_QUIT;

                    case MENU_QUIT:
                        return SOLITAIRE_QUIT;

                    case MENU_USB:
                        return SOLITAIRE_USB;
                }
                break;

            case SYS_POWEROFF:
                return SOLITAIRE_SAVE_AND_QUIT;

            default:
                if( rb->default_event_handler( button ) == SYS_USB_CONNECTED )
                    return SOLITAIRE_USB;
                break;
        }

#ifdef NEED_LASTBUTTON_VAR
        if( button != BUTTON_NONE )
            lastbutton = button;
#endif

        /* fix incoherences concerning cur_col and cur_card */
        c = find_card_col( cur_card );
        if( c != NOT_A_COL && c != cur_col )
            cur_card = find_last_card( cur_col );

        if(    cur_card == NOT_A_CARD
            && find_last_card( cur_col ) != NOT_A_CARD )
            cur_card = find_last_card( cur_col );

        rb->yield();
    }
}

/**
 * Plugin entry point
 */

enum plugin_status plugin_start(const void* parameter )
{
    int result;

    /* plugin init */
    (void)parameter;

    configfile_load(CONFIG_FILENAME, config,
                    sizeof(config) / sizeof(config[0]), CFGFILE_VERSION);
    rb->memcpy(&sol, &sol_disk, sizeof(sol));   /* copy to running config */

    if( load_game() == 0 )
    {
        rb->splash( HZ, "Resuming saved game." );
        result = SOLITAIRE_QUIT;
    }
    else
        result = SOLITAIRE_WIN;

    /* play the game :)
     * Keep playing if a game was won (that means display the menu after
     * winning instead of quiting) */
    while( ( result = solitaire( result ) ) == SOLITAIRE_WIN );

    if( result != SOLITAIRE_QUIT )
        /* result == SOLITAIRE_USB || result == SOLITAIRE_SAVE_AND_QUIT */
        save_game();

    if (rb->memcmp(&sol, &sol_disk, sizeof(sol))) /* save settings if changed */
    {
        rb->memcpy(&sol_disk, &sol, sizeof(sol));
        configfile_save(CONFIG_FILENAME, config,
                        sizeof(config) / sizeof(config[0]), CFGFILE_VERSION);
    }

    /* Exit the plugin */
    return ( result == SOLITAIRE_USB ) ? PLUGIN_USB_CONNECTED : PLUGIN_OK;
}
