/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2002 Philipp Pertermann
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
#include "lib/configfile.h"
#include "lib/helper.h"
#include "lib/playback_control.h"

#ifdef DEBUG_WORMLET
static long max_cycle;
#endif

/* size of the field the worm lives in */
#define FIELD_RECT_X 1
#define FIELD_RECT_Y 1
#define FIELD_RECT_WIDTH  (LCD_WIDTH - 45)
#define FIELD_RECT_HEIGHT (LCD_HEIGHT - 2)

/* when the game starts */
#define INITIAL_WORM_LENGTH 10

/* num of pixel the worm grows per eaten food */
#define WORM_PER_FOOD 7

/* num of worms creeping in the FIELD */
#define MAX_WORMS 3

/* minimal distance between a worm and an argh
   when a new argh is made */
#define MIN_ARGH_DIST 5

#if (CONFIG_KEYPAD == RECORDER_PAD)
#define BTN_DIR_UP BUTTON_UP
#define BTN_DIR_DOWN BUTTON_DOWN
#define BTN_DIR_LEFT BUTTON_LEFT
#define BTN_DIR_RIGHT BUTTON_RIGHT
#define BTN_PLAYER2_DIR1 BUTTON_F2
#define BTN_PLAYER2_DIR2 BUTTON_F3
#define BTN_STARTPAUSE BUTTON_PLAY
#define BTN_QUIT BUTTON_OFF
#define BTN_STOPRESET BUTTON_ON
#define BTN_TOGGLE_KEYS BUTTON_F1

#if BUTTON_REMOTE != 0
#define BTN_RC_UP BUTTON_RC_VOL_UP
#define BTN_RC_DOWN BUTTON_RC_VOL_DOWN
#define REMOTE
#define MULTIPLAYER
#endif

#elif (CONFIG_KEYPAD == ARCHOS_AV300_PAD)
#define BTN_DIR_UP BUTTON_UP
#define BTN_DIR_DOWN BUTTON_DOWN
#define BTN_DIR_LEFT BUTTON_LEFT
#define BTN_DIR_RIGHT BUTTON_RIGHT
#define BTN_PLAYER2_DIR1 BUTTON_F2
#define BTN_PLAYER2_DIR2 BUTTON_F3
#define BTN_STARTPAUSE BUTTON_SELECT
#define BTN_QUIT BUTTON_OFF
#define BTN_STOPRESET BUTTON_ON
#define BTN_TOGGLE_KEYS BUTTON_F1

#elif (CONFIG_KEYPAD == ONDIO_PAD)
#define BTN_DIR_UP BUTTON_UP
#define BTN_DIR_DOWN BUTTON_DOWN
#define BTN_DIR_LEFT BUTTON_LEFT
#define BTN_DIR_RIGHT BUTTON_RIGHT
#define BTN_STARTPAUSE (BUTTON_MENU|BUTTON_REL)
#define BTN_QUIT (BUTTON_OFF|BUTTON_REL)
#define BTN_STOPRESET (BUTTON_OFF|BUTTON_MENU)

#elif (CONFIG_KEYPAD == IPOD_4G_PAD) || (CONFIG_KEYPAD == IPOD_3G_PAD) || \
      (CONFIG_KEYPAD == IPOD_1G2G_PAD)

#define BTN_DIR_UP BUTTON_MENU
#define BTN_DIR_DOWN BUTTON_PLAY
#define BTN_DIR_LEFT BUTTON_LEFT
#define BTN_DIR_RIGHT BUTTON_RIGHT
#define BTN_STARTPAUSE (BUTTON_SELECT|BUTTON_REL)
#define BTN_QUIT (BUTTON_SELECT|BUTTON_MENU)
#define BTN_STOPRESET (BUTTON_SELECT|BUTTON_PLAY)

#elif (CONFIG_KEYPAD == IRIVER_H300_PAD) || (CONFIG_KEYPAD == IRIVER_H100_PAD)

#define BTN_DIR_UP BUTTON_UP
#define BTN_DIR_DOWN BUTTON_DOWN
#define BTN_DIR_LEFT BUTTON_LEFT
#define BTN_DIR_RIGHT BUTTON_RIGHT
#define BTN_STARTPAUSE (BUTTON_SELECT|BUTTON_REL)
#define BTN_QUIT BUTTON_OFF
#define BTN_STOPRESET BUTTON_ON

#define BTN_RC_QUIT BUTTON_RC_STOP

#elif (CONFIG_KEYPAD == IAUDIO_X5M5_PAD)

#define BTN_DIR_UP BUTTON_UP
#define BTN_DIR_DOWN BUTTON_DOWN
#define BTN_DIR_LEFT BUTTON_LEFT
#define BTN_DIR_RIGHT BUTTON_RIGHT
#define BTN_STARTPAUSE BUTTON_PLAY
#define BTN_QUIT BUTTON_POWER
#define BTN_STOPRESET BUTTON_REC

#elif (CONFIG_KEYPAD == GIGABEAT_PAD)

#define BTN_DIR_UP BUTTON_UP
#define BTN_DIR_DOWN BUTTON_DOWN
#define BTN_DIR_LEFT BUTTON_LEFT
#define BTN_DIR_RIGHT BUTTON_RIGHT
#define BTN_STARTPAUSE BUTTON_SELECT
#define BTN_QUIT BUTTON_POWER
#define BTN_STOPRESET BUTTON_A

#elif (CONFIG_KEYPAD == SANSA_E200_PAD) || \
(CONFIG_KEYPAD == SANSA_C200_PAD)

#define BTN_DIR_UP BUTTON_UP
#define BTN_DIR_DOWN BUTTON_DOWN
#define BTN_DIR_LEFT BUTTON_LEFT
#define BTN_DIR_RIGHT BUTTON_RIGHT
#define BTN_STARTPAUSE BUTTON_SELECT
#define BTN_QUIT BUTTON_POWER
#define BTN_STOPRESET BUTTON_REC

#elif (CONFIG_KEYPAD == SANSA_CLIP_PAD)

#define BTN_DIR_UP BUTTON_UP
#define BTN_DIR_DOWN BUTTON_DOWN
#define BTN_DIR_LEFT BUTTON_LEFT
#define BTN_DIR_RIGHT BUTTON_RIGHT
#define BTN_STARTPAUSE BUTTON_SELECT
#define BTN_QUIT BUTTON_POWER
#define BTN_STOPRESET BUTTON_HOME

#elif (CONFIG_KEYPAD == SANSA_FUZE_PAD)

#define BTN_DIR_UP BUTTON_UP
#define BTN_DIR_DOWN BUTTON_DOWN
#define BTN_DIR_LEFT BUTTON_LEFT
#define BTN_DIR_RIGHT BUTTON_RIGHT
#define BTN_STARTPAUSE BUTTON_SELECT
#define BTN_QUIT (BUTTON_HOME|BUTTON_REPEAT)
#define BTN_STOPRESET (BUTTON_SELECT | BUTTON_UP)

#elif (CONFIG_KEYPAD == SANSA_M200_PAD)

#define BTN_DIR_UP BUTTON_UP
#define BTN_DIR_DOWN BUTTON_DOWN
#define BTN_DIR_LEFT BUTTON_LEFT
#define BTN_DIR_RIGHT BUTTON_RIGHT
#define BTN_STARTPAUSE (BUTTON_SELECT | BUTTON_REL)
#define BTN_QUIT BUTTON_POWER
#define BTN_STOPRESET (BUTTON_SELECT | BUTTON_UP)

#elif (CONFIG_KEYPAD == IRIVER_H10_PAD)

#define BTN_DIR_UP BUTTON_SCROLL_UP
#define BTN_DIR_DOWN BUTTON_SCROLL_DOWN
#define BTN_DIR_LEFT BUTTON_LEFT
#define BTN_DIR_RIGHT BUTTON_RIGHT
#define BTN_STARTPAUSE BUTTON_PLAY
#define BTN_QUIT BUTTON_POWER
#define BTN_STOPRESET BUTTON_REW

#elif (CONFIG_KEYPAD == GIGABEAT_S_PAD) || \
      (CONFIG_KEYPAD == SAMSUNG_YPR0_PAD)

#define BTN_DIR_UP BUTTON_UP
#define BTN_DIR_DOWN BUTTON_DOWN
#define BTN_DIR_LEFT BUTTON_LEFT
#define BTN_DIR_RIGHT BUTTON_RIGHT
#define BTN_STARTPAUSE BUTTON_SELECT
#define BTN_QUIT BUTTON_BACK
#define BTN_STOPRESET BUTTON_MENU

#elif (CONFIG_KEYPAD == MROBE100_PAD)

#define BTN_DIR_UP BUTTON_UP
#define BTN_DIR_DOWN BUTTON_DOWN
#define BTN_DIR_LEFT BUTTON_LEFT
#define BTN_DIR_RIGHT BUTTON_RIGHT
#define BTN_STARTPAUSE BUTTON_SELECT
#define BTN_QUIT BUTTON_POWER
#define BTN_STOPRESET BUTTON_DISPLAY

#elif CONFIG_KEYPAD == IAUDIO_M3_PAD

#define BTN_DIR_UP BUTTON_RC_VOL_UP
#define BTN_DIR_DOWN BUTTON_RC_VOL_DOWN
#define BTN_DIR_LEFT BUTTON_RC_REW
#define BTN_DIR_RIGHT BUTTON_RC_FF
#define BTN_STARTPAUSE BUTTON_RC_PLAY
#define BTN_QUIT BUTTON_RC_REC
#define BTN_STOPRESET BUTTON_RC_MODE

#elif (CONFIG_KEYPAD == COWON_D2_PAD)

#define BTN_QUIT BUTTON_POWER

#elif CONFIG_KEYPAD == CREATIVEZVM_PAD

#define BTN_DIR_UP BUTTON_UP
#define BTN_DIR_DOWN BUTTON_DOWN
#define BTN_DIR_LEFT BUTTON_LEFT
#define BTN_DIR_RIGHT BUTTON_RIGHT
#define BTN_STARTPAUSE BUTTON_PLAY
#define BTN_QUIT BUTTON_BACK
#define BTN_STOPRESET BUTTON_MENU

#elif CONFIG_KEYPAD == CREATIVE_ZENXFI3_PAD

#define BTN_DIR_UP BUTTON_UP
#define BTN_DIR_DOWN BUTTON_DOWN
#define BTN_DIR_LEFT BUTTON_BACK
#define BTN_DIR_RIGHT BUTTON_MENU
#define BTN_STARTPAUSE (BUTTON_PLAY|BUTTON_REL)
#define BTN_QUIT BUTTON_POWER
#define BTN_STOPRESET (BUTTON_PLAY|BUTTON_REPEAT)

#elif CONFIG_KEYPAD == PHILIPS_HDD1630_PAD

