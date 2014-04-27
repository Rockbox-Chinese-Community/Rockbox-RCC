/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2004-2006 Antoine Cellerier <dionoea -at- videolan -dot- org>
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



/* what the minesweeper() function can return */
enum minesweeper_status {
    MINESWEEPER_WIN,
    MINESWEEPER_LOSE,
    MINESWEEPER_QUIT,
    MINESWEEPER_USB
};

/* variable button definitions */
#if CONFIG_KEYPAD == RECORDER_PAD
#   define MINESWP_LEFT     BUTTON_LEFT
#   define MINESWP_RIGHT    BUTTON_RIGHT
#   define MINESWP_UP       BUTTON_UP
#   define MINESWP_DOWN     BUTTON_DOWN
#   define MINESWP_QUIT     BUTTON_OFF
#   define MINESWP_TOGGLE   BUTTON_ON
#   define MINESWP_TOGGLE2  BUTTON_F1
#   define MINESWP_DISCOVER BUTTON_PLAY
#   define MINESWP_DISCOVER2 BUTTON_F2
#   define MINESWP_INFO     BUTTON_F3

#elif CONFIG_KEYPAD == ARCHOS_AV300_PAD
#   define MINESWP_LEFT     BUTTON_LEFT
#   define MINESWP_RIGHT    BUTTON_RIGHT
#   define MINESWP_UP       BUTTON_UP
#   define MINESWP_DOWN     BUTTON_DOWN
#   define MINESWP_QUIT     BUTTON_OFF
#   define MINESWP_TOGGLE   BUTTON_ON
#   define MINESWP_TOGGLE2  BUTTON_F1
#   define MINESWP_DISCOVER BUTTON_SELECT
#   define MINESWP_DISCOVER2 BUTTON_F2
#   define MINESWP_INFO     BUTTON_F3

#elif CONFIG_KEYPAD == ONDIO_PAD
#   define MINESWP_LEFT     BUTTON_LEFT
#   define MINESWP_RIGHT    BUTTON_RIGHT
#   define MINESWP_UP       BUTTON_UP
#   define MINESWP_DOWN     BUTTON_DOWN
#   define MINESWP_QUIT     BUTTON_OFF
#   define MINESWP_TOGGLE_PRE BUTTON_MENU
#   define MINESWP_TOGGLE   (BUTTON_MENU | BUTTON_REL)
#   define MINESWP_DISCOVER (BUTTON_MENU | BUTTON_REPEAT)
#   define MINESWP_INFO     (BUTTON_MENU | BUTTON_OFF)

#elif (CONFIG_KEYPAD == IRIVER_H100_PAD) || \
      (CONFIG_KEYPAD == IRIVER_H300_PAD)
#   define MINESWP_LEFT     BUTTON_LEFT
#   define MINESWP_RIGHT    BUTTON_RIGHT
#   define MINESWP_UP       BUTTON_UP
#   define MINESWP_DOWN     BUTTON_DOWN
#   define MINESWP_QUIT     BUTTON_OFF
#   define MINESWP_TOGGLE   BUTTON_ON
#   define MINESWP_TOGGLE2  BUTTON_REC
#   define MINESWP_DISCOVER BUTTON_SELECT
#   define MINESWP_INFO     BUTTON_MODE

#   define MINESWP_RC_QUIT  BUTTON_RC_STOP

#elif (CONFIG_KEYPAD == IPOD_4G_PAD) || \
      (CONFIG_KEYPAD == IPOD_3G_PAD) || \
      (CONFIG_KEYPAD == IPOD_1G2G_PAD)
#   define MINESWP_SCROLLWHEEL
#   define MINESWP_LEFT       BUTTON_LEFT
#   define MINESWP_RIGHT      BUTTON_RIGHT
#   define MINESWP_UP         BUTTON_MENU
#   define MINESWP_DOWN       BUTTON_PLAY
#   define MINESWP_NEXT       BUTTON_SCROLL_FWD
#   define MINESWP_PREV       BUTTON_SCROLL_BACK
#   define MINESWP_QUIT       (BUTTON_SELECT | BUTTON_MENU)
#   define MINESWP_TOGGLE_PRE BUTTON_SELECT
#   define MINESWP_TOGGLE     (BUTTON_SELECT | BUTTON_REL)
#   define MINESWP_DISCOVER   (BUTTON_SELECT | BUTTON_REPEAT)
#   define MINESWP_INFO       (BUTTON_SELECT | BUTTON_PLAY)

#elif (CONFIG_KEYPAD == IAUDIO_X5M5_PAD)
#   define MINESWP_LEFT     BUTTON_LEFT
#   define MINESWP_RIGHT    BUTTON_RIGHT
#   define MINESWP_UP       BUTTON_UP
#   define MINESWP_DOWN     BUTTON_DOWN
#   define MINESWP_QUIT     BUTTON_POWER
#   define MINESWP_TOGGLE   BUTTON_PLAY
#   define MINESWP_DISCOVER BUTTON_SELECT
#   define MINESWP_INFO     BUTTON_REC

