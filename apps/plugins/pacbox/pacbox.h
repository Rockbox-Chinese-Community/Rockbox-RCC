/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Pacbox - a Pacman Emulator for Rockbox
 *
 * Based on PIE - Pacman Instructional Emulator
 *
 * Copyright (c) 1997-2003,2004 Alessandro Scotti
 * http://www.ascotti.org/
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

/* Platform-specific defines - used in both C and ASM files */

#ifndef _PACBOX_H
#define _PACBOX_H

#include "config.h"

#if CONFIG_KEYPAD == IPOD_4G_PAD

#define PACMAN_UP      BUTTON_RIGHT
#define PACMAN_DOWN    BUTTON_LEFT
#define PACMAN_LEFT    BUTTON_MENU
#define PACMAN_RIGHT   BUTTON_PLAY
#define PACMAN_1UP     BUTTON_SELECT
#define PACMAN_COIN    BUTTON_SELECT
#define PACMAN_MENU    (BUTTON_MENU | BUTTON_SELECT)

#elif CONFIG_KEYPAD == IRIVER_H100_PAD || CONFIG_KEYPAD == IRIVER_H300_PAD

#define PACMAN_UP      BUTTON_RIGHT
#define PACMAN_DOWN    BUTTON_LEFT
#define PACMAN_LEFT    BUTTON_UP
#define PACMAN_RIGHT   BUTTON_DOWN
#define PACMAN_1UP     BUTTON_SELECT
#define PACMAN_2UP     BUTTON_ON
#define PACMAN_COIN    BUTTON_REC
#define PACMAN_MENU    BUTTON_MODE

#ifdef HAVE_REMOTE_LCD

#define PACMAN_HAS_REMOTE

#define PACMAN_RC_UP      BUTTON_RC_VOL_UP
#define PACMAN_RC_DOWN    BUTTON_RC_VOL_DOWN
#define PACMAN_RC_LEFT    BUTTON_RC_REW
#define PACMAN_RC_RIGHT   BUTTON_RC_FF
#define PACMAN_RC_1UP     BUTTON_RC_SOURCE
#define PACMAN_RC_2UP     BUTTON_RC_BITRATE
#define PACMAN_RC_COIN    BUTTON_RC_REC
#define PACMAN_RC_MENU    BUTTON_RC_MODE

#endif

#elif CONFIG_KEYPAD == GIGABEAT_PAD

#define PACMAN_UP      BUTTON_UP
#define PACMAN_DOWN    BUTTON_DOWN
#define PACMAN_LEFT    BUTTON_LEFT
#define PACMAN_RIGHT   BUTTON_RIGHT
#define PACMAN_1UP     BUTTON_SELECT
#define PACMAN_2UP     BUTTON_POWER
#define PACMAN_COIN    BUTTON_A
#define PACMAN_MENU    BUTTON_MENU

#elif CONFIG_KEYPAD == GIGABEAT_S_PAD

#define PACMAN_UP      BUTTON_UP
#define PACMAN_DOWN    BUTTON_DOWN
#define PACMAN_LEFT    BUTTON_LEFT
#define PACMAN_RIGHT   BUTTON_RIGHT
#define PACMAN_1UP     BUTTON_SELECT
#define PACMAN_2UP     BUTTON_POWER
#define PACMAN_COIN    BUTTON_PLAY
#define PACMAN_MENU    BUTTON_MENU

#elif CONFIG_KEYPAD == IAUDIO_X5M5_PAD

#define PACMAN_UP      BUTTON_RIGHT
#define PACMAN_DOWN    BUTTON_LEFT
#define PACMAN_LEFT    BUTTON_UP
#define PACMAN_RIGHT   BUTTON_DOWN
#define PACMAN_1UP     BUTTON_SELECT
#define PACMAN_2UP     BUTTON_POWER
#define PACMAN_COIN    BUTTON_REC
#define PACMAN_MENU    BUTTON_PLAY

#elif CONFIG_KEYPAD == SANSA_E200_PAD

#define PACMAN_UP       BUTTON_UP
#define PACMAN_DOWN     BUTTON_DOWN
#define PACMAN_LEFT     BUTTON_LEFT
#define PACMAN_RIGHT    BUTTON_RIGHT
#define PACMAN_1UP      BUTTON_SELECT
#define PACMAN_2UP      BUTTON_REC
#define PACMAN_COIN_PRE BUTTON_SELECT
#define PACMAN_COIN     (BUTTON_SELECT | BUTTON_DOWN)
#define PACMAN_MENU     BUTTON_POWER