#define BTN_DIR_UP BUTTON_UP
#define BTN_DIR_DOWN BUTTON_DOWN
#define BTN_DIR_LEFT BUTTON_LEFT
#define BTN_DIR_RIGHT BUTTON_RIGHT
#define BTN_STARTPAUSE BUTTON_MENU
#define BTN_QUIT BUTTON_POWER
#define BTN_STOPRESET BUTTON_VIEW

#elif CONFIG_KEYPAD == PHILIPS_HDD6330_PAD

#define BTN_DIR_UP BUTTON_UP
#define BTN_DIR_DOWN BUTTON_DOWN
#define BTN_DIR_LEFT BUTTON_PREV
#define BTN_DIR_RIGHT BUTTON_NEXT
#define BTN_STARTPAUSE BUTTON_MENU
#define BTN_QUIT BUTTON_POWER
#define BTN_STOPRESET BUTTON_RIGHT

#elif CONFIG_KEYPAD == PHILIPS_SA9200_PAD

#define BTN_DIR_UP BUTTON_UP
#define BTN_DIR_DOWN BUTTON_DOWN
#define BTN_DIR_LEFT BUTTON_PREV
#define BTN_DIR_RIGHT BUTTON_RIGHT
#define BTN_STARTPAUSE BUTTON_MENU
#define BTN_QUIT BUTTON_POWER
#define BTN_STOPRESET BUTTON_RIGHT

#elif (CONFIG_KEYPAD == ONDAVX747_PAD) || \
(CONFIG_KEYPAD == ONDAVX777_PAD) || \
CONFIG_KEYPAD == MROBE500_PAD

#define BTN_QUIT BUTTON_POWER

#elif CONFIG_KEYPAD == SAMSUNG_YH_PAD

#define BTN_DIR_UP     BUTTON_UP
#define BTN_DIR_DOWN   BUTTON_DOWN
#define BTN_DIR_LEFT   BUTTON_LEFT
#define BTN_DIR_RIGHT  BUTTON_RIGHT
#define BTN_STARTPAUSE BUTTON_PLAY
#define BTN_QUIT       BUTTON_FFWD
#define BTN_STOPRESET  BUTTON_REW

#elif CONFIG_KEYPAD == PBELL_VIBE500_PAD

#define BTN_DIR_UP     BUTTON_UP
#define BTN_DIR_DOWN   BUTTON_DOWN
#define BTN_DIR_LEFT   BUTTON_PREV
#define BTN_DIR_RIGHT  BUTTON_NEXT
#define BTN_STARTPAUSE BUTTON_PLAY
#define BTN_QUIT       BUTTON_REC
#define BTN_STOPRESET  BUTTON_CANCEL

#elif CONFIG_KEYPAD == MPIO_HD200_PAD

#define BTN_DIR_UP BUTTON_REW
#define BTN_DIR_DOWN BUTTON_FF
#define BTN_DIR_LEFT BUTTON_VOL_DOWN
#define BTN_DIR_RIGHT BUTTON_VOL_UP
#define BTN_STARTPAUSE BUTTON_FUNC
#define BTN_QUIT (BUTTON_REC|BUTTON_PLAY)
#define BTN_STOPRESET (BUTTON_FUNC|BUTTON_REPEAT)

#elif CONFIG_KEYPAD == MPIO_HD300_PAD

#define BTN_DIR_UP BUTTON_UP
#define BTN_DIR_DOWN BUTTON_DOWN
#define BTN_DIR_LEFT BUTTON_REW
#define BTN_DIR_RIGHT BUTTON_FF
#define BTN_STARTPAUSE BUTTON_PLAY
#define BTN_QUIT (BUTTON_MENU | BUTTON_REPEAT)
#define BTN_STOPRESET (BUTTON_PLAY | BUTTON_REPEAT)

#elif CONFIG_KEYPAD == SANSA_FUZEPLUS_PAD

#define BTN_DIR_UP BUTTON_UP
#define BTN_DIR_DOWN BUTTON_DOWN
#define BTN_DIR_LEFT BUTTON_LEFT
#define BTN_DIR_RIGHT BUTTON_RIGHT
#define BTN_STARTPAUSE BUTTON_PLAYPAUSE
#define BTN_QUIT BUTTON_POWER
#define BTN_STOPRESET BUTTON_BACK

#elif CONFIG_KEYPAD == SANSA_CONNECT_PAD

#define BTN_DIR_UP BUTTON_UP
#define BTN_DIR_DOWN BUTTON_DOWN
#define BTN_DIR_LEFT BUTTON_LEFT
#define BTN_DIR_RIGHT BUTTON_RIGHT
#define BTN_STARTPAUSE BUTTON_SELECT
#define BTN_QUIT BUTTON_POWER
#define BTN_STOPRESET BUTTON_VOL_DOWN

#elif CONFIG_KEYPAD == HM60X_PAD

#define BTN_DIR_UP     BUTTON_UP
#define BTN_DIR_DOWN   BUTTON_DOWN
#define BTN_DIR_LEFT   BUTTON_LEFT
#define BTN_DIR_RIGHT  BUTTON_RIGHT
#define BTN_STARTPAUSE BUTTON_SELECT
#define BTN_QUIT       BUTTON_POWER
#define BTN_STOPRESET  (BUTTON_POWER|BUTTON_SELECT)

#elif CONFIG_KEYPAD == HM801_PAD

#define BTN_DIR_UP     BUTTON_UP
#define BTN_DIR_DOWN   BUTTON_DOWN
#define BTN_DIR_LEFT   BUTTON_LEFT
#define BTN_DIR_RIGHT  BUTTON_RIGHT
#define BTN_STARTPAUSE BUTTON_SELECT
#define BTN_QUIT       BUTTON_POWER
#define BTN_STOPRESET  BUTTON_PLAY

#else
#error No keymap defined!
#endif

#ifdef HAVE_TOUCHSCREEN
#ifndef BTN_DIR_UP
#define BTN_DIR_UP     BUTTON_TOPMIDDLE
#endif
#ifndef BTN_DIR_DOWN
#define BTN_DIR_DOWN   BUTTON_BOTTOMMIDDLE
#endif
#ifndef BTN_DIR_LEFT
#define BTN_DIR_LEFT   BUTTON_MIDLEFT
#endif
#ifndef BTN_DIR_RIGHT
#define BTN_DIR_RIGHT  BUTTON_MIDRIGHT
#endif
#ifndef BTN_STARTPAUSE
#define BTN_STARTPAUSE BUTTON_CENTER
#endif
#ifndef BTN_QUIT
#define BTN_QUIT       BUTTON_TOPLEFT
#endif
#ifndef BTN_STOPRESET
#define BTN_STOPRESET  BUTTON_TOPRIGHT
#endif
#endif

#if (LCD_WIDTH == 96) && (LCD_HEIGHT == 96)
#define FOOD_SIZE 3
#define ARGH_SIZE 4
#define SPEED 14
#define MAX_WORM_SEGMENTS 128
#elif (LCD_WIDTH == 112) && (LCD_HEIGHT == 64)
#define FOOD_SIZE 3
#define ARGH_SIZE 4
#define SPEED 14
#define MAX_WORM_SEGMENTS 128
#elif (LCD_WIDTH == 128) && (LCD_HEIGHT == 64)
#define FOOD_SIZE 3
#define ARGH_SIZE 4
#define SPEED 14
#define MAX_WORM_SEGMENTS 128
#elif (LCD_WIDTH == 132) && (LCD_HEIGHT == 80)
#define FOOD_SIZE 3
#define ARGH_SIZE 4
#define SPEED 14
#define MAX_WORM_SEGMENTS 128
#elif (LCD_WIDTH == 128) && (LCD_HEIGHT == 96)
#define FOOD_SIZE 3
#define ARGH_SIZE 4
#define SPEED 12
#define MAX_WORM_SEGMENTS 128
#elif (LCD_WIDTH == 138) && (LCD_HEIGHT == 110)
#define FOOD_SIZE 4
#define ARGH_SIZE 5
#define SPEED 10
#define MAX_WORM_SEGMENTS 128
#elif (LCD_WIDTH == 128) && (LCD_HEIGHT == 128)
#define FOOD_SIZE 4
#define ARGH_SIZE 5
#define SPEED 9
#define MAX_WORM_SEGMENTS 128
#elif ((LCD_WIDTH == 160) && (LCD_HEIGHT == 128)) || \
      ((LCD_WIDTH == 128) && (LCD_HEIGHT == 160))
#define FOOD_SIZE 4
#define ARGH_SIZE 5
#define SPEED 8
#define MAX_WORM_SEGMENTS 256
#elif (LCD_WIDTH == 176) && (LCD_HEIGHT == 132)
#define FOOD_SIZE 4
#define ARGH_SIZE 5
#define SPEED 6
#define MAX_WORM_SEGMENTS 256
#elif (LCD_WIDTH == 220) && (LCD_HEIGHT == 176)
#define FOOD_SIZE 5
#define ARGH_SIZE 6
#define SPEED 4
#define MAX_WORM_SEGMENTS 512
#elif (LCD_WIDTH == 176) && (LCD_HEIGHT == 220)
#define FOOD_SIZE 5
#define ARGH_SIZE 6
#define SPEED 4
#define MAX_WORM_SEGMENTS 512
#elif ((LCD_WIDTH == 320) && (LCD_HEIGHT == 240)) || \
    ((LCD_WIDTH == 240) && ((LCD_HEIGHT == 320) || (LCD_HEIGHT == 400)))
#define FOOD_SIZE 7
#define ARGH_SIZE 8
#define SPEED 4
#define MAX_WORM_SEGMENTS 512
#elif   ((LCD_WIDTH == 640) && (LCD_HEIGHT == 480)) || \
        ((LCD_WIDTH == 480) && (LCD_HEIGHT == 640))
#define FOOD_SIZE 14
#define ARGH_SIZE 16
#define SPEED 4
#define MAX_WORM_SEGMENTS 512
#endif

#ifdef HAVE_LCD_COLOR
#define COLOR_WORM LCD_RGBPACK(80, 40, 0)
#define COLOR_ARGH LCD_RGBPACK(175, 0, 0)
#define COLOR_FOOD LCD_RGBPACK(0, 150, 0)
#define COLOR_FG   LCD_RGBPACK(0, 0, 0)
#define COLOR_BG   LCD_RGBPACK(181, 199, 231)
#endif

#define CHECK_SQUARE_COLLISION(x1,y1,s1,x2,y2,s2) (x1+s1>x2)&&(x2+s2>x1)&&(y1+s1>y2)&&(y2+s2>y1)

/**
 * All the properties that a worm has.
 */
static struct worm {
    /* The worm is stored in a ring of xy coordinates */
    int x[MAX_WORM_SEGMENTS];
    int y[MAX_WORM_SEGMENTS];