#elif (CONFIG_KEYPAD == GIGABEAT_PAD)
#   define MINESWP_LEFT     BUTTON_LEFT
#   define MINESWP_RIGHT    BUTTON_RIGHT
#   define MINESWP_UP       BUTTON_UP
#   define MINESWP_DOWN     BUTTON_DOWN
#   define MINESWP_QUIT     BUTTON_POWER
#   define MINESWP_TOGGLE   BUTTON_A
#   define MINESWP_DISCOVER BUTTON_SELECT
#   define MINESWP_INFO     BUTTON_MENU

#elif (CONFIG_KEYPAD == SANSA_E200_PAD)

#   define MINESWP_SCROLLWHEEL
#   define MINESWP_LEFT     BUTTON_LEFT
#   define MINESWP_RIGHT    BUTTON_RIGHT
#   define MINESWP_UP       BUTTON_UP
#   define MINESWP_DOWN     BUTTON_DOWN
#   define MINESWP_QUIT     BUTTON_POWER
#   define MINESWP_NEXT     BUTTON_SCROLL_FWD
#   define MINESWP_PREV     BUTTON_SCROLL_BACK
#   define MINESWP_TOGGLE   BUTTON_REC
#   define MINESWP_DISCOVER BUTTON_SELECT
#   define MINESWP_INFO     (BUTTON_REC|BUTTON_REPEAT)

#elif (CONFIG_KEYPAD == SANSA_FUZE_PAD)

#   define MINESWP_LEFT     BUTTON_LEFT
#   define MINESWP_RIGHT    BUTTON_RIGHT
#   define MINESWP_UP       BUTTON_UP
#   define MINESWP_DOWN     BUTTON_DOWN
#   define MINESWP_QUIT     (BUTTON_HOME|BUTTON_REPEAT)
#   define MINESWP_TOGGLE   BUTTON_SCROLL_FWD
#   define MINESWP_DISCOVER BUTTON_SELECT
#   define MINESWP_INFO     BUTTON_SCROLL_BACK

#elif (CONFIG_KEYPAD == SANSA_C200_PAD) || \
(CONFIG_KEYPAD == SANSA_CLIP_PAD) || \
(CONFIG_KEYPAD == SANSA_M200_PAD)
#   define MINESWP_LEFT       BUTTON_LEFT
#   define MINESWP_RIGHT      BUTTON_RIGHT
#   define MINESWP_UP         BUTTON_UP
#   define MINESWP_DOWN       BUTTON_DOWN
#   define MINESWP_QUIT       BUTTON_POWER
#   define MINESWP_TOGGLE_PRE BUTTON_SELECT
#   define MINESWP_TOGGLE     (BUTTON_SELECT | BUTTON_REL)
#   define MINESWP_TOGGLE2    BUTTON_VOL_DOWN
#   define MINESWP_DISCOVER   (BUTTON_SELECT | BUTTON_REPEAT)
#   define MINESWP_DISCOVER2  BUTTON_VOL_UP
#   define MINESWP_INFO       (BUTTON_SELECT | BUTTON_UP)

#elif (CONFIG_KEYPAD == IRIVER_H10_PAD)
#   define MINESWP_LEFT     BUTTON_LEFT
#   define MINESWP_RIGHT    BUTTON_RIGHT
#   define MINESWP_UP       BUTTON_SCROLL_UP
#   define MINESWP_DOWN     BUTTON_SCROLL_DOWN
#   define MINESWP_QUIT     BUTTON_POWER
#   define MINESWP_TOGGLE   BUTTON_PLAY
#   define MINESWP_DISCOVER BUTTON_REW
#   define MINESWP_INFO     (BUTTON_REW | BUTTON_PLAY)

#elif (CONFIG_KEYPAD == GIGABEAT_S_PAD)
#   define MINESWP_LEFT     BUTTON_LEFT
#   define MINESWP_RIGHT    BUTTON_RIGHT
#   define MINESWP_UP       BUTTON_UP
#   define MINESWP_DOWN     BUTTON_DOWN
#   define MINESWP_QUIT     BUTTON_BACK
#   define MINESWP_TOGGLE   BUTTON_PLAY
#   define MINESWP_DISCOVER BUTTON_SELECT
#   define MINESWP_INFO     BUTTON_MENU

#elif (CONFIG_KEYPAD == MROBE100_PAD)
#   define MINESWP_LEFT     BUTTON_LEFT
#   define MINESWP_RIGHT    BUTTON_RIGHT
#   define MINESWP_UP       BUTTON_UP
#   define MINESWP_DOWN     BUTTON_DOWN
#   define MINESWP_QUIT     BUTTON_POWER
#   define MINESWP_TOGGLE   BUTTON_DISPLAY
#   define MINESWP_DISCOVER BUTTON_SELECT
#   define MINESWP_INFO     BUTTON_MENU