#elif CONFIG_KEYPAD == SANSA_FUZE_PAD

#define PACMAN_UP       BUTTON_RIGHT
#define PACMAN_DOWN     BUTTON_LEFT
#define PACMAN_LEFT     BUTTON_UP
#define PACMAN_RIGHT    BUTTON_DOWN
#define PACMAN_1UP      BUTTON_SELECT
#define PACMAN_COIN_PRE BUTTON_SELECT
#define PACMAN_COIN     (BUTTON_SELECT | BUTTON_DOWN)
#define PACMAN_MENU     BUTTON_HOME

#elif CONFIG_KEYPAD == IRIVER_H10_PAD

#if defined(IRIVER_H10_5GB)
#define PACMAN_UP      BUTTON_SCROLL_UP
#define PACMAN_DOWN    BUTTON_SCROLL_DOWN
#define PACMAN_LEFT    BUTTON_LEFT
#define PACMAN_RIGHT   BUTTON_RIGHT
#else
#define PACMAN_UP      BUTTON_RIGHT
#define PACMAN_DOWN    BUTTON_LEFT
#define PACMAN_LEFT    BUTTON_SCROLL_UP
#define PACMAN_RIGHT   BUTTON_SCROLL_DOWN
#endif

#define PACMAN_1UP     BUTTON_REW
#define PACMAN_2UP     BUTTON_POWER
#define PACMAN_COIN    BUTTON_FF
#define PACMAN_MENU    BUTTON_PLAY

#elif CONFIG_KEYPAD == MROBE500_PAD

#define PACMAN_UP       BUTTON_RC_PLAY
#define PACMAN_DOWN     BUTTON_RC_DOWN
#define PACMAN_LEFT     BUTTON_RC_REW
#define PACMAN_RIGHT    BUTTON_RC_FF
#define PACMAN_1UP      BUTTON_RC_VOL_DOWN
#define PACMAN_2UP      BUTTON_RC_VOL_UP
#define PACMAN_COIN_PRE BUTTON_RC_MODE
#define PACMAN_COIN     (BUTTON_RC_MODE | BUTTON_RC_DOWN)
#define PACMAN_MENU     BUTTON_POWER

#elif CONFIG_KEYPAD == COWON_D2_PAD

#define PACMAN_MENU     (BUTTON_MENU|BUTTON_REL)

#elif CONFIG_KEYPAD == IAUDIO67_PAD

#define PACMAN_UP       BUTTON_STOP
#define PACMAN_DOWN     BUTTON_PLAY
#define PACMAN_LEFT     BUTTON_LEFT
#define PACMAN_RIGHT    BUTTON_RIGHT
#define PACMAN_1UP      BUTTON_VOLDOWN
#define PACMAN_2UP      BUTTON_VOLUP
#define PACMAN_COIN_PRE BUTTON_MENU
#define PACMAN_COIN     (BUTTON_MENU | BUTTON_PLAY)
#define PACMAN_MENU     BUTTON_POWER

#elif CONFIG_KEYPAD == CREATIVEZVM_PAD

#define PACMAN_UP      BUTTON_UP
#define PACMAN_DOWN    BUTTON_DOWN
#define PACMAN_LEFT    BUTTON_LEFT
#define PACMAN_RIGHT   BUTTON_RIGHT
#define PACMAN_1UP     BUTTON_CUSTOM
#define PACMAN_2UP     BUTTON_PLAY
#define PACMAN_COIN    BUTTON_SELECT
#define PACMAN_MENU    BUTTON_MENU

#elif CONFIG_KEYPAD == CREATIVE_ZENXFI3_PAD

#define PACMAN_UP       BUTTON_UP
#define PACMAN_DOWN     BUTTON_DOWN
#define PACMAN_LEFT     BUTTON_BACK
#define PACMAN_RIGHT    BUTTON_MENU
#define PACMAN_1UP      BUTTON_PLAY
#define PACMAN_COIN_PRE BUTTON_PLAY
#define PACMAN_COIN     (BUTTON_PLAY | BUTTON_DOWN)
#define PACMAN_MENU     BUTTON_POWER

#elif CONFIG_KEYPAD == PHILIPS_HDD1630_PAD