    int head;      /* index of the head within the buffer */
    int tail;      /* index of the tail within the buffer */
    int growing;   /* number of cyles the worm still keeps growing */
    bool alive;    /* the worms living state */

    /* direction vector in which the worm moves */
    int dirx; /* only values -1 0 1 allowed */
    int diry; /* only values -1 0 1 allowed */

    /* this method is used to fetch the direction the user
       has selected. It can be one of the values
       human_player1, human_player2, remote_player, virtual_player.
       All these values are fuctions, that can change the direction
       of the worm */
    void (*fetch_worm_direction)(struct worm *w);
} worms[MAX_WORMS];

/* stores the highscore - besides it was scored by a virtual player */
static int highscore;

#define MAX_FOOD 5 /* maximal number of food items */

/* The arrays store the food coordinates  */
static int foodx[MAX_FOOD];
static int foody[MAX_FOOD];

#define MAX_ARGH 100    /* maximal number of argh items */
#define ARGHS_PER_FOOD 2 /* number of arghs produced per eaten food */

/* The arrays store the argh coordinates */
static int arghx[MAX_ARGH];
static int arghy[MAX_ARGH];

/* the number of arghs that are currently in use */
static int argh_count;

/* the number of arghs per food, settable by user */
static int arghs_per_food = ARGHS_PER_FOOD;
/* the size of the argh, settable by user */
static int argh_size = ARGH_SIZE;
/* the size of the food, settable by user */
static int food_size = FOOD_SIZE;
/* the speed of the worm, settable by user */
static int speed = SPEED;
/* the amount a worm grows by eating a food, settable by user */
static int worm_food = WORM_PER_FOOD;

/* End additional variables */

/* the number of active worms (dead or alive) */
static int worm_count = MAX_WORMS;

/* in multiplayer mode: en- / disables the remote worm control
   in singleplayer mode: toggles 4 / 2 button worm control */
static bool use_remote = false;

/* return values of check_collision */
#define COLLISION_NONE 0
#define COLLISION_WORM 1
#define COLLISION_FOOD 2
#define COLLISION_ARGH 3
#define COLLISION_FIELD 4

static const char *const state_desc[] = {
    [COLLISION_NONE]  = NULL,
    [COLLISION_WORM]  = "Wormed",
    [COLLISION_FOOD]  = "Growing",
    [COLLISION_ARGH]  = "Argh",
    [COLLISION_FIELD] = "Crashed",
};

/* constants for use as directions.
   Note that the values are ordered clockwise.
   Thus increasing / decreasing the values
   is equivalent to right / left turns. */
#define WEST  0
#define NORTH 1
#define EAST  2
#define SOUTH 3

/* direction of human player 1 */
static int player1_dir = EAST;
/* direction of human player 2 */
static int player2_dir = EAST;
/* direction of human player 3 */
static int player3_dir = EAST;

/* the number of (human) players that currently
   control a worm */
static int players = 1;

#define SETTINGS_VERSION 1
#define SETTINGS_MIN_VERSION 1
#define SETTINGS_FILENAME "wormlet.cfg"

static struct configdata config[] =
{
    {TYPE_INT, 0, 1024, { .int_p = &highscore }, "highscore", NULL},
    {TYPE_INT, 0, 15, { .int_p = &arghs_per_food }, "arghs per food", NULL},
    {TYPE_INT, 0, 15, { .int_p = &argh_size }, "argh size", NULL},
    {TYPE_INT, 0, 15, { .int_p = &food_size }, "food size", NULL},
    {TYPE_INT, 0, 3, { .int_p = &players }, "players", NULL},
    {TYPE_INT, 0, 3, { .int_p = &worm_count }, "worms", NULL},
    {TYPE_INT, 0, 20, { .int_p = &speed }, "speed", NULL},
    {TYPE_INT, 0, 15, { .int_p = &worm_food }, "Worm Growth Per Food", NULL}
};

/**
 * Returns the direction id in which the worm
 * currently is creeping.
 * @param struct worm *w The worm that is to be investigated.
 *        w Must not be null.
 * @return int A value 0 <= value < 4
 *         Note the predefined constants NORTH, SOUTH, EAST, WEST
 */
static int get_worm_dir(struct worm *w)
{
    int retVal ;
    if (w->dirx == 0) {
        if (w->diry == 1) {
            retVal = SOUTH;
        } else {
            retVal = NORTH;
        }
    } else {
        if (w->dirx == 1) {
            retVal = EAST;
        } else {
            retVal = WEST;
        }
    }
    return retVal;
}

/**
 * Set the direction of the specified worm with a direction id.
 * Increasing the value by 1 means to turn the worm direction
 * to right by 90 degree.
 * @param struct worm *w The worm that is to be altered. w Must not be null.
 * @param int dir The new direction in which the worm is to creep.
 *        dir must be  0 <= dir < 4. Use predefined constants
 *        NORTH, SOUTH, EAST, WEST
 */
static void set_worm_dir(struct worm *w, int dir)
{
    switch (dir) {
        case WEST:
            w->dirx = -1;
            w->diry = 0;
            break;
        case NORTH:
            w->dirx = 0;
            w->diry = - 1;
            break;
        case EAST:
            w->dirx = 1;
            w->diry = 0;
            break;
        case SOUTH:
            w->dirx = 0;
            w->diry = 1;
            break;
    }
}

/**
 * Returns the current length of the worm array. This
 * is also a value for the number of bends that are in the worm.
 * @return int a positive value with 0 <= value < MAX_WORM_SEGMENTS
 */
static int get_worm_array_length(struct worm *w)
{
    /* initial simple calculation will be overwritten if wrong. */
    int retVal = w->head - w->tail;

    /* if the worm 'crosses' the boundaries of the ringbuffer */
    if (retVal < 0) {
        retVal = w->head + MAX_WORM_SEGMENTS - w->tail;
    }

    return retVal;
}

/**
 * Returns the score the specified worm. The score is the length
 * of the worm.
 * @param struct worm *w The worm that is to be investigated.
 *        w must not be null.
 * @return int The length of the worm (>= 0).
 */
static int get_score(struct worm *w)
{
    int retval = 0;
    int length = get_worm_array_length(w);
    int i;
    for (i = 0; i < length; i++) {

        /* The iteration iterates the length of the worm.
           Here's the conversion to the true indices within the worm arrays. */
        int linestart = (w->tail + i  ) % MAX_WORM_SEGMENTS;
        int lineend   = (linestart + 1) % MAX_WORM_SEGMENTS;
        int startx = w->x[linestart];
        int starty = w->y[linestart];
        int endx = w->x[lineend];
        int endy = w->y[lineend];

        int minimum, maximum;

        if (startx == endx) {
            minimum = MIN(starty, endy);
            maximum = MAX(starty, endy);
        } else {
            minimum = MIN(startx, endx);
            maximum = MAX(startx, endx);
        }
        retval += abs(maximum - minimum);
    }
    return retval;
}

/**
 * Determines wether the line specified by startx, starty, endx, endy intersects
 * the rectangle specified by x, y, width, height. Note that the line must be exactly
 * horizontal or vertical (startx == endx or starty == endy).
 * @param int startx The x coordinate of the start point of the line.
 * @param int starty The y coordinate of the start point of the line.
 * @param int endx The x coordinate of the end point of the line.
 * @param int endy The y coordinate of the end point of the line.
 * @param int x The x coordinate of the top left corner of the rectangle.
 * @param int y The y coordinate of the top left corner of the rectangle.
 * @param int width The width of the rectangle.
 * @param int height The height of the rectangle.
 * @return bool Returns true if the specified line intersects with the recangle.
 */
static bool line_in_rect(int startx, int starty, int endx, int endy,
                         int x, int y, int width, int height)
{
    bool retval = false;
    int simple, simplemin, simplemax;
    int compa, compb, compmin, compmax;
    int temp;
    if (startx == endx) {
        simple = startx;
        simplemin = x;
        simplemax = x + width;

        compa = starty;
        compb = endy;
        compmin = y;
        compmax = y + height;
    } else {
        simple = starty;
        simplemin = y;
        simplemax = y + height;

        compa = startx;
        compb = endx;
        compmin = x;
        compmax = x + width;
    };

    temp = compa;
    compa = MIN(compa, compb);
    compb = MAX(temp, compb);

    if (simplemin <= simple && simple <= simplemax) {
        if ((compmin <= compa && compa <= compmax) ||
            (compmin <= compb && compb <= compmax) ||
            (compa <= compmin && compb >= compmax)) {
            retval = true;
        }
    }
    return retval;
}

/**
 * Tests wether the specified worm intersects with the rect.
 * @param struct worm *w The worm to be investigated
 * @param int x The x coordinate of the top left corner of the rect
 * @param int y The y coordinate of the top left corner of the rect
 * @param int widht The width of the rect
 * @param int height The height of the rect
 * @return bool Returns true if the worm intersects with the rect
 */
static bool worm_in_rect(struct worm *w, int x, int y, int width, int height)
{
    bool retval = false;


    /* get_worm_array_length is expensive -> buffer the value */
    int wormLength = get_worm_array_length(w);
    int i;

    /* test each entry that is part of the worm */
    for (i = 0; i < wormLength && retval == false; i++) {

        /* The iteration iterates the length of the worm.
           Here's the conversion to the true indices within the worm arrays. */
        int linestart = (w->tail + i  ) % MAX_WORM_SEGMENTS;
        int lineend   = (linestart + 1) % MAX_WORM_SEGMENTS;
        int startx = w->x[linestart];
        int starty = w->y[linestart];
        int endx = w->x[lineend];
        int endy = w->y[lineend];

        retval = line_in_rect(startx, starty, endx, endy, x, y, width, height);
    }

    return retval;
}

/**
 * Checks wether a specific food in the food arrays is at the
 * specified coordinates.
 * @param int foodIndex The index of the food in the food arrays
 * @param int x the x coordinate.
 * @param int y the y coordinate.
 * @return Returns true if the coordinate hits the food specified by
 * foodIndex.
 */
static bool specific_food_collision(int foodIndex, int x, int y)
{
    bool retVal = false;
    if (x >= foodx[foodIndex]             &&
        x <  foodx[foodIndex] + food_size &&
        y >= foody[foodIndex]             &&
        y <  foody[foodIndex] + food_size) {

        retVal = true;
    }
    return retVal;
}

/**
 * Returns the index of the food that is at the
 * given coordinates. If no food is at the coordinates
 * -1 is returned.
 * @return int  -1 <= value < MAX_FOOD
 */
static int food_collision(int x, int y)
{
    int i = 0;
    int retVal = -1;
    for (i = 0; i < MAX_FOOD; i++) {
        if (specific_food_collision(i, x, y)) {
            retVal = i;
            break;
        }
    }
    return retVal;
}