#elif CONFIG_KEYPAD == IAUDIO_M3_PAD
#   define MINESWP_LEFT     BUTTON_RC_REW
#   define MINESWP_RIGHT    BUTTON_RC_FF
#   define MINESWP_UP       BUTTON_RC_VOL_UP
#   define MINESWP_DOWN     BUTTON_RC_VOL_DOWN
#   define MINESWP_QUIT     BUTTON_RC_REC
#   define MINESWP_TOGGLE   BUTTON_RC_MODE
#   define MINESWP_DISCOVER BUTTON_RC_PLAY
#   define MINESWP_INFO     BUTTON_RC_MENU

#elif (CONFIG_KEYPAD == COWON_D2_PAD)
#   define MINESWP_QUIT     BUTTON_POWER

#elif CONFIG_KEYPAD == CREATIVEZVM_PAD
#   define MINESWP_LEFT     BUTTON_LEFT
#   define MINESWP_RIGHT    BUTTON_RIGHT
#   define MINESWP_UP       BUTTON_UP
#   define MINESWP_DOWN     BUTTON_DOWN
#   define MINESWP_QUIT     BUTTON_BACK
#   define MINESWP_TOGGLE   BUTTON_SELECT
#   define MINESWP_DISCOVER BUTTON_PLAY
#   define MINESWP_INFO     BUTTON_MENU

#elif CONFIG_KEYPAD == PHILIPS_HDD1630_PAD
#   define MINESWP_LEFT     BUTTON_LEFT
#   define MINESWP_RIGHT    BUTTON_RIGHT
#   define MINESWP_UP       BUTTON_UP
#   define MINESWP_DOWN     BUTTON_DOWN
#   define MINESWP_QUIT     BUTTON_POWER
#   define MINESWP_TOGGLE   BUTTON_VIEW
#   define MINESWP_DISCOVER BUTTON_SELECT
#   define MINESWP_INFO     BUTTON_MENU

#elif CONFIG_KEYPAD == PHILIPS_HDD6330_PAD
#   define MINESWP_LEFT     BUTTON_LEFT
#   define MINESWP_RIGHT    BUTTON_RIGHT
#   define MINESWP_UP       BUTTON_UP
#   define MINESWP_DOWN     BUTTON_DOWN
#   define MINESWP_QUIT     BUTTON_POWER
#   define MINESWP_TOGGLE   BUTTON_NEXT
#   define MINESWP_DISCOVER BUTTON_PLAY
#   define MINESWP_INFO     BUTTON_MENU

#elif CONFIG_KEYPAD == PHILIPS_SA9200_PAD
#   define MINESWP_LEFT     BUTTON_PREV
#   define MINESWP_RIGHT    BUTTON_NEXT
#   define MINESWP_UP       BUTTON_UP
#   define MINESWP_DOWN     BUTTON_DOWN
#   define MINESWP_QUIT     BUTTON_POWER
#   define MINESWP_TOGGLE   BUTTON_RIGHT
#   define MINESWP_DISCOVER BUTTON_PLAY
#   define MINESWP_INFO     BUTTON_MENU

#elif (CONFIG_KEYPAD == ONDAVX747_PAD) || \
CONFIG_KEYPAD == ONDAVX777_PAD || \
CONFIG_KEYPAD == MROBE500_PAD
#   define MINESWP_QUIT     BUTTON_POWER

#elif (CONFIG_KEYPAD == SAMSUNG_YH_PAD)
#   define MINESWP_LEFT     BUTTON_LEFT
#   define MINESWP_RIGHT    BUTTON_RIGHT
#   define MINESWP_UP       BUTTON_UP
#   define MINESWP_DOWN     BUTTON_DOWN
#   define MINESWP_QUIT     BUTTON_REC
#   define MINESWP_TOGGLE   BUTTON_PLAY
#   define MINESWP_DISCOVER BUTTON_REW
#   define MINESWP_INFO     BUTTON_FFWD

#elif (CONFIG_KEYPAD == PBELL_VIBE500_PAD)
#   define MINESWP_LEFT     BUTTON_PREV
#   define MINESWP_RIGHT    BUTTON_NEXT
#   define MINESWP_UP       BUTTON_UP
#   define MINESWP_DOWN     BUTTON_DOWN
#   define MINESWP_QUIT     BUTTON_REC
#   define MINESWP_TOGGLE   BUTTON_PLAY
#   define MINESWP_DISCOVER BUTTON_OK
#   define MINESWP_INFO     BUTTON_MENU