#define PACMAN_UP      BUTTON_UP
#define PACMAN_DOWN    BUTTON_DOWN
#define PACMAN_LEFT    BUTTON_LEFT
#define PACMAN_RIGHT   BUTTON_RIGHT
#define PACMAN_1UP     BUTTON_VOL_UP
#define PACMAN_2UP     BUTTON_VOL_DOWN
#define PACMAN_COIN    BUTTON_VIEW
#define PACMAN_MENU    BUTTON_MENU

#elif CONFIG_KEYPAD == PHILIPS_HDD6330_PAD

#define PACMAN_UP      BUTTON_NEXT
#define PACMAN_DOWN    BUTTON_PREV
#define PACMAN_LEFT    BUTTON_UP
#define PACMAN_RIGHT   BUTTON_DOWN
#define PACMAN_1UP     BUTTON_VOL_UP
#define PACMAN_2UP     BUTTON_DOWN
#define PACMAN_COIN    BUTTON_RIGHT
#define PACMAN_MENU    BUTTON_MENU

#elif CONFIG_KEYPAD == PHILIPS_SA9200_PAD

#define PACMAN_UP      BUTTON_UP
#define PACMAN_DOWN    BUTTON_DOWN
#define PACMAN_LEFT    BUTTON_PREV
#define PACMAN_RIGHT   BUTTON_NEXT
#define PACMAN_1UP     BUTTON_VOL_UP
#define PACMAN_2UP     BUTTON_VOL_DOWN
#define PACMAN_COIN    BUTTON_MENU
#define PACMAN_MENU    BUTTON_POWER

#elif CONFIG_KEYPAD == ONDAVX747_PAD

#define PACMAN_MENU     (BUTTON_MENU|BUTTON_REL)

#elif CONFIG_KEYPAD == ONDAVX777_PAD
#define PACMAN_MENU     BUTTON_POWER

#elif CONFIG_KEYPAD == SAMSUNG_YH_PAD

#define PACMAN_UP       BUTTON_UP
#define PACMAN_DOWN     BUTTON_DOWN
#define PACMAN_LEFT     BUTTON_LEFT
#define PACMAN_RIGHT    BUTTON_RIGHT
#define PACMAN_1UP      BUTTON_REC
#define PACMAN_2UP      BUTTON_REW
#define PACMAN_COIN     BUTTON_FFWD
#define PACMAN_MENU     BUTTON_PLAY

#elif CONFIG_KEYPAD == PBELL_VIBE500_PAD

#define PACMAN_UP       BUTTON_UP
#define PACMAN_DOWN     BUTTON_DOWN
#define PACMAN_LEFT     BUTTON_PREV
#define PACMAN_RIGHT    BUTTON_NEXT
#define PACMAN_1UP      BUTTON_PLAY
#define PACMAN_2UP      BUTTON_REC
#define PACMAN_COIN     BUTTON_OK
#define PACMAN_MENU     BUTTON_MENU

#elif CONFIG_KEYPAD == SANSA_FUZEPLUS_PAD

#define PACMAN_UP       BUTTON_UP
#define PACMAN_DOWN     BUTTON_DOWN
#define PACMAN_LEFT     BUTTON_LEFT
#define PACMAN_RIGHT    BUTTON_RIGHT
#define PACMAN_1UP      BUTTON_SELECT
#define PACMAN_2UP      BUTTON_BOTTOMRIGHT
#define PACMAN_COIN     BUTTON_PLAYPAUSE
#define PACMAN_MENU     BUTTON_POWER

#elif CONFIG_KEYPAD == SANSA_CONNECT_PAD

#define PACMAN_UP       BUTTON_UP
#define PACMAN_DOWN     BUTTON_DOWN
#define PACMAN_LEFT     BUTTON_LEFT
#define PACMAN_RIGHT    BUTTON_RIGHT
#define PACMAN_1UP      BUTTON_SELECT
#define PACMAN_2UP      BUTTON_NEXT
#define PACMAN_COIN     BUTTON_VOL_DOWN
#define PACMAN_MENU     BUTTON_POWER

#elif CONFIG_KEYPAD == SAMSUNG_YPR0_PAD

#define PACMAN_UP       BUTTON_UP
#define PACMAN_DOWN     BUTTON_DOWN
#define PACMAN_LEFT     BUTTON_LEFT
#define PACMAN_RIGHT    BUTTON_RIGHT
#define PACMAN_1UP      BUTTON_SELECT
#define PACMAN_2UP      BUTTON_POWER
#define PACMAN_COIN     BUTTON_USER
#define PACMAN_MENU     BUTTON_MENU