/**
 * Checks wether a specific argh in the argh arrays is at the
 * specified coordinates.
 * @param int arghIndex The index of the argh in the argh arrays
 * @param int x the x coordinate.
 * @param int y the y coordinate.
 * @return Returns true if the coordinate hits the argh specified by
 * arghIndex.
 */
static bool specific_argh_collision(int arghIndex, int x, int y)
{
    if ( x >= arghx[arghIndex] &&
         y >= arghy[arghIndex] &&
         x <  arghx[arghIndex] + argh_size &&
         y <  arghy[arghIndex] + argh_size )
    {
        return true;
    }

    return false;
}

/**
 * Returns the index of the argh that is at the
 * given coordinates. If no argh is at the coordinates
 * -1 is returned.
 * @param int x The x coordinate.
 * @param int y The y coordinate.
 * @return int  -1 <= value < argh_count <= MAX_ARGH
 */
static int argh_collision(int x, int y)
{
    int i = 0;
    int retVal = -1;

    /* search for the argh that has the specified coords */
    for (i = 0; i < argh_count; i++) {
        if (specific_argh_collision(i, x, y)) {
            retVal = i;
            break;
        }
    }
    return retVal;
}

/**
 * Checks wether the worm collides with the food at the specfied food-arrays.
 * @param int foodIndex The index of the food in the arrays. Ensure the value is
 * 0 <= foodIndex <= MAX_FOOD
 * @return Returns true if the worm collides with the specified food.
 */
static bool worm_food_collision(struct worm *w, int foodIndex)
{
    bool retVal = false;

    retVal = worm_in_rect(w, foodx[foodIndex], foody[foodIndex],
                          food_size - 1, food_size - 1);

    return retVal;
}

/**
 * Returns true if the worm hits the argh within the next moves (unless
 * the worm changes it's direction).
 * @param struct worm *w - The worm to investigate
 * @param int argh_idx - The index of the argh
 * @param int moves - The number of moves that are considered.
 * @return Returns false if the specified argh is not hit within the next
 *         moves.
 */
static bool worm_argh_collision_in_moves(struct worm *w, int argh_idx, int moves)
{
    bool retVal = false;
    int x1, y1, x2, y2;
    x1 = w->x[w->head];
    y1 = w->y[w->head];

    x2 = w->x[w->head] + moves * w->dirx;
    y2 = w->y[w->head] + moves * w->diry;

    retVal = line_in_rect(x1, y1, x2, y2, arghx[argh_idx], arghy[argh_idx],
                         argh_size, argh_size);
    return retVal;
}

/**
 * Checks wether the worm collides with the argh at the specfied argh-arrays.
 * @param int arghIndex The index of the argh in the arrays.
 * Ensure the value is 0 <= arghIndex < argh_count <= MAX_ARGH
 * @return Returns true if the worm collides with the specified argh.
 */
static bool worm_argh_collision(struct worm *w, int arghIndex)
{
    bool retVal = false;

    retVal = worm_in_rect(w, arghx[arghIndex], arghy[arghIndex],
                          argh_size - 1, argh_size - 1);

    return retVal;
}

/**
 * Find new coordinates for the food stored in foodx[index], foody[index]
 * that don't collide with any other food or argh
 * @param int index
 * Ensure that 0 <= index < MAX_FOOD.
 */
static void make_food(int index)
{
    int x = 0;
    int y = 0;
    bool collisionDetected = false;
    int i;

    do {
        /* make coordinates for a new food so that
           the entire food lies within the FIELD */
        x = rb->rand() % (FIELD_RECT_WIDTH  - food_size);
        y = rb->rand() % (FIELD_RECT_HEIGHT - food_size);
        collisionDetected = false;
        /* Ensure that the new food doesn't collide with any
           existing foods or arghs.
           If the new food hit any existing
           argh or food a collision is detected.
        */

        for (i=0; i<MAX_FOOD && !collisionDetected; i++) {
            collisionDetected = CHECK_SQUARE_COLLISION(x,y,food_size,foodx[i],foody[i],food_size);
        }
        for (i=0; i<argh_count && !collisionDetected; i++) {
            collisionDetected = CHECK_SQUARE_COLLISION(x,y,food_size,arghx[i],arghy[i],argh_size);
        }

        /* use coordinates for further testing */
        foodx[index] = x;
        foody[index] = y;

        /* now test wether we accidently hit the worm with food ;) */
        i = 0;
        for (i = 0; i < worm_count && !collisionDetected; i++) {

            collisionDetected = worm_food_collision(&worms[i], index);

        }
    }
    while (collisionDetected);
    return;
}

/**
 * Clears a food from the lcd buffer.
 * @param int index The index of the food arrays under which
 * the coordinates of the desired food can be found. Ensure
 * that the value is 0 <= index <= MAX_FOOD.
 */
static void clear_food(int index)
{
    /* remove the old food from the screen */
    rb->lcd_set_drawmode(DRMODE_SOLID|DRMODE_INVERSEVID);
    rb->lcd_fillrect(foodx[index] + FIELD_RECT_X,
                     foody[index] + FIELD_RECT_Y,
                     food_size, food_size);
    rb->lcd_set_drawmode(DRMODE_SOLID);
}

/**
 * Draws a food in the lcd buffer.
 * @param int index The index of the food arrays under which
 * the coordinates of the desired food can be found. Ensure
 * that the value is 0 <= index <= MAX_FOOD.
 */
static void draw_food(int index)
{
    /* draw the food object */
#ifdef HAVE_LCD_COLOR
    rb->lcd_set_foreground(COLOR_FOOD);
#endif
    rb->lcd_fillrect(foodx[index] + FIELD_RECT_X,
                     foody[index] + FIELD_RECT_Y,
                     food_size, food_size);
    rb->lcd_set_drawmode(DRMODE_SOLID|DRMODE_INVERSEVID);
    rb->lcd_fillrect(foodx[index] + FIELD_RECT_X + 1,
                     foody[index] + FIELD_RECT_Y + 1,
                     food_size - 2, food_size - 2);
    rb->lcd_set_drawmode(DRMODE_SOLID);
#ifdef HAVE_LCD_COLOR
    rb->lcd_set_foreground(COLOR_FG);
#endif
}

/**
 * Find new coordinates for the argh stored in arghx[index], arghy[index]
 * that don't collide with any other food or argh.
 * @param int index
 * Ensure that 0 <= index < argh_count < MAX_ARGH.
 */
static void make_argh(int index)
{
    int x = -1;
    int y = -1;
    bool collisionDetected = false;
    int i;

    do {
        /* make coordinates for a new argh so that
           the entire food lies within the FIELD */
        x = rb->rand() % (FIELD_RECT_WIDTH  - argh_size);
        y = rb->rand() % (FIELD_RECT_HEIGHT - argh_size);
        collisionDetected = false;
        /* Ensure that the new argh doesn't intersect with any
           existing foods or arghs.
           If the new argh hit any existing
           argh or food an intersection is detected.
        */

        for (i=0; i<MAX_FOOD && !collisionDetected; i++) {
            collisionDetected = CHECK_SQUARE_COLLISION(x,y,argh_size,foodx[i],foody[i],food_size);
        }
        for (i=0; i<argh_count && !collisionDetected; i++) {
            collisionDetected = CHECK_SQUARE_COLLISION(x,y,argh_size,arghx[i],arghy[i],argh_size);
        }

        /* use the candidate coordinates to make a real argh */
        arghx[index] = x;
        arghy[index] = y;

        /* now test wether we accidently hit the worm with argh ;) */
        for (i = 0; i < worm_count && !collisionDetected; i++) {
            collisionDetected |= worm_argh_collision(&worms[i], index);
            collisionDetected |= worm_argh_collision_in_moves(&worms[i], index,
                                                              MIN_ARGH_DIST);
        }
    }
    while (collisionDetected);
    return;
}

/**
 * Draws an argh in the lcd buffer.
 * @param int index The index of the argh arrays under which
 * the coordinates of the desired argh can be found. Ensure
 * that the value is 0 <= index < argh_count <= MAX_ARGH.
 */
static void draw_argh(int index)
{
    /* draw the new argh */
#ifdef HAVE_LCD_COLOR
    rb->lcd_set_foreground(COLOR_ARGH);
#endif
    rb->lcd_fillrect(arghx[index] + FIELD_RECT_X,
                     arghy[index] + FIELD_RECT_Y,
                     argh_size, argh_size);
#ifdef HAVE_LCD_COLOR
    rb->lcd_set_foreground(COLOR_FG);
#endif
}

static void virtual_player(struct worm *w);
/**
 * Initialzes the specified worm with INITIAL_WORM_LENGTH
 * and the tail at the specified position. The worm will
 * be initialized alive and creeping EAST.
 * @param struct worm *w The worm that is to be initialized
 * @param int x The x coordinate at which the tail of the worm starts.
 *        x must be 0 <= x < FIELD_RECT_WIDTH.
 * @param int y The y coordinate at which the tail of the worm starts
 *        y must be 0 <= y < FIELD_RECT_WIDTH.
 */
static void init_worm(struct worm *w, int x, int y)
{
    /* initialize the worm size */
    w->head = 1;
    w->tail = 0;

    w->x[w->head] = x + 1;
    w->y[w->head] = y;

    w->x[w->tail] = x;
    w->y[w->tail] = y;

    /* set the initial direction the worm creeps to */
    w->dirx = 1;
    w->diry = 0;

    w->growing = INITIAL_WORM_LENGTH - 1;
    w->alive = true;
    w->fetch_worm_direction = virtual_player;
}

/**
 * Writes the direction that was stored for
 * human player 1 into the specified worm. This function
 * may be used to be stored in worm.fetch_worm_direction.
 * The value of
 * the direction is read from player1_dir.
 * @param struct worm *w - The worm of which the direction
 * is altered.
 */
static void human_player1(struct worm *w) {
    set_worm_dir(w, player1_dir);
}

/**
 * Writes the direction that was stored for
 * human player 2 into the specified worm. This function
 * may be used to be stored in worm.fetch_worm_direction.
 * The value of
 * the direction is read from player2_dir.
 * @param struct worm *w - The worm of which the direction
 * is altered.
 */
static void human_player2(struct worm *w) {
    set_worm_dir(w, player2_dir);
}

/**
 * Writes the direction that was stored for
 * human player using a remote control
 * into the specified worm. This function
 * may be used to be stored in worm.fetch_worm_direction.
 * The value of
 * the direction is read from player3_dir.
 * @param struct worm *w - The worm of which the direction
 * is altered.
 */
static void remote_player(struct worm *w) {
    set_worm_dir(w, player3_dir);
}

/**
 * Initializes the worm-, food- and argh-arrays, draws a frame,
 * makes some food and argh and display all that stuff.
 */