#elif CONFIG_KEYPAD == MPIO_HD200_PAD
#   define MINESWP_LEFT     BUTTON_VOL_DOWN
#   define MINESWP_RIGHT    BUTTON_VOL_UP
#   define MINESWP_UP       BUTTON_REW
#   define MINESWP_DOWN     BUTTON_FF
#   define MINESWP_QUIT     (BUTTON_REC|BUTTON_PLAY)
#   define MINESWP_TOGGLE   BUTTON_PLAY
#   define MINESWP_DISCOVER BUTTON_FUNC
#   define MINESWP_INFO     BUTTON_REC

#elif CONFIG_KEYPAD == MPIO_HD300_PAD
#   define MINESWP_LEFT     BUTTON_REW
#   define MINESWP_RIGHT    BUTTON_FF
#   define MINESWP_UP       BUTTON_UP
#   define MINESWP_DOWN     BUTTON_DOWN
#   define MINESWP_QUIT     (BUTTON_MENU|BUTTON_REPEAT)
#   define MINESWP_TOGGLE   BUTTON_ENTER
#   define MINESWP_DISCOVER BUTTON_PLAY
#   define MINESWP_INFO     (BUTTON_MENU|BUTTON_REL)

#elif CONFIG_KEYPAD == SANSA_FUZEPLUS_PAD
#   define MINESWP_LEFT     BUTTON_LEFT
#   define MINESWP_RIGHT    BUTTON_RIGHT
#   define MINESWP_UP       BUTTON_UP
#   define MINESWP_DOWN     BUTTON_DOWN
#   define MINESWP_QUIT     BUTTON_POWER
#   define MINESWP_TOGGLE   BUTTON_SELECT
#   define MINESWP_DISCOVER BUTTON_PLAYPAUSE
#   define MINESWP_NEXT     BUTTON_BOTTOMRIGHT
#   define MINESWP_PREV     BUTTON_BOTTOMLEFT
#   define MINESWP_INFO     BUTTON_BACK


#elif (CONFIG_KEYPAD == SANSA_CONNECT_PAD)
#   define MINESWP_SCROLLWHEEL
#   define MINESWP_LEFT     BUTTON_LEFT
#   define MINESWP_RIGHT    BUTTON_RIGHT
#   define MINESWP_UP       BUTTON_UP
#   define MINESWP_DOWN     BUTTON_DOWN
#   define MINESWP_QUIT     BUTTON_POWER
#   define MINESWP_NEXT     BUTTON_SCROLL_FWD
#   define MINESWP_PREV     BUTTON_SCROLL_BACK
#   define MINESWP_TOGGLE   BUTTON_NEXT
#   define MINESWP_DISCOVER BUTTON_SELECT
#   define MINESWP_INFO     BUTTON_PREV

#elif (CONFIG_KEYPAD == SAMSUNG_YPR0_PAD)
#   define MINESWP_LEFT     BUTTON_LEFT
#   define MINESWP_RIGHT    BUTTON_RIGHT
#   define MINESWP_UP       BUTTON_UP
#   define MINESWP_DOWN     BUTTON_DOWN
#   define MINESWP_QUIT     BUTTON_BACK
#   define MINESWP_TOGGLE   BUTTON_USER
#   define MINESWP_DISCOVER BUTTON_SELECT
#   define MINESWP_INFO     BUTTON_MENU

#elif (CONFIG_KEYPAD == HM60X_PAD)

#   define MINESWP_LEFT     BUTTON_LEFT
#   define MINESWP_RIGHT    BUTTON_RIGHT
#   define MINESWP_UP       BUTTON_UP
#   define MINESWP_DOWN     BUTTON_DOWN
#   define MINESWP_QUIT     BUTTON_POWER
#   define MINESWP_TOGGLE   (BUTTON_LEFT|BUTTON_POWER)
#   define MINESWP_DISCOVER BUTTON_SELECT
#   define MINESWP_INFO     (BUTTON_UP|BUTTON_POWER)

#elif (CONFIG_KEYPAD == HM801_PAD)

#   define MINESWP_LEFT     BUTTON_LEFT
#   define MINESWP_RIGHT    BUTTON_RIGHT
#   define MINESWP_UP       BUTTON_UP
#   define MINESWP_DOWN     BUTTON_DOWN
#   define MINESWP_QUIT     BUTTON_POWER
#   define MINESWP_TOGGLE   BUTTON_PLAY
#   define MINESWP_DISCOVER BUTTON_SELECT
#   define MINESWP_INFO     BUTTON_PREV

#elif (CONFIG_KEYPAD == CREATIVE_ZENXFI3_PAD)

#   define MINESWP_LEFT     BUTTON_BACK
#   define MINESWP_RIGHT    BUTTON_MENU
#   define MINESWP_UP       BUTTON_UP
#   define MINESWP_DOWN     BUTTON_DOWN
#   define MINESWP_QUIT     BUTTON_POWER
#   define MINESWP_TOGGLE   BUTTON_VOL_UP
#   define MINESWP_DISCOVER BUTTON_PLAY
#   define MINESWP_INFO     BUTTON_VOL_DOWN