#elif CONFIG_KEYPAD == HM60X_PAD

#define PACMAN_UP       BUTTON_UP
#define PACMAN_DOWN     BUTTON_DOWN
#define PACMAN_LEFT     BUTTON_LEFT
#define PACMAN_RIGHT    BUTTON_RIGHT
#define PACMAN_1UP      BUTTON_SELECT
#define PACMAN_2UP      (BUTTON_POWER | BUTTON_UP)
#define PACMAN_COIN     (BUTTON_POWER | BUTTON_DOWN)
#define PACMAN_MENU     BUTTON_POWER

#elif CONFIG_KEYPAD == HM801_PAD

#define PACMAN_UP       BUTTON_UP
#define PACMAN_DOWN     BUTTON_DOWN
#define PACMAN_LEFT     BUTTON_LEFT
#define PACMAN_RIGHT    BUTTON_RIGHT
#define PACMAN_1UP      BUTTON_SELECT
#define PACMAN_2UP      BUTTON_PLAY
#define PACMAN_COIN     BUTTON_PREV
#define PACMAN_MENU     BUTTON_POWER

#else

#error Keymap not defined!

#endif

#ifdef HAVE_TOUCHSCREEN
#ifndef PACMAN_UP
#define PACMAN_UP       BUTTON_MIDRIGHT
#endif
#ifndef PACMAN_DOWN
#define PACMAN_DOWN     BUTTON_MIDLEFT
#endif
#ifndef PACMAN_LEFT
#define PACMAN_LEFT     BUTTON_TOPMIDDLE
#endif
#ifndef PACMAN_RIGHT
#define PACMAN_RIGHT    BUTTON_BOTTOMMIDDLE
#endif
#ifndef PACMAN_1UP
#define PACMAN_1UP      BUTTON_BOTTOMLEFT
#endif
#ifndef PACMAN_2UP
#define PACMAN_2UP      BUTTON_BOTTOMRIGHT
#endif
#ifndef PACMAN_COIN
#define PACMAN_COIN     BUTTON_CENTER
#endif
#ifndef PACMAN_MENU
#define PACMAN_MENU    (BUTTON_TOPLEFT|BUTTON_REL)
#endif
#endif

#if (LCD_HEIGHT >= 288)
#define XOFS ((LCD_WIDTH-224)/2)
#define YOFS ((LCD_HEIGHT-288)/2)
#elif (LCD_WIDTH >= 288)
#define XOFS ((LCD_WIDTH-288)/2)
#define YOFS ((LCD_HEIGHT-224)/2)
#elif (LCD_WIDTH >= 220)
#define XOFS ((LCD_WIDTH-(288*3/4))/2)
#define YOFS ((LCD_HEIGHT-(224*3/4))/2)
#elif (LCD_WIDTH >= 168) && (LCD_HEIGHT >= 216)
#define XOFS ((LCD_WIDTH-(224*3/4))/2)
#define YOFS ((LCD_HEIGHT-(288*3/4))/2)
#elif (LCD_WIDTH >= 144)
#define XOFS ((LCD_WIDTH-288/2)/2)
#define YOFS ((LCD_HEIGHT-224/2)/2)
#elif (LCD_WIDTH >= 128)
#define XOFS ((LCD_WIDTH-224/2)/2)
#if LCD_HEIGHT < 144
#define YCLIP ((288-2*LCD_HEIGHT)/2)
#define YOFS 0
#else
#define YCLIP 0
#define YOFS ((LCD_HEIGHT-288/2)/2)
#endif
#endif

/* How many video frames (out of a possible 60) we display each second.
   NOTE: pacbox.c assumes this is an integer divisor of 60
 */
#if defined(TOSHIBA_GIGABEAT_S) || defined (TOSHIBA_GIGABEAT_F) || \
    defined(SANSA_FUZEPLUS)
/* Gigabeat S,F and Sansa Fuze+ can manage the full framerate 
   (1 in 1 frames) */
#define FPS 60
#elif defined(IPOD_NANO)
/* The Nano can manage full-speed at 30fps (1 in 2 frames) */
#define FPS 30
#else
/* We aim for 20fps on the other targets (1 in 3 frames) */
#define FPS 20
#endif

/* 16kHz sounds pretty good - use it if available */
#define PREFERRED_SAMPLING_RATE SAMPR_16

#endif