static void init_wormlet(void)
{
    int i;

    for (i = 0; i< worm_count; i++) {
        /* Initialize all the worm coordinates to center. */
        int x = (int)(FIELD_RECT_WIDTH / 2);
        int y = (int)((FIELD_RECT_HEIGHT - 20)/ 2) + i * 10;

        init_worm(&worms[i], x, y);
    }

    player1_dir = EAST;
    player2_dir = EAST;
    player3_dir = EAST;

    if (players > 0) {
        worms[0].fetch_worm_direction = human_player1;
    }

    if (players > 1) {
        if (use_remote) {
            worms[1].fetch_worm_direction = remote_player;
        } else {
            worms[1].fetch_worm_direction = human_player2;
        }
    }

    if (players > 2) {
        worms[2].fetch_worm_direction = human_player2;
    }

    /* Needed when the game is restarted using BTN_STOPRESET */
    rb->lcd_clear_display();

    /* make and display some food and argh */
    argh_count = MAX_FOOD;
    for (i = 0; i < MAX_FOOD; i++) {
        make_food(i);
        draw_food(i);
        make_argh(i);
        draw_argh(i);
    }

    /* draw the game field */
    rb->lcd_set_drawmode(DRMODE_COMPLEMENT);
    rb->lcd_fillrect(0, 0, FIELD_RECT_WIDTH + 2, FIELD_RECT_HEIGHT + 2);
    rb->lcd_fillrect(1, 1, FIELD_RECT_WIDTH, FIELD_RECT_HEIGHT);
    rb->lcd_set_drawmode(DRMODE_SOLID);

    /* make everything visible */
    rb->lcd_update();
}


/**
 * Move the worm one step further if it is alive.
 * The direction in which the worm moves is taken from dirx and diry.
 * move_worm decreases growing if > 0. While the worm is growing the tail
 * is left untouched.
 * @param struct worm *w The worm to move. w must not be NULL.
 */
static void move_worm(struct worm *w)
{
    if (w->alive) {
        /* determine the head point and its precessor */
        int headx = w->x[w->head];
        int heady = w->y[w->head];
        int prehead = (w->head + MAX_WORM_SEGMENTS - 1) % MAX_WORM_SEGMENTS;
        int preheadx = w->x[prehead];
        int preheady = w->y[prehead];

        /* determine the old direction */
        int olddirx;
        int olddiry;
        if (headx == preheadx) {
            olddirx = 0;
            olddiry = (heady > preheady) ? 1 : -1;
        } else {
            olddiry = 0;
            olddirx = (headx > preheadx) ? 1 : -1;
        }

        /* olddir == dir?
           a change of direction means a new segment
           has been opened */
        if (olddirx != w->dirx ||
            olddiry != w->diry) {
            w->head = (w->head + 1) % MAX_WORM_SEGMENTS;
        }

        /* new head position */
        w->x[w->head] = headx + w->dirx;
        w->y[w->head] = heady + w->diry;


        /* while the worm is growing no tail procession is necessary */
        if (w->growing > 0) {
    /* update the worms grow state */
            w->growing--;
        }

        /* if the worm isn't growing the tail has to be dragged */
        else {
            /* index of the end of the tail segment */
            int tail_segment_end = (w->tail + 1) % MAX_WORM_SEGMENTS;

            /* drag the end of the tail */
            /* only one coordinate has to be altered. Here it is
               determined which one */
            int dir = 0; /* specifies wether the coord has to be in- or decreased */
            if (w->x[w->tail] == w->x[tail_segment_end]) {
                dir = (w->y[w->tail] - w->y[tail_segment_end] < 0) ? 1 : -1;
                w->y[w->tail] += dir;
            } else {
                dir = (w->x[w->tail] - w->x[tail_segment_end] < 0) ? 1 : -1;
                w->x[w->tail] += dir;
            }

            /* when the tail has been dragged so far that it meets
               the next segment start the tail segment is obsolete and
               must be freed */
            if (w->x[w->tail] == w->x[tail_segment_end] &&
                w->y[w->tail] == w->y[tail_segment_end]){

                /* drop the last tail point */
                w->tail = tail_segment_end;
            }
        }
    }
}

/**
 * Draws the head and clears the tail of the worm in
 * the display buffer. lcd_update() is NOT called thus
 * the caller has to take care that the buffer is displayed.
 */
static void draw_worm(struct worm *w)
{
    /* draw the new head */
    int x = w->x[w->head];
    int y = w->y[w->head];
#ifdef HAVE_LCD_COLOR
    rb->lcd_set_foreground(COLOR_WORM);
#endif
    if (x >= 0 && x < FIELD_RECT_WIDTH && y >= 0 && y < FIELD_RECT_HEIGHT) {
        rb->lcd_drawpixel(x + FIELD_RECT_X, y + FIELD_RECT_Y);
    }

    rb->lcd_set_drawmode(DRMODE_SOLID|DRMODE_INVERSEVID);

    /* clear the space behind the worm */
    x = w->x[w->tail] ;
    y = w->y[w->tail] ;
    if (x >= 0 && x < FIELD_RECT_WIDTH && y >= 0 && y < FIELD_RECT_HEIGHT) {
        rb->lcd_drawpixel(x + FIELD_RECT_X, y + FIELD_RECT_Y);
    }
    rb->lcd_set_drawmode(DRMODE_SOLID);
#ifdef HAVE_LCD_COLOR
    rb->lcd_set_foreground(COLOR_FG);
#endif
}

/**
 * Checks wether the coordinate is part of the worm. Returns
 * true if any part of the worm was hit - including the head.
 * @param x int The x coordinate
 * @param y int The y coordinate
 * @return int The index of the worm arrays that contain x, y.
 * Returns -1 if the coordinates are not part of the worm.
 */
static int specific_worm_collision(struct worm *w, int x, int y)
{
    int retVal = -1;

    /* get_worm_array_length is expensive -> buffer the value */
    int wormLength = get_worm_array_length(w);
    int i;

    /* test each entry that is part of the worm */
    for (i = 0; i < wormLength && retVal == -1; i++) {

        /* The iteration iterates the length of the worm.
           Here's the conversion to the true indices within the worm arrays. */
        int linestart = (w->tail + i  ) % MAX_WORM_SEGMENTS;
        int lineend   = (linestart + 1) % MAX_WORM_SEGMENTS;
        bool samex = (w->x[linestart] == x) && (w->x[lineend] == x);
        bool samey = (w->y[linestart] == y) && (w->y[lineend] == y);
        if (samex || samey){
            int test, min, max, tmp;

            if (samey) {
                min = w->x[linestart];
                max = w->x[lineend];
                test = x;
            } else {
                min = w->y[linestart];
                max = w->y[lineend];
                test = y;
            }

            tmp = min;
            min = MIN(min, max);
            max = MAX(tmp, max);

            if (min <= test && test <= max) {
                retVal = lineend;
            }
        }
    }
    return retVal;
}

/**
 * Increases the length of the specified worm by marking
 * that it may grow by len pixels. Note that the worm has
 * to move to make the growing happen.
 * @param worm *w The worm that is to be altered.
 * @param int len A positive value specifying the amount of
 * pixels the worm may grow.
 */
static void add_growing(struct worm *w, int len) {
    w->growing += len;
}

/**
 * Determins the worm that is at the coordinates x, y. The parameter
 * w is a switch parameter that changes the functionality of worm_collision.
 * If w is specified and x,y hits the head of w NULL is returned.
 * This is a useful way to determine wether the head of w hits
 * any worm but including itself but excluding its own head.
 * (It hits always its own head ;))
 * If w is set to NULL worm_collision returns any worm including all heads
 * that is at position of x,y.
 * @param struct worm *w The worm of which the head should be excluded in
 * the test. w may be set to NULL.
 * @param int x The x coordinate that is checked
 * @param int y The y coordinate that is checkec
 * @return struct worm*  The worm that has been hit by x,y. If no worm
 * was at the position NULL is returned.
 */
static struct worm* worm_collision(struct worm *w, int x, int y)
{
    struct worm *retVal = NULL;
    int i;
    for (i = 0; (i < worm_count) && (retVal == NULL); i++) {
        int collision_at = specific_worm_collision(&worms[i], x, y);
        if (collision_at != -1) {
            if (!(w == &worms[i] && collision_at == w->head)){
                retVal = &worms[i];
            }
        }
    }
    return retVal;
}

/**
 * Returns true if the head of the worm just has
 * crossed the field boundaries.
 * @return bool true if the worm just has wrapped.
 */
static bool field_collision(struct worm *w)
{
    bool retVal = false;
    if ((w->x[w->head] >= FIELD_RECT_WIDTH)  ||
        (w->y[w->head] >= FIELD_RECT_HEIGHT) ||
        (w->x[w->head] < 0)                  ||
        (w->y[w->head] < 0))
    {
        retVal = true;
    }
    return retVal;
}


/**
 * Returns true if the specified coordinates are within the
 * field specified by the FIELD_RECT_XXX constants.
 * @param int x The x coordinate of the point that is investigated
 * @param int y The y coordinate of the point that is investigated
 * @return bool Returns false if x,y specifies a point outside the
 * field of worms.
 */
static bool is_in_field_rect(int x, int y)
{
    bool retVal = false;
    retVal = (x >= 0 && x < FIELD_RECT_WIDTH &&
              y >= 0 && y < FIELD_RECT_HEIGHT);
    return retVal;
}

/**
 * Checks and returns wether the head of the w
 * is colliding with something currently.
 * @return int One of the values:
 *   COLLISION_NONE
 *   COLLISION_w
 *   COLLISION_FOOD
 *   COLLISION_ARGH
 *   COLLISION_FIELD
 */
static int check_collision(struct worm *w)
{
    int retVal = COLLISION_NONE;

    if (worm_collision(w, w->x[w->head], w->y[w->head]) != NULL)
        retVal = COLLISION_WORM;

    if (food_collision(w->x[w->head], w->y[w->head]) >= 0)
        retVal = COLLISION_FOOD;

    if (argh_collision(w->x[w->head], w->y[w->head]) >= 0)
        retVal = COLLISION_ARGH;

    if (field_collision(w))
        retVal = COLLISION_FIELD;

    return retVal;
}

/**
 * Returns the index of the food that is closest to the point
 * specified by x, y. This index may be used in the foodx and
 * foody arrays.
 * @param int x The x coordinate of the point
 * @param int y The y coordinate of the point
 * @return int A value usable as index in foodx and foody.
 */