#else
#error No keymap defined!
#endif

#ifdef HAVE_TOUCHSCREEN
#ifndef MINESWP_QUIT
#   define MINESWP_QUIT     BUTTON_TOPLEFT
#endif
#ifndef MINESWP_LEFT
#   define MINESWP_LEFT     BUTTON_MIDLEFT
#endif
#ifndef MINESWP_RIGHT
#   define MINESWP_RIGHT    BUTTON_MIDRIGHT
#endif
#ifndef MINESWP_UP
#   define MINESWP_UP       BUTTON_TOPMIDDLE
#endif
#ifndef MINESWP_DOWN
#   define MINESWP_DOWN     BUTTON_BOTTOMMIDDLE
#endif
#ifndef MINESWP_TOGGLE
#   define MINESWP_TOGGLE   BUTTON_CENTER
#endif
#ifndef MINESWP_DISCOVER
#   define MINESWP_DISCOVER BUTTON_BOTTOMLEFT
#endif
#ifndef MINESWP_INFO
#   define MINESWP_INFO     BUTTON_BOTTOMRIGHT
#endif
#endif

extern const fb_data minesweeper_tiles[];

#ifdef HAVE_LCD_COLOR
#   if ( LCD_HEIGHT * LCD_WIDTH ) / ( 16 * 16 ) >= 130
        /* We want to have at least 130 tiles on the screen */
#       define TileSize 16
#   elif ( LCD_HEIGHT * LCD_WIDTH ) / ( 12 * 12 ) >= 130
#       define TileSize 12
#   else
#       define TileSize 10
#   endif
#   define BackgroundColor LCD_RGBPACK( 128, 128, 128 )
#elif LCD_DEPTH > 1
#   define TileSize 12
#else
#   define TileSize 8
#endif

#define Mine         9
#define Flag         10
#define Unknown      11
#define ExplodedMine 12
#define WrongFlag    13
#define CorrectFlag  14

#define draw_tile( num, x, y ) \
    rb->lcd_bitmap_part( minesweeper_tiles, 0, num * TileSize, \
                         TileSize, left+x*TileSize, top+y*TileSize, \
                         TileSize, TileSize )

#define invert_tile( x, y ) \
    rb->lcd_set_drawmode(DRMODE_COMPLEMENT); \
    rb->lcd_fillrect( left+x*TileSize, top+y*TileSize, TileSize, TileSize ); \
    rb->lcd_set_drawmode(DRMODE_SOLID);


/* the tile struct
 * if there is a mine, mine is true
 * if tile is known by player, known is true
 * if tile has a flag, flag is true
 * neighbors is the total number of mines arround tile
 */
typedef struct tile
{
    unsigned char mine : 1;
    unsigned char known : 1;
    unsigned char flag : 1;
    unsigned char neighbors : 4;
} tile;

/* the height and width of the field */
#define MAX_HEIGHT (LCD_HEIGHT/TileSize)
#define MAX_WIDTH  (LCD_WIDTH/TileSize)
static int height = MAX_HEIGHT;
static int width = MAX_WIDTH;
static int top;
static int left;

/* The Minefield. Caution it is defined as Y, X! Not the opposite. */
static tile minefield[MAX_HEIGHT][MAX_WIDTH];

/* total number of mines on the game */
static int mine_num = 0;

/* percentage of mines on minefield used during generation */
static int percent = 16;

/* number of tiles left on the game */
static int tiles_left;

/* Because mines are set after the first move... */
static bool no_mines = true;

/* We need a stack (created on discover()) for the cascade algorithm. */
static int stack_pos = 0;

#ifdef HAVE_TOUCHSCREEN

#include "lib/pluginlib_touchscreen.h"
static struct ts_raster mine_raster = { 0, 0, MAX_WIDTH, MAX_HEIGHT, TileSize, TileSize };
#endif


static void push( int *stack, int y, int x )
{
    if( stack_pos <= height*width )
    {
        stack[++stack_pos] = y;
        stack[++stack_pos] = x;
    }
}

/* Unveil tiles and push them to stack if they are empty. */
static void unveil( int *stack, int y, int x )
{
    if( x < 0 || y < 0 || x > width - 1 || y > height - 1
       || minefield[y][x].known
       || minefield[y][x].mine || minefield[y][x].flag ) return;

    minefield[y][x].known = 1;

    if( minefield[y][x].neighbors == 0 )
        push( stack, y, x );
}

static int is_flagged( int y, int x )
{
    if( x >= 0 && y >= 0 && x < width && y < height && minefield[y][x].flag )
        return 1;
    return 0;
}

static int neighbors_flagged( int y, int x )
{
    return is_flagged( y-1, x-1 ) +
           is_flagged( y-1, x ) +
           is_flagged( y-1, x+1 ) +
           is_flagged( y, x-1 ) +
           is_flagged( y, x ) +
           is_flagged( y, x+1 ) +
           is_flagged( y+1, x-1 ) +
           is_flagged( y+1, x ) +
           is_flagged( y+1, x+1 );
}

static bool discover( int y, int x, bool explore_neighbors )
{
    /* Selected tile. */
    if( x < 0 || y < 0 || x > width - 1 || y > height - 1)
        return false;

    if( minefield[y][x].known || minefield[y][x].mine || minefield[y][x].flag )
    {
        if( !minefield[y][x].flag && minefield[y][x].mine )
            return true;

        if( explore_neighbors && minefield[y][x].known &&
            minefield[y][x].neighbors == neighbors_flagged( y, x ) )
        {
            return discover( y-1, x-1, false ) ||
                   discover( y-1, x, false ) ||
                   discover( y-1, x+1, false ) ||
                   discover( y, x-1, false ) ||
                   discover( y, x, false ) ||
                   discover( y, x+1, false ) ||
                   discover( y+1, x-1, false ) ||
                   discover( y+1, x, false ) ||
                   discover( y+1, x+1, false );
        }

        return false;
    }

    minefield[y][x].known = 1;
    /* Exit if the tile is not empty. (no mines nearby) */
    if( minefield[y][x].neighbors ) return false;

    int stack[height*width];

    push( stack, y, x );

    /* Scan all nearby tiles. If we meet a tile with a number we just unveil
     * it. If we meet an empty tile, we push the location in stack. For each
     * location in stack we do the same thing. (scan again all nearby tiles)
     */
    while( stack_pos )
    {
        /* Pop x, y from stack. */
        x = stack[stack_pos--];
        y = stack[stack_pos--];

        unveil( stack, y-1, x-1 );
        unveil( stack, y-1, x   );
        unveil( stack, y-1, x+1 );
        unveil( stack, y,   x+1 );
        unveil( stack, y+1, x+1 );
        unveil( stack, y+1, x   );
        unveil( stack, y+1, x-1 );
        unveil( stack, y,   x-1 );
    }

    return false;
}

/* Reset the whole board for a new game. */
static void minesweeper_init( void )
{
    rb->memset(minefield, 0, sizeof(minefield));
    no_mines = true;
    tiles_left = width*height;
}


/* put mines on the mine field */
/* there is p% chance that a tile is a mine */
/* if the tile has coordinates (x,y), or is adjacent to those,
 * then it can't be a mine because that would reduce the game
 * from a logic game to a guessing game. */
static void minesweeper_putmines( int p, int x, int y )
{
    int i,j;

    mine_num = 0;
    for( i = 0; i < height; i++ )
    {
        for( j = 0; j < width; j++ )
        {
            if( rb->rand()%100 < p
                && !( i>=y-1 && i<=y+1 && j>=x-1 && j<=x+1 ) )
            {
                minefield[i][j].mine = 1;
                mine_num++;
            }
            else
            {
                minefield[i][j].mine = 0;
            }
            minefield[i][j].neighbors = 0;
        }
    }

    /* we need to compute the neighbor element for each tile */
    for( i = 0; i < height; i++ )
    {
        for( j = 0; j < width; j++ )
        {
            if( i > 0 )
            {
                if( j > 0 )
                    minefield[i][j].neighbors += minefield[i-1][j-1].mine;
                minefield[i][j].neighbors += minefield[i-1][j].mine;
                if( j < width - 1 )
                    minefield[i][j].neighbors += minefield[i-1][j+1].mine;
            }
            if( j > 0 )
                minefield[i][j].neighbors += minefield[i][j-1].mine;
            if( j < width - 1 )
                minefield[i][j].neighbors += minefield[i][j+1].mine;
            if( i < height - 1 )
            {
                if( j > 0 )
                    minefield[i][j].neighbors += minefield[i+1][j-1].mine;
                minefield[i][j].neighbors += minefield[i+1][j].mine;
                if( j < width - 1 )
                    minefield[i][j].neighbors += minefield[i+1][j+1].mine;
            }
        }
    }

    no_mines = false;

    /* In case the user is lucky and there are no mines positioned. */
    if( !mine_num && height*width != 1 )
    {
        minesweeper_putmines(p, x, y);
    }
}

/* A function that will uncover all the board, when the user wins or loses.
   can easily be expanded, (just a call assigned to a button) as a solver. */