static int get_nearest_food(int x, int y)
{
    int nearestfood = 0;
    int olddistance = FIELD_RECT_WIDTH + FIELD_RECT_HEIGHT;
    int deltax = 0;
    int deltay = 0;
    int foodindex;
    for (foodindex = 0; foodindex < MAX_FOOD; foodindex++) {
        int distance;
        deltax = foodx[foodindex] - x;
        deltay = foody[foodindex] - y;
        deltax = deltax > 0 ? deltax : deltax * (-1);
        deltay = deltay > 0 ? deltay : deltay * (-1);
        distance = deltax + deltay;

        if (distance < olddistance) {
            olddistance = distance;
            nearestfood = foodindex;
        }
    }
    return nearestfood;
}

/**
 * Returns wether the specified position is next to the worm
 * and in the direction the worm looks. Use this method to
 * test wether this position would be hit with the next move of
 * the worm unless the worm changes its direction.
 * @param struct worm *w - The worm to be investigated
 * @param int x - The x coordinate of the position to test.
 * @param int y - The y coordinate of the position to test.
 * @return Returns true if the worm will hit the position unless
 * it change its direction before the next move.
 */
static bool is_in_front_of_worm(struct worm *w, int x, int y)
{
    bool infront = false;
    int deltax = x - w->x[w->head];
    int deltay = y - w->y[w->head];

    if (w->dirx == 0) {
        infront = (w->diry * deltay) > 0;
    } else {
        infront = (w->dirx * deltax) > 0;
    }
    return infront;
}

/**
 * Returns true if the worm will collide with the next move unless
 * it changes its direction.
 * @param struct worm *w - The worm to be investigated.
 * @return Returns true if the worm will collide with the next move
 * unless it changes its direction.
 */
static bool will_worm_collide(struct worm *w)
{
    int x = w->x[w->head] + w->dirx;
    int y = w->y[w->head] + w->diry;
    bool retVal = !is_in_field_rect(x, y);
    if (!retVal) {
        retVal = (argh_collision(x, y) != -1);
    }

    if (!retVal) {
        retVal = (worm_collision(w, x, y) != NULL);
    }
    return retVal;
}

/**
 * This function
 * may be used to be stored in worm.fetch_worm_direction for
 * worms that are not controlled by humans but by artificial stupidity.
 * A direction is searched that doesn't lead to collision but to the nearest
 * food - but not very intelligent. The direction is written to the specified
 * worm.
 * @param struct worm *w - The worm of which the direction
 * is altered.
 */
static void virtual_player(struct worm *w)
{
    bool isright;
    int plana, planb, planc;
    /* find the next lunch */
    int nearestfood = get_nearest_food(w->x[w->head], w->y[w->head]);

    /* determine in which direction it is */

    /* in front of me? */
    bool infront = is_in_front_of_worm(w, foodx[nearestfood], foody[nearestfood]);

    /* left right of me? */
    int olddir = get_worm_dir(w);
    set_worm_dir(w, (olddir + 1) % 4);
    isright = is_in_front_of_worm(w, foodx[nearestfood], foody[nearestfood]);
    set_worm_dir(w, olddir);

    /* detect situation, set strategy */
    if (infront) {
        if (isright) {
            plana = olddir;
            planb = (olddir + 1) % 4;
            planc = (olddir + 3) % 4;
        } else {
            plana = olddir;
            planb = (olddir + 3) % 4;
            planc = (olddir + 1) % 4;
        }
    } else {
        if (isright) {
            plana = (olddir + 1) % 4;
            planb = olddir;
            planc = (olddir + 3) % 4;
        } else {
            plana = (olddir + 3) % 4;
            planb = olddir;
            planc = (olddir + 1) % 4;
        }
    }

    /* test for collision */
    set_worm_dir(w, plana);
    if (will_worm_collide(w)){

        /* plan b */
        set_worm_dir(w, planb);

        /* test for collision */
        if (will_worm_collide(w)) {

            /* plan c */
            set_worm_dir(w, planc);
        }
    }
}

/**
 * prints out the score board with all the status information
 * about the game.
 */
static void score_board(void)
{
    int i;
    int y = 0;
    rb->lcd_set_drawmode(DRMODE_SOLID|DRMODE_INVERSEVID);
    rb->lcd_fillrect(FIELD_RECT_WIDTH + 2, 0,
                     LCD_WIDTH - FIELD_RECT_WIDTH - 2, LCD_HEIGHT);
    rb->lcd_set_drawmode(DRMODE_SOLID);
    for (i = 0; i < worm_count; i++) {
        int score = get_score(&worms[i]);
        int collision = check_collision(&worms[i]);
        const char *state_str;

        /* high score */
        if (worms[i].fetch_worm_direction != virtual_player){
            if (highscore < score) {
                highscore = score;
            }
        }

        /* worm state */
        if (collision == COLLISION_NONE) {
            if (worms[i].growing > 0)
                state_str = "Growing";
            else {
                state_str = worms[i].alive ? "Hungry" : "Wormed";
            }
        } else {
            state_str = state_desc[collision];
        }

        /* length */
        rb->lcd_putsxyf(FIELD_RECT_WIDTH + 3, y  , "Len:%d", score);
        rb->lcd_putsxyf(FIELD_RECT_WIDTH + 3, y+8, state_str);

        if (!worms[i].alive){
            rb->lcd_set_drawmode(DRMODE_COMPLEMENT);
            rb->lcd_fillrect(FIELD_RECT_WIDTH + 2, y,
                             LCD_WIDTH - FIELD_RECT_WIDTH - 2, 17);
            rb->lcd_set_drawmode(DRMODE_SOLID);
        }
        y += 19;
    }
#ifdef DEBUG_WORMLET
    rb->lcd_putsxyf(FIELD_RECT_WIDTH + 3, LCD_HEIGHT - 8, "ticks %d", max_cycle);
#else
    rb->lcd_putsxyf(FIELD_RECT_WIDTH + 3, LCD_HEIGHT - 8, "Hs: %d", highscore);
#endif
}

/**
 * Checks for collisions of the worm and its environment and
 * takes appropriate actions like growing the worm or killing it.
 * @return bool Returns true if the worm is dead. Returns
 * false if the worm is healthy, up and creeping.
 */
static bool process_collisions(struct worm *w)
{
    int index = -1;

    w->alive &= !field_collision(w);

    if (w->alive) {

        /* check if food was eaten */
        index = food_collision(w->x[w->head], w->y[w->head]);
        if (index != -1){
            int i;

            clear_food(index);
            make_food(index);
            draw_food(index);

            for (i = 0; i < arghs_per_food; i++) {
                argh_count++;
                if (argh_count > MAX_ARGH)
                    argh_count = MAX_ARGH;
                make_argh(argh_count - 1);
                draw_argh(argh_count - 1);
            }

            add_growing(w, worm_food);

            draw_worm(w);
        }

        /* check if argh was eaten */
        else {
            index = argh_collision(w->x[w->head], w->y[w->head]);
            if (index != -1) {
                w->alive = false;
        }
            else {
                if (worm_collision(w, w->x[w->head], w->y[w->head]) != NULL) {
                    w->alive = false;
                }
            }
        }
    }
    return !w->alive;
}

/**
 * The main loop of the game.
 * @return bool Returns true if the game ended
 * with a dead worm. Returns false if the user
 * aborted the game manually.
 */
static int run(void)
{
    int button = 0;
    int wormDead = false;
    bool paused = false;

    /* ticks are counted to compensate speed variations */
    long cycle_start = 0, cycle_end = 0;
#ifdef DEBUG_WORMLET
    int ticks_to_max_cycle_reset = 20;
    max_cycle = 0;
#endif

    /* initialize the board and so on */
    init_wormlet();

    cycle_start = *rb->current_tick;
    /* change the direction of the worm */
    while (!wormDead)
    {
        int i;
        long cycle_duration=0;

#ifdef HAS_BUTTON_HOLD
        if (rb->button_hold())
        paused = true;
#endif

        switch (button) {
            case BTN_STARTPAUSE:
                paused = !paused;
                break;
            case BTN_STOPRESET:
                if (paused)
                    return 1; /* restart game */
                else
                    paused = true;
                break;
#ifdef BTN_RC_QUIT
            case BTN_RC_QUIT:
#endif
            case BTN_QUIT:
                return 2;  /* back to menu */
                break;
        }
        if (!paused)
        {
            switch (button) {
                case BTN_DIR_UP:
                    if (players == 1 && !use_remote) {
                        player1_dir = NORTH;
                    }
                    break;

                case BTN_DIR_DOWN:
                    if (players == 1 && !use_remote) {
                        player1_dir = SOUTH;
                    }
                    break;

                case BTN_DIR_LEFT:
                    if (players != 1 || use_remote) {
                        player1_dir = (player1_dir + 3) % 4;
                    } else {
                        player1_dir = WEST;
                    }
                    break;

                case BTN_DIR_RIGHT:
                    if (players != 1 || use_remote) {
                        player1_dir = (player1_dir + 1) % 4;
                    } else {
                        player1_dir = EAST;
                    }
                    break;

#ifdef MULTIPLAYER
                case BTN_PLAYER2_DIR1:
                    player2_dir = (player2_dir + 3) % 4;
                    break;

                case BTN_PLAYER2_DIR2:
                    player2_dir = (player2_dir + 1) % 4;
                    break;
#endif

#ifdef REMOTE
                case BTN_RC_UP:
                    player3_dir = (player3_dir + 1) % 4;
                    break;

                case BTN_RC_DOWN:
                    player3_dir = (player3_dir + 3) % 4;
                    break;
#endif
            }


            for (i = 0; i < worm_count; i++) {
                worms[i].fetch_worm_direction(&worms[i]);
            }

            wormDead = true;
            for (i = 0; i < worm_count; i++){
                struct worm *w = &worms[i];
                move_worm(w);
                wormDead &= process_collisions(w);
                draw_worm(w);
            }
            score_board();
            rb->lcd_update();
            if (button == BTN_STOPRESET) {
                wormDead = true;
            }

            /* here the wormlet game cycle ends
               thus the current tick is stored
               as end time */
            cycle_end = *rb->current_tick;

            /* The duration of the game cycle */
            cycle_duration = cycle_end - cycle_start;
            cycle_duration = MAX(0, cycle_duration);
            cycle_duration = MIN(speed -1, cycle_duration);


#ifdef DEBUG_WORMLET
            ticks_to_max_cycle_reset--;
            if (ticks_to_max_cycle_reset <= 0) {
                max_cycle = 0;
            }

            if (max_cycle < cycle_duration) {
                max_cycle = cycle_duration;
                ticks_to_max_cycle_reset = 20;
            }
#endif
        }
        /* adjust the number of ticks to wait for a button.
           This ensures that a complete game cycle including
           user input runs in constant time */
        button = rb->button_get_w_tmo(speed - cycle_duration);
        cycle_start = *rb->current_tick;
    }

    rb->splash(HZ*2, "Game Over!");

    return 2; /* back to menu */
}

#ifdef DEBUG_WORMLET

/**
 * Just a test routine that checks that worm_food_collision works
 * in some typical situations.
 */