static void mine_show( void )
{
    int i, j, button;

    for( i = 0; i < height; i++ )
    {
        for( j = 0; j < width; j++ )
        {
            if( minefield[i][j].mine )
            {
                if( minefield[i][j].known )
                    draw_tile( ExplodedMine, j, i );
                else if( minefield[i][j].flag )
                    draw_tile( CorrectFlag, j, i );
                else
                    draw_tile( Mine, j, i );
            }
            else
            {
                if( minefield[i][j].flag )
                    draw_tile( WrongFlag, j, i );
                else
                    draw_tile( minefield[i][j].neighbors, j, i );
            }
        }
    }
    rb->lcd_update();

    do
        button = rb->button_get(true);
    while( ( button == BUTTON_NONE )
           || ( button & (BUTTON_REL|BUTTON_REPEAT) ) );
#ifdef HAVE_TOUCHSCREEN
    button = BUTTON_NONE;
#endif
}

static int count_tiles_left( void )
{
    int tiles = 0;
    int i, j;
    for( i = 0; i < height; i++ )
        for( j = 0; j < width; j++ )
             if( minefield[i][j].known == 0 )
                tiles++;
    return tiles;
}

static int count_flags( void )
{
    int flags = 0;
    int i, j;
    for( i = 0; i < height; i++ )
        for( j = 0; j < width; j++ )
             if( minefield[i][j].flag == 1 )
                flags++;
    return flags;
}

/* welcome screen where player can chose mine percentage */
static enum minesweeper_status menu( void )
{
    int selection = 0, result = MINESWEEPER_QUIT;
    bool menu_quit = false;

    MENUITEM_STRINGLIST( menu, "Minesweeper Menu", NULL, "Play Minesweeper",
                         "Mine Percentage", "Number of Rows",
                         "Number of Columns", "Playback Control", "Quit" );

#ifdef HAVE_LCD_COLOR
    rb->lcd_set_foreground( rb->global_settings->fg_color );
    rb->lcd_set_background( rb->global_settings->bg_color );
#endif

    while( !menu_quit )
    {
        switch( rb->do_menu( &menu, &selection, NULL, false ) )
        {
            case 0:
                result = MINESWEEPER_WIN; /* start playing */
                menu_quit = true;
                break;

            case 1:
                rb->set_int( "Mine Percentage", "%", UNIT_INT, &percent, NULL,
                             1, 2, 98, NULL );
                break;

            case 2:
                rb->set_int( "Number of Rows", "", UNIT_INT, &height, NULL,
                             1, 1, MAX_HEIGHT, NULL );
                break;

            case 3:
                rb->set_int( "Number of Columns", "", UNIT_INT, &width, NULL,
                             1, 1, MAX_WIDTH, NULL );
                break;

            case 4:
                playback_control( NULL );
                break;

            default:
                result = MINESWEEPER_QUIT; /* quit program */
                menu_quit = true;
                break;
        }
    }

    return result;
}

/* the big and ugly game function */
static enum minesweeper_status minesweeper( void )
{
    int i, j;
    int button;
#if defined(HAVE_TOUCHSCREEN) || defined(MINESWP_TOGGLE_PRE)
    int lastbutton = BUTTON_NONE;
#endif

    /* the cursor coordinates */
    int x=0, y=0;

    /**
     * Show the menu
     */
    if( ( i = menu() ) != MINESWEEPER_WIN ) return i;

    /**
     * Init game
     */
    top = (LCD_HEIGHT-height*TileSize)/2;
    left = (LCD_WIDTH-width*TileSize)/2;

#ifdef HAVE_TOUCHSCREEN
    mine_raster.tl_x = left;
    mine_raster.tl_y = top;
    mine_raster.width = width*TileSize;
    mine_raster.height = height*TileSize;
#endif

    rb->srand( *rb->current_tick );
    minesweeper_init();
    x = 0;
    y = 0;