static void test_worm_food_collision(void)
{
    int collision_count = 0;
    int i;
    rb->lcd_clear_display();
    init_worm(&worms[0], 10, 10);
    add_growing(&worms[0], 10);
    set_worm_dir(&worms[0], EAST);
    for (i = 0; i < 10; i++) {
        move_worm(&worms[0]);
        draw_worm(&worms[0]);
    }

    set_worm_dir(&worms[0], SOUTH);
    for (i = 0; i < 10; i++) {
        move_worm(&worms[0]);
        draw_worm(&worms[0]);
    }

    foodx[0] = 15;
    foody[0] = 12;
    for (foody[0] = 20; foody[0] > 0; foody[0] --) {
        bool collision;
        draw_worm(&worms[0]);
        draw_food(0);
        collision = worm_food_collision(&worms[0], 0);
        if (collision) {
            collision_count++;
        }
        rb->lcd_putsxyf(0, LCD_HEIGHT -8, "collisions: %d", collision_count);
        rb->lcd_update();
    }
    if (collision_count != food_size) {
        rb->button_get(true);
    }


    foody[0] = 15;
    for (foodx[0] = 30; foodx[0] > 0; foodx[0] --) {
        bool collision;
        draw_worm(&worms[0]);
        draw_food(0);
        collision = worm_food_collision(&worms[0], 0);
        if (collision) {
            collision_count ++;
        }
        rb->lcd_putsxyf(0, LCD_HEIGHT -8, "collisions: %d", collision_count);
        rb->lcd_update();
    }
    if (collision_count != food_size * 2) {
        rb->button_get(true);
    }

}

static bool expensive_worm_in_rect(struct worm *w, int rx, int ry, int rw, int rh)
{
    int x, y;
    bool retVal = false;
    for (x = rx; x < rx + rw; x++){
        for (y = ry; y < ry + rh; y++) {
            if (specific_worm_collision(w, x, y) != -1) {
                retVal = true;
            }
        }
    }
    return retVal;
}

static void test_worm_argh_collision(void)
{
    int i;
    int dir;
    int collision_count = 0;
    rb->lcd_clear_display();
    init_worm(&worms[0], 10, 10);
    add_growing(&worms[0], 40);
    for (dir = 0; dir < 4; dir++) {
        set_worm_dir(&worms[0], (EAST + dir) % 4);
        for (i = 0; i < 10; i++) {
            move_worm(&worms[0]);
            draw_worm(&worms[0]);
        }
    }

    arghx[0] = 12;
    for (arghy[0] = 0; arghy[0] < FIELD_RECT_HEIGHT - argh_size; arghy[0]++){
        bool collision;
        draw_argh(0);
        collision = worm_argh_collision(&worms[0], 0);
        if (collision) {
            collision_count ++;
        }
        rb->lcd_putsxyf(0, LCD_HEIGHT -8, "collisions: %d", collision_count);
        rb->lcd_update();
    }
    if (collision_count != argh_size * 2) {
        rb->button_get(true);
    }

    arghy[0] = 12;
    for (arghx[0] = 0; arghx[0] < FIELD_RECT_HEIGHT - argh_size; arghx[0]++){
        bool collision;
        draw_argh(0);
        collision = worm_argh_collision(&worms[0], 0);
        if (collision) {
            collision_count ++;
        }
        rb->lcd_putsxyf(0, LCD_HEIGHT -8, "collisions: %d", collision_count);
        rb->lcd_update();
    }
    if (collision_count != argh_size * 4) {
        rb->button_get(true);
    }
}

static int testline_in_rect(void)
{
    int testfailed = -1;

    int rx = 10;
    int ry = 15;
    int rw = 20;
    int rh = 25;

    /* Test 1 */
    int x1 = 12;
    int y1 = 8;
    int x2 = 12;
    int y2 = 42;

    if (!line_in_rect(x1, y1, x2, y2, rx, ry, rw, rh) &&
        !line_in_rect(x2, y2, x1, y1, rx, ry, rw, rh)) {
        rb->lcd_drawrect(rx, ry, rw, rh);
        rb->lcd_drawline(x1, y1, x2, y2);
        rb->lcd_update();
        rb->lcd_putsxy(0, 0, "failed 1");
        rb->button_get(true);
        testfailed = 1;
    }

    /* test 2 */
    y2 = 20;
    if (!line_in_rect(x1, y1, x2, y2, rx, ry, rw, rh) &&
        !line_in_rect(x2, y2, x1, y1, rx, ry, rw, rh)) {
        rb->lcd_drawrect(rx, ry, rw, rh);
        rb->lcd_drawline(x1, y1, x2, y2);
        rb->lcd_putsxy(0, 0, "failed 2");
        rb->lcd_update();
        rb->button_get(true);
        testfailed = 2;
    }

    /* test 3 */
    y1 = 30;
    if (!line_in_rect(x1, y1, x2, y2, rx, ry, rw, rh) &&
        !line_in_rect(x2, y2, x1, y1, rx, ry, rw, rh)) {
        rb->lcd_drawrect(rx, ry, rw, rh);
        rb->lcd_drawline(x1, y1, x2, y2);
        rb->lcd_putsxy(0, 0, "failed 3");
        rb->lcd_update();
        rb->button_get(true);
        testfailed = 3;
    }

    /* test 4 */
    y2 = 45;
    if (!line_in_rect(x1, y1, x2, y2, rx, ry, rw, rh) &&
        !line_in_rect(x2, y2, x1, y1, rx, ry, rw, rh)) {
        rb->lcd_drawrect(rx, ry, rw, rh);
        rb->lcd_drawline(x1, y1, x2, y2);
        rb->lcd_putsxy(0, 0, "failed 4");
        rb->lcd_update();
        rb->button_get(true);
        testfailed = 4;
    }

    /* test 5 */
    y1 = 50;
    if (line_in_rect(x1, y1, x2, y2, rx, ry, rw, rh) ||
        line_in_rect(x2, y2, x1, y1, rx, ry, rw, rh)) {
        rb->lcd_drawrect(rx, ry, rw, rh);
        rb->lcd_drawline(x1, y1, x2, y2);
        rb->lcd_putsxy(0, 0, "failed 5");
        rb->lcd_update();
        rb->button_get(true);
        testfailed = 5;
    }

    /* test 6 */
    y1 = 5;
    y2 = 7;
    if (line_in_rect(x1, y1, x2, y2, rx, ry, rw, rh) ||
        line_in_rect(x2, y2, x1, y1, rx, ry, rw, rh)) {
        rb->lcd_drawrect(rx, ry, rw, rh);
        rb->lcd_drawline(x1, y1, x2, y2);
        rb->lcd_putsxy(0, 0, "failed 6");
        rb->lcd_update();
        rb->button_get(true);
        testfailed = 6;
    }

    /* test 7 */
    x1 = 8;
    y1 = 20;
    x2 = 35;
    y2 = 20;
    if (!line_in_rect(x1, y1, x2, y2, rx, ry, rw, rh) &&
        !line_in_rect(x2, y2, x1, y1, rx, ry, rw, rh)) {
        rb->lcd_drawrect(rx, ry, rw, rh);
        rb->lcd_drawline(x1, y1, x2, y2);
        rb->lcd_putsxy(0, 0, "failed 7");
        rb->lcd_update();
        rb->button_get(true);
        testfailed = 7;
    }

    /* test 8 */
    x2 = 12;
    if (!line_in_rect(x1, y1, x2, y2, rx, ry, rw, rh) &&
        !line_in_rect(x2, y2, x1, y1, rx, ry, rw, rh)) {
        rb->lcd_drawrect(rx, ry, rw, rh);
        rb->lcd_drawline(x1, y1, x2, y2);
        rb->lcd_putsxy(0, 0, "failed 8");
        rb->lcd_update();
        rb->button_get(true);
        testfailed = 8;
    }

    /* test 9 */
    x1 = 25;
    if (!line_in_rect(x1, y1, x2, y2, rx, ry, rw, rh) &&
        !line_in_rect(x2, y2, x1, y1, rx, ry, rw, rh)) {
        rb->lcd_drawrect(rx, ry, rw, rh);
        rb->lcd_drawline(x1, y1, x2, y2);
        rb->lcd_putsxy(0, 0, "failed 9");
        rb->lcd_update();
        rb->button_get(true);
        testfailed = 9;
    }

    /* test 10 */
    x2 = 37;
    if (!line_in_rect(x1, y1, x2, y2, rx, ry, rw, rh) &&
        !line_in_rect(x2, y2, x1, y1, rx, ry, rw, rh)) {
        rb->lcd_drawrect(rx, ry, rw, rh);
        rb->lcd_drawline(x1, y1, x2, y2);
        rb->lcd_putsxy(0, 0, "failed 10");
        rb->lcd_update();
        rb->button_get(true);
        testfailed = 10;
    }

    /* test 11 */
    x1 = 42;
    if (line_in_rect(x1, y1, x2, y2, rx, ry, rw, rh) ||
        line_in_rect(x2, y2, x1, y1, rx, ry, rw, rh)) {
        rb->lcd_drawrect(rx, ry, rw, rh);
        rb->lcd_drawline(x1, y1, x2, y2);
        rb->lcd_putsxy(0, 0, "failed 11");
        rb->lcd_update();
        rb->button_get(true);
        testfailed = 11;
    }

    /* test 12 */
    x1 = 5;
    x2 = 7;
    if (line_in_rect(x1, y1, x2, y2, rx, ry, rw, rh) ||
        line_in_rect(x2, y2, x1, y1, rx, ry, rw, rh)) {
        rb->lcd_drawrect(rx, ry, rw, rh);
        rb->lcd_drawline(x1, y1, x2, y2);
        rb->lcd_putsxy(0, 0, "failed 12");
        rb->lcd_update();
        rb->button_get(true);
        testfailed = 12;
    }

    /* test 13 */
    rx = 9;
    ry = 15;
    rw = food_size;
    rh = food_size;

    x1 = 10;
    y1 = 10;
    x2 = 10;
    y2 = 20;
    if (!(line_in_rect(x1, y1, x2, y2, rx, ry, rw, rh) &&
          line_in_rect(x2, y2, x1, y1, rx, ry, rw, rh))) {
        rb->lcd_drawrect(rx, ry, rw, rh);
        rb->lcd_drawline(x1, y1, x2, y2);
        rb->lcd_putsxy(0, 0, "failed 13");
        rb->lcd_update();
        rb->button_get(true);
        testfailed = 13;
    }

    /* test 14 */
    rx = 9;
    ry = 15;
    rw = 4;
    rh = 4;

    x1 = 10;
    y1 = 10;
    x2 = 10;
    y2 = 19;
    if (!(line_in_rect(x1, y1, x2, y2, rx, ry, rw, rh) &&
          line_in_rect(x2, y2, x1, y1, rx, ry, rw, rh))) {
        rb->lcd_drawline(x1, y1, x2, y2);
        rb->lcd_invertrect(rx, ry, rw, rh);
        rb->lcd_putsxy(0, 0, "failed 14");
        rb->lcd_update();
        rb->button_get(true);
        testfailed = 14;
    }

    rb->lcd_clear_display();

    return testfailed;
}