    /**
     * Play
     */
    while( true )
    {

        /* clear the screen buffer */
#ifdef HAVE_LCD_COLOR
        rb->lcd_set_background( BackgroundColor );
#endif
        rb->lcd_clear_display();

        /* display the mine field */
        for( i = 0; i < height; i++ )
        {
            for( j = 0; j < width; j++ )
            {
                if( minefield[i][j].known )
                {
                    draw_tile( minefield[i][j].neighbors, j, i );
                }
                else if(minefield[i][j].flag)
                {
                    draw_tile( Flag, j, i );
                }
                else
                {
                    draw_tile( Unknown, j, i );
                }
            }
        }

        /* display the cursor */
        invert_tile( x, y );

        /* update the screen */
        rb->lcd_update();

        button = rb->button_get(true);
#ifdef HAVE_TOUCHSCREEN
        if(button & BUTTON_TOUCHSCREEN)
        {
            struct ts_raster_result res;
            if(touchscreen_map_raster(&mine_raster, rb->button_get_data() >> 16, rb->button_get_data() & 0xffff, &res) == 1)
            {
                button &= ~BUTTON_TOUCHSCREEN;
                lastbutton &= ~BUTTON_TOUCHSCREEN;

                if(button & BUTTON_REPEAT && lastbutton != MINESWP_TOGGLE && lastbutton ^ BUTTON_REPEAT)
                    button = MINESWP_TOGGLE;
                else if(button == BUTTON_REL && lastbutton ^ BUTTON_REPEAT)
                    button = MINESWP_DISCOVER;
                else
                    button |= BUTTON_TOUCHSCREEN;

                x = res.x;
                y = res.y;
            }
        }
#endif
        switch(button)
        {
            /* quit minesweeper (you really shouldn't use this button ...) */
#ifdef MINESWP_RC_QUIT
            case MINESWP_RC_QUIT:
#endif
            case MINESWP_QUIT:
                return MINESWEEPER_QUIT;

            /* move cursor left */
            case MINESWP_LEFT:
            case MINESWP_LEFT|BUTTON_REPEAT:
                if( --x < 0)
                    x += width;
                break;

            /* move cursor right */
            case MINESWP_RIGHT:
            case MINESWP_RIGHT|BUTTON_REPEAT:
                if( ++x >= width )
                    x -= width;
                break;

            /* move cursor down */
            case MINESWP_DOWN:
            case MINESWP_DOWN|BUTTON_REPEAT:
                if( ++y >= height )
                    y -= height;
                break;

            /* move cursor up */
            case MINESWP_UP:
            case MINESWP_UP|BUTTON_REPEAT:
                if( --y < 0 )
                    y += height;
                break;

            /*move cursor though the entire field*/
#ifdef MINESWP_SCROLLWHEEL
            case MINESWP_NEXT:
            case MINESWP_NEXT|BUTTON_REPEAT:
                if (x == width -1 ) {
                    if( ++y >= height )
                        y -= height;
                }
                if( ++x >= width )
                    x -= width;
                break;

            case MINESWP_PREV:
            case MINESWP_PREV|BUTTON_REPEAT:
                if (x == 0) {
                    if( --y < 0 )
                        y += height;
                }
                if( --x < 0 )
                    x += width;
                break;
#endif
            /* discover a tile (and it's neighbors if .neighbors == 0) */
            case MINESWP_DISCOVER:
#ifdef MINESWP_DISCOVER2
            case MINESWP_DISCOVER2:
#endif
                if( minefield[y][x].flag ) break;
                /* we put the mines on the first "click" so that you don't
                 * lose on the first "click" */
                if( tiles_left == width*height && no_mines )
                    minesweeper_putmines(percent,x,y);

                if( discover( y, x, true ) )
                {
                    minefield[y][x].known = 1;
                    return MINESWEEPER_LOSE;
                }

                tiles_left = count_tiles_left();
                if( tiles_left == mine_num )
                {
                    return MINESWEEPER_WIN;
                }
                break;

            /* toggle flag under cursor */
            case MINESWP_TOGGLE:
#ifdef MINESWP_TOGGLE_PRE
                if( lastbutton != MINESWP_TOGGLE_PRE )
                    break;
#endif
#ifdef MINESWP_TOGGLE2
            case MINESWP_TOGGLE2:
#endif
                if( !minefield[y][x].known )
                    minefield[y][x].flag = !minefield[y][x].flag;
                break;

            /* show how many mines you think you have found and how many
             * there really are on the game */
            case MINESWP_INFO:
                if( no_mines )
                    break;
                int flags_used = count_flags();
                if (flags_used == 1) {
                    rb->splashf( HZ*2, "You marked 1 field. There are %d mines.",
                                mine_num );
                }
                else
                {
                    rb->splashf( HZ*2, "You marked %d fields. There are %d mines.",
                                flags_used, mine_num );
                }
                break;

            default:
                if( rb->default_event_handler( button ) == SYS_USB_CONNECTED )
                    return MINESWEEPER_USB;
                break;
        }
#if defined(HAVE_TOUCHSCREEN) || defined(MINESWP_TOGGLE_PRE)
        if( button != BUTTON_NONE )
            lastbutton = button;
#endif
    }

}

/* plugin entry point */
enum plugin_status plugin_start(const void* parameter)
{
    bool exit = false;

    (void)parameter;
#if LCD_DEPTH > 1
    rb->lcd_set_backdrop(NULL);
#endif

    while( !exit )
    {
        switch( minesweeper() )
        {
            case MINESWEEPER_WIN:
                rb->splash( HZ, "You Win!" );
                rb->lcd_clear_display();
                mine_show();
                break;

            case MINESWEEPER_LOSE:
                rb->splash( HZ, "You Lose!" );
                rb->lcd_clear_display();
                mine_show();
                break;

            case MINESWEEPER_USB:
                return PLUGIN_USB_CONNECTED;

            case MINESWEEPER_QUIT:
                exit = true;
                break;

            default:
                break;
        }
    }

    return PLUGIN_OK;
}