/**
 * Just a test routine to test wether specific_worm_collision might work properly
 */
static int test_specific_worm_collision(void)
{
    int collisions = 0;
    int dir;
    int x = 0;
    int y = 0;
    rb->lcd_clear_display();
    init_worm(&worms[0], 10, 20);
    add_growing(&worms[0], 20 - INITIAL_WORM_LENGTH);

    for (dir = EAST; dir < EAST + 4; dir++) {
        int i;
        set_worm_dir(&worms[0], dir % 4);
        for (i = 0; i < 5; i++) {
            if (!(dir % 4 == NORTH && i == 9)) {
                move_worm(&worms[0]);
                draw_worm(&worms[0]);
            }
        }
    }

    for (y = 15; y < 30; y ++){
        for (x = 5; x < 20; x++) {
            if (specific_worm_collision(&worms[0], x, y) != -1) {
                collisions ++;
            }
            rb->lcd_invertpixel(x + FIELD_RECT_X, y + FIELD_RECT_Y);
            rb->lcd_putsxyf(0, LCD_HEIGHT - 8, "collisions %d", collisions);
            rb->lcd_update();
        }
    }
    if (collisions != 21) {
        rb->button_get(true);
    }
    return collisions;
}

static void test_make_argh(void)
{
    int dir;
    int seed = 0;
    int hit = 0;
    int failures = 0;
    int last_failures = 0;
    int i, worm_idx;
    rb->lcd_clear_display();
    worm_count = 3;

    for (worm_idx = 0; worm_idx < worm_count; worm_idx++) {
        init_worm(&worms[worm_idx], 10 + worm_idx * 20, 20);
        add_growing(&worms[worm_idx], 40 - INITIAL_WORM_LENGTH);
    }

    for (dir = EAST; dir < EAST + 4; dir++) {
        for (worm_idx = 0; worm_idx < worm_count; worm_idx++) {
            set_worm_dir(&worms[worm_idx], dir % 4);
        for (i = 0; i < 10; i++) {
            if (!(dir % 4 == NORTH && i == 9)) {
                    move_worm(&worms[worm_idx]);
                    draw_worm(&worms[worm_idx]);
                }
            }
        }
    }

    rb->lcd_update();

    for (seed = 0; hit < 20; seed += 2) {
        int x, y;
        rb->srand(seed);
        x = rb->rand() % (FIELD_RECT_WIDTH  - argh_size);
        y = rb->rand() % (FIELD_RECT_HEIGHT - argh_size);

        for (worm_idx = 0; worm_idx < worm_count; worm_idx++){
            if (expensive_worm_in_rect(&worms[worm_idx], x, y, argh_size, argh_size)) {
                int tries = 0;
                rb->srand(seed);

                tries = make_argh(0);
                if ((x == arghx[0] && y == arghy[0]) || tries < 2) {
                    failures ++;
                }

                rb->lcd_putsxyf(0, LCD_HEIGHT - 8, "(%d;%d) fail%d try%d",
                             x, y, failures, tries);
                rb->lcd_update();
                rb->lcd_invertrect(x + FIELD_RECT_X, y+ FIELD_RECT_Y,
                                   argh_size, argh_size);
                rb->lcd_update();
                draw_argh(0);
                rb->lcd_update();
                rb->lcd_invertrect(x + FIELD_RECT_X, y + FIELD_RECT_Y,
                                   argh_size, argh_size);
                rb->lcd_clearrect(arghx[0] + FIELD_RECT_X, arghy[0] + FIELD_RECT_Y,
                                  argh_size, argh_size);

                if (failures > last_failures) {
                    rb->button_get(true);
                }
                last_failures = failures;
                hit ++;
            }
        }
    }
}

static void test_worm_argh_collision_in_moves(void) {
    int hit_count = 0;
    int i;
    rb->lcd_clear_display();
    init_worm(&worms[0], 10, 20);

    arghx[0] = 20;
    arghy[0] = 18;
    draw_argh(0);

    set_worm_dir(&worms[0], EAST);
    for (i = 0; i < 20; i++) {
        move_worm(&worms[0]);
        draw_worm(&worms[0]);
        if (worm_argh_collision_in_moves(&worms[0], 0, 5)){
            hit_count ++;
        }
        rb->lcd_putsxyf(0, LCD_HEIGHT - 8, "in 5 moves hits: %d", hit_count);
        rb->lcd_update();
    }
    if (hit_count != argh_size + 5) {
        rb->button_get(true);
    }
}
#endif /* DEBUG_WORMLET */

/*
 * Reverts default settings
 */
static void default_settings(void)
{
    arghs_per_food = ARGHS_PER_FOOD;
    argh_size = ARGH_SIZE;
    food_size = FOOD_SIZE;
    speed = SPEED;
    worm_food = WORM_PER_FOOD;
    players = 1;
    worm_count = MAX_WORMS;
    use_remote = false;
    return;
}

/*
 * Launches the wormlet game
 */
static bool launch_wormlet(void)
{
    int game_result = 1;

    rb->lcd_clear_display();

    /* Turn off backlight timeout */
    backlight_ignore_timeout();

    /* start the game */
    while (game_result == 1)
        game_result = run();

    switch (game_result)
    {
        case 2:
            /* Turn on backlight timeout (revert to settings) */
            backlight_use_settings();
            return false;
            break;
    }
    return false;
}

/**
 * Main entry point
 */
enum plugin_status plugin_start(const void* parameter)
{
    int result;
    int menu_quit = 0;
    int new_setting;

    (void)(parameter);

    default_settings();
    if (configfile_load(SETTINGS_FILENAME, config,
                        sizeof(config)/sizeof(*config),
                        SETTINGS_MIN_VERSION ) < 0)
    {
        /* If the loading failed, save a new config file (as the disk is
           already spinning) */
        configfile_save(SETTINGS_FILENAME, config,
                        sizeof(config)/sizeof(*config),
                        SETTINGS_VERSION);
    }

#ifdef HAVE_LCD_COLOR
    rb->lcd_set_foreground(COLOR_FG);
    rb->lcd_set_background(COLOR_BG);
#endif

#if LCD_DEPTH > 1
    rb->lcd_set_backdrop(NULL);
#endif

#ifdef DEBUG_WORMLET
    testline_in_rect();
    test_worm_argh_collision_in_moves();
    test_make_argh();
    test_worm_food_collision();
    test_worm_argh_collision();
    test_specific_worm_collision();
#endif

    /* Setup screen */

    static const struct opt_items noyes[2] = {
        { "No", -1 },
        { "Yes", -1 },
    };

    static const struct opt_items remoteonly_option[1] = {
        { "Remote Control", -1 }
    };

    static const struct opt_items key24_option[2] = {
        { "4 Key Control", -1 },
        { "2 Key Control", -1 }
    };

#ifdef REMOTE
    static const struct opt_items remote_option[2] = {
        { "Remote Control", -1 },
        { "No Rem. Control", -1 }
    };
#else
    static const struct opt_items key2_option[1] = {
        { "2 Key Control", -1 }
    };
#endif

    static const struct opt_items nokey_option[1] = {
        { "Out of Control", -1 }
    };

    MENUITEM_STRINGLIST(menu, "Wormlet Menu", NULL, "Play Wormlet!",
                        "Number of Worms", "Number of Players", "Control Style",
                        "Worm Growth Per Food","Worm Speed","Arghs Per Food",
                        "Argh Size","Food Size","Revert to Default Settings",
                        "Playback Control", "Quit");

    rb->button_clear_queue();

    while (!menu_quit) {
        switch(rb->do_menu(&menu, &result, NULL, false))
        {
            case 0:
                rb->lcd_setfont(FONT_SYSFIXED);
                launch_wormlet();
                break;
            case 1:
                rb->set_int("Number of Worms", "", UNIT_INT, &worm_count, NULL,
                            1, 1, 3, NULL);
                if (worm_count < players) {
                    worm_count = players;
                }
                break;
            case 2:
#ifdef MULTIPLAYER
                rb->set_int("Number of Players", "", UNIT_INT, &players, NULL,
                            1, 0, 4, NULL);
#else
                rb->set_int("Number of Players", "", UNIT_INT, &players, NULL,
                            1, 0, 2, NULL);
#endif
                if (players > worm_count) {
                    worm_count = players;
                }
                if (players > 2) {
                    use_remote = true;
                }
                break;
            case 3:
                switch(players) {
                    case 0:
                        rb->set_option("Control Style",&use_remote,INT,
                                       nokey_option, 1, NULL);
                        break;
                    case 1:
                        rb->set_option("Control Style",&use_remote,INT,
                                       key24_option, 2, NULL);
                        break;
                    case 2:
#ifdef REMOTE
                        rb->set_option("Control Style",&use_remote,INT,
                                       remote_option, 2, NULL);
#else
                        rb->set_option("Control Style",&use_remote,INT,
                                       key2_option, 1, NULL);
#endif
                        break;
                    case 3:
                        rb->set_option("Control Style",&use_remote,INT,
                                       remoteonly_option, 1, NULL);
                        break;
                }
                break;
            case 4:
                rb->set_int("Worm Growth Per Food", "", UNIT_INT, &worm_food,
                            NULL, 1, 0, 15, NULL);
                break;
            case 5:
                new_setting = 20 - speed;
                rb->set_int("Worm Speed", "", UNIT_INT, &new_setting,
                            NULL, 1, 0, 20, NULL);
                speed = 20 - new_setting;
                break;
            case 6:
                rb->set_int("Arghs Per Food", "", UNIT_INT, &arghs_per_food,
                            NULL, 1, 0, 8, NULL);
                break;
            case 7:
                rb->set_int("Argh Size", "", UNIT_INT, &argh_size,
                            NULL, 1, 2, 10, NULL);
                break;
            case 8:
                rb->set_int("Food Size", "", UNIT_INT, &food_size,
                            NULL, 1, 2, 10, NULL);
                break;
            case 9:
                new_setting = 0;
                rb->set_option("Reset Settings?", &new_setting, INT, noyes , 2, NULL);
                if (new_setting == 1)
                    default_settings();
                break;
            case 10:
                playback_control(NULL);
                break;
            default:
                menu_quit=1;
                break;
        }
    }

    configfile_save(SETTINGS_FILENAME, config,
                        sizeof(config)/sizeof(*config),
                        SETTINGS_VERSION);

    return PLUGIN_OK;
}
