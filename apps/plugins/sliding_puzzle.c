/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2002 Vicentini Martin
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

/* variable button definitions */
#if CONFIG_KEYPAD == RECORDER_PAD
#define PUZZLE_QUIT BUTTON_OFF
#define PUZZLE_LEFT BUTTON_LEFT
#define PUZZLE_RIGHT BUTTON_RIGHT
#define PUZZLE_UP BUTTON_UP
#define PUZZLE_DOWN BUTTON_DOWN
#define PUZZLE_SHUFFLE BUTTON_F1
#define PUZZLE_PICTURE BUTTON_F2

#elif CONFIG_KEYPAD == ARCHOS_AV300_PAD
#define PUZZLE_QUIT BUTTON_OFF
#define PUZZLE_LEFT BUTTON_LEFT
#define PUZZLE_RIGHT BUTTON_RIGHT
#define PUZZLE_UP BUTTON_UP
#define PUZZLE_DOWN BUTTON_DOWN
#define PUZZLE_SHUFFLE BUTTON_F1
#define PUZZLE_PICTURE BUTTON_F2

#elif CONFIG_KEYPAD == ONDIO_PAD
#define PUZZLE_QUIT BUTTON_OFF
#define PUZZLE_LEFT BUTTON_LEFT
#define PUZZLE_RIGHT BUTTON_RIGHT
#define PUZZLE_UP BUTTON_UP
#define PUZZLE_DOWN BUTTON_DOWN
#define PUZZLE_SHUFFLE_PICTURE_PRE BUTTON_MENU
#define PUZZLE_SHUFFLE (BUTTON_MENU | BUTTON_REPEAT)
#define PUZZLE_PICTURE (BUTTON_MENU | BUTTON_REL)

#elif (CONFIG_KEYPAD == IRIVER_H100_PAD) || \
      (CONFIG_KEYPAD == IRIVER_H300_PAD)
#define PUZZLE_QUIT BUTTON_OFF
#define PUZZLE_LEFT BUTTON_LEFT
#define PUZZLE_RIGHT BUTTON_RIGHT
#define PUZZLE_UP BUTTON_UP
#define PUZZLE_DOWN BUTTON_DOWN
#define PUZZLE_SHUFFLE BUTTON_SELECT
#define PUZZLE_PICTURE BUTTON_ON

#define PUZZLE_RC_QUIT BUTTON_RC_STOP

#elif (CONFIG_KEYPAD == IPOD_4G_PAD) || \
      (CONFIG_KEYPAD == IPOD_3G_PAD) || \
      (CONFIG_KEYPAD == IPOD_1G2G_PAD)
#define PUZZLE_QUIT    (BUTTON_SELECT | BUTTON_MENU)
#define PUZZLE_LEFT BUTTON_LEFT
#define PUZZLE_RIGHT BUTTON_RIGHT
#define PUZZLE_UP      BUTTON_MENU
#define PUZZLE_DOWN    BUTTON_PLAY
#define PUZZLE_SHUFFLE (BUTTON_SELECT | BUTTON_LEFT)
#define PUZZLE_PICTURE (BUTTON_SELECT | BUTTON_RIGHT)

#elif (CONFIG_KEYPAD == IAUDIO_X5M5_PAD)
#define PUZZLE_QUIT BUTTON_POWER
#define PUZZLE_LEFT BUTTON_LEFT
#define PUZZLE_RIGHT BUTTON_RIGHT
#define PUZZLE_UP BUTTON_UP
#define PUZZLE_DOWN BUTTON_DOWN
#define PUZZLE_SHUFFLE BUTTON_REC
#define PUZZLE_PICTURE BUTTON_PLAY

#elif (CONFIG_KEYPAD == GIGABEAT_PAD)
#define PUZZLE_QUIT BUTTON_POWER
#define PUZZLE_LEFT BUTTON_LEFT
#define PUZZLE_RIGHT BUTTON_RIGHT
#define PUZZLE_UP BUTTON_UP
#define PUZZLE_DOWN BUTTON_DOWN
#define PUZZLE_SHUFFLE BUTTON_SELECT
#define PUZZLE_PICTURE BUTTON_A

#elif (CONFIG_KEYPAD == SANSA_E200_PAD) || \
(CONFIG_KEYPAD == SANSA_C200_PAD) 
#define PUZZLE_QUIT BUTTON_POWER
#define PUZZLE_LEFT BUTTON_LEFT
#define PUZZLE_RIGHT BUTTON_RIGHT
#define PUZZLE_UP BUTTON_UP
#define PUZZLE_DOWN BUTTON_DOWN
#define PUZZLE_SHUFFLE BUTTON_REC
#define PUZZLE_PICTURE BUTTON_SELECT

#elif (CONFIG_KEYPAD == SANSA_FUZE_PAD)
#define PUZZLE_QUIT (BUTTON_HOME|BUTTON_REPEAT)
#define PUZZLE_LEFT BUTTON_LEFT
#define PUZZLE_RIGHT BUTTON_RIGHT
#define PUZZLE_UP BUTTON_UP
#define PUZZLE_DOWN BUTTON_DOWN
#define PUZZLE_SHUFFLE BUTTON_SELECT|BUTTON_DOWN
#define PUZZLE_PICTURE BUTTON_SELECT

#elif (CONFIG_KEYPAD == SANSA_CLIP_PAD)
#define PUZZLE_QUIT BUTTON_POWER
#define PUZZLE_LEFT BUTTON_LEFT
#define PUZZLE_RIGHT BUTTON_RIGHT
#define PUZZLE_UP BUTTON_UP
#define PUZZLE_DOWN BUTTON_DOWN
#define PUZZLE_SHUFFLE BUTTON_HOME
#define PUZZLE_PICTURE BUTTON_SELECT

#elif (CONFIG_KEYPAD == SANSA_M200_PAD)
#define PUZZLE_QUIT BUTTON_POWER
#define PUZZLE_LEFT BUTTON_LEFT
#define PUZZLE_RIGHT BUTTON_RIGHT
#define PUZZLE_UP BUTTON_UP
#define PUZZLE_DOWN BUTTON_DOWN
#define PUZZLE_SHUFFLE (BUTTON_SELECT | BUTTON_UP)
#define PUZZLE_PICTURE (BUTTON_SELECT | BUTTON_REL)

#elif (CONFIG_KEYPAD == IRIVER_H10_PAD)
#define PUZZLE_QUIT BUTTON_POWER
#define PUZZLE_LEFT BUTTON_LEFT
#define PUZZLE_RIGHT BUTTON_RIGHT
#define PUZZLE_UP BUTTON_SCROLL_UP
#define PUZZLE_DOWN BUTTON_SCROLL_DOWN
#define PUZZLE_SHUFFLE BUTTON_REW
#define PUZZLE_PICTURE BUTTON_PLAY

#elif (CONFIG_KEYPAD == GIGABEAT_S_PAD) || \
      (CONFIG_KEYPAD == SAMSUNG_YPR0_PAD)
#define PUZZLE_QUIT BUTTON_BACK
#define PUZZLE_LEFT BUTTON_LEFT
#define PUZZLE_RIGHT BUTTON_RIGHT
#define PUZZLE_UP BUTTON_UP
#define PUZZLE_DOWN BUTTON_DOWN
#define PUZZLE_SHUFFLE BUTTON_SELECT
#define PUZZLE_PICTURE BUTTON_MENU

#elif (CONFIG_KEYPAD == MROBE100_PAD)
#define PUZZLE_QUIT BUTTON_POWER
#define PUZZLE_LEFT BUTTON_LEFT
#define PUZZLE_RIGHT BUTTON_RIGHT
#define PUZZLE_UP BUTTON_UP
#define PUZZLE_DOWN BUTTON_DOWN
#define PUZZLE_SHUFFLE BUTTON_SELECT
#define PUZZLE_PICTURE BUTTON_DISPLAY

#elif CONFIG_KEYPAD == IAUDIO_M3_PAD
#define PUZZLE_QUIT BUTTON_RC_REC
#define PUZZLE_LEFT BUTTON_RC_REW
#define PUZZLE_RIGHT BUTTON_RC_FF
#define PUZZLE_UP BUTTON_RC_VOL_UP
#define PUZZLE_DOWN BUTTON_RC_VOL_DOWN
#define PUZZLE_SHUFFLE BUTTON_RC_MODE
#define PUZZLE_PICTURE BUTTON_RC_MENU

#elif (CONFIG_KEYPAD == COWON_D2_PAD)
#define PUZZLE_QUIT BUTTON_POWER
#define PUZZLE_QUIT_TEXT "[POWER]"

#elif CONFIG_KEYPAD == CREATIVEZVM_PAD
#define PUZZLE_QUIT BUTTON_BACK
#define PUZZLE_LEFT BUTTON_LEFT
#define PUZZLE_RIGHT BUTTON_RIGHT
#define PUZZLE_UP BUTTON_UP
#define PUZZLE_DOWN BUTTON_DOWN
#define PUZZLE_SHUFFLE BUTTON_PLAY
#define PUZZLE_PICTURE BUTTON_MENU

#elif (CONFIG_KEYPAD == CREATIVE_ZENXFI3_PAD)
#define PUZZLE_QUIT BUTTON_POWER
#define PUZZLE_LEFT BUTTON_BACK
#define PUZZLE_RIGHT BUTTON_MENU
#define PUZZLE_UP BUTTON_UP
#define PUZZLE_DOWN BUTTON_DOWN
#define PUZZLE_SHUFFLE BUTTON_VOL_UP
#define PUZZLE_PICTURE BUTTON_PLAY

#elif CONFIG_KEYPAD == PHILIPS_HDD1630_PAD
#define PUZZLE_QUIT BUTTON_POWER
#define PUZZLE_LEFT BUTTON_LEFT
#define PUZZLE_RIGHT BUTTON_RIGHT
#define PUZZLE_UP BUTTON_UP
#define PUZZLE_DOWN BUTTON_DOWN
#define PUZZLE_SHUFFLE BUTTON_VIEW
#define PUZZLE_PICTURE BUTTON_MENU

#elif CONFIG_KEYPAD == PHILIPS_HDD6330_PAD
#define PUZZLE_QUIT BUTTON_POWER
#define PUZZLE_LEFT BUTTON_LEFT
#define PUZZLE_RIGHT BUTTON_RIGHT
#define PUZZLE_UP BUTTON_UP
#define PUZZLE_DOWN BUTTON_DOWN
#define PUZZLE_SHUFFLE BUTTON_NEXT
#define PUZZLE_PICTURE BUTTON_MENU

#elif CONFIG_KEYPAD == PHILIPS_SA9200_PAD
#define PUZZLE_QUIT BUTTON_POWER
#define PUZZLE_LEFT BUTTON_PREV
#define PUZZLE_RIGHT BUTTON_NEXT
#define PUZZLE_UP BUTTON_UP
#define PUZZLE_DOWN BUTTON_DOWN
#define PUZZLE_SHUFFLE BUTTON_RIGHT
#define PUZZLE_PICTURE BUTTON_MENU

#elif CONFIG_KEYPAD == ONDAVX747_PAD || \
CONFIG_KEYPAD == ONDAVX777_PAD || \
CONFIG_KEYPAD == MROBE500_PAD
#define PUZZLE_QUIT BUTTON_POWER
#define PUZZLE_QUIT_TEXT "[POWER]"

#elif (CONFIG_KEYPAD == SAMSUNG_YH_PAD)
#define PUZZLE_QUIT    BUTTON_REC
#define PUZZLE_LEFT    BUTTON_LEFT
#define PUZZLE_RIGHT   BUTTON_RIGHT
#define PUZZLE_UP      BUTTON_UP
#define PUZZLE_DOWN    BUTTON_DOWN
#define PUZZLE_SHUFFLE BUTTON_REW
#define PUZZLE_PICTURE BUTTON_PLAY

#elif (CONFIG_KEYPAD == PBELL_VIBE500_PAD)
#define PUZZLE_QUIT    BUTTON_REC
#define PUZZLE_LEFT    BUTTON_PREV
#define PUZZLE_RIGHT   BUTTON_NEXT
#define PUZZLE_UP      BUTTON_UP
#define PUZZLE_DOWN    BUTTON_DOWN
#define PUZZLE_SHUFFLE BUTTON_CANCEL
#define PUZZLE_PICTURE BUTTON_MENU

#elif CONFIG_KEYPAD == MPIO_HD200_PAD
#define PUZZLE_QUIT    (BUTTON_REC | BUTTON_PLAY)
#define PUZZLE_LEFT BUTTON_VOL_DOWN
#define PUZZLE_RIGHT BUTTON_VOL_UP
#define PUZZLE_UP      BUTTON_REW
#define PUZZLE_DOWN    BUTTON_FF
#define PUZZLE_SHUFFLE BUTTON_REC
#define PUZZLE_PICTURE BUTTON_PLAY

#elif CONFIG_KEYPAD == MPIO_HD300_PAD
#define PUZZLE_QUIT    (BUTTON_MENU | BUTTON_REPEAT)
#define PUZZLE_LEFT BUTTON_FF
#define PUZZLE_RIGHT BUTTON_REW
#define PUZZLE_UP      BUTTON_DOWN
#define PUZZLE_DOWN    BUTTON_UP
#define PUZZLE_SHUFFLE BUTTON_ENTER
#define PUZZLE_PICTURE BUTTON_PLAY

#elif CONFIG_KEYPAD == SANSA_FUZEPLUS_PAD
#define PUZZLE_QUIT    BUTTON_POWER
#define PUZZLE_LEFT    BUTTON_LEFT
#define PUZZLE_RIGHT   BUTTON_RIGHT
#define PUZZLE_UP      BUTTON_UP
#define PUZZLE_DOWN    BUTTON_DOWN
#define PUZZLE_SHUFFLE BUTTON_PLAYPAUSE|BUTTON_REPEAT
#define PUZZLE_PICTURE BUTTON_SELECT|BUTTON_REPEAT

#elif CONFIG_KEYPAD == SANSA_CONNECT_PAD
#define PUZZLE_QUIT    BUTTON_POWER
#define PUZZLE_LEFT    BUTTON_LEFT
#define PUZZLE_RIGHT   BUTTON_RIGHT
#define PUZZLE_UP      BUTTON_UP
#define PUZZLE_DOWN    BUTTON_DOWN
#define PUZZLE_SHUFFLE BUTTON_VOL_DOWN
#define PUZZLE_PICTURE BUTTON_SELECT

#elif (CONFIG_KEYPAD == HM60X_PAD)
#define PUZZLE_QUIT    BUTTON_POWER
#define PUZZLE_LEFT    BUTTON_LEFT
#define PUZZLE_RIGHT   BUTTON_RIGHT
#define PUZZLE_UP      BUTTON_UP
#define PUZZLE_DOWN    BUTTON_DOWN
#define PUZZLE_SHUFFLE BUTTON_SELECT
#define PUZZLE_PICTURE (BUTTON_POWER|BUTTON_SELECT)

#elif (CONFIG_KEYPAD == HM801_PAD)
#define PUZZLE_QUIT    BUTTON_POWER
#define PUZZLE_LEFT    BUTTON_LEFT
#define PUZZLE_RIGHT   BUTTON_RIGHT
#define PUZZLE_UP      BUTTON_UP
#define PUZZLE_DOWN    BUTTON_DOWN
#define PUZZLE_SHUFFLE BUTTON_SELECT
#define PUZZLE_PICTURE BUTTON_PLAY

#else
#error No keymap defined!
#endif

#ifdef HAVE_TOUCHSCREEN
#ifndef PUZZLE_QUIT
#define PUZZLE_QUIT    BUTTON_TOPLEFT
#endif
#ifndef PUZZLE_LEFT
#define PUZZLE_LEFT    BUTTON_MIDLEFT
#endif
#ifndef PUZZLE_RIGHT
#define PUZZLE_RIGHT   BUTTON_MIDRIGHT
#endif
#ifndef PUZZLE_UP
#define PUZZLE_UP      BUTTON_TOPMIDDLE
#endif
#ifndef PUZZLE_DOWN
#define PUZZLE_DOWN    BUTTON_BOTTOMMIDDLE
#endif
#ifndef PUZZLE_SHUFFLE
#define PUZZLE_SHUFFLE BUTTON_BOTTOMLEFT
#endif
#ifndef PUZZLE_PICTURE
#define PUZZLE_PICTURE BUTTON_CENTER
#endif
#ifndef PUZZLE_QUIT_TEXT 
#define PUZZLE_QUIT_TEXT "[TOPLEFT]"
#endif
#ifndef PUZZLE_SHUFFLE_TEXT 
#define PUZZLE_SHUFFLE_TEXT "[BOTTOMLEFT]"
#endif
#ifndef PUZZLE_PICTURE_TEXT 
#define PUZZLE_PICTURE_TEXT "[CENTER]"
#endif
#endif

#ifdef HAVE_ALBUMART
#include "lib/read_image.h"
#define READ_IMAGE read_image_file
#else
#define READ_IMAGE rb->read_bmp_file
#endif

#include "pluginbitmaps/sliding_puzzle.h"
#define IMAGE_WIDTH BMPWIDTH_sliding_puzzle
#define IMAGE_HEIGHT BMPHEIGHT_sliding_puzzle
#define IMAGE_SIZE IMAGE_WIDTH

/* use a square image, (the default Archos bitmap looks square on its display)
   Puzzle image dimension is min(lcd_height,lcd_width)
   4x4 is more convenient for square puzzles
   Note: sliding_puzzle.bmp should be evenly divisible by SPOTS_X
   and SPOTS_Y, otherwise lcd_bitmap_part stride won't be correct */
#define SPOTS_X 4
#define SPOTS_Y 4
#define SPOTS_WIDTH  (IMAGE_WIDTH / SPOTS_X)
#define SPOTS_HEIGHT (IMAGE_HEIGHT / SPOTS_Y)
#define NUM_SPOTS    (SPOTS_X*SPOTS_Y)
#define HOLE_ID      (NUM_SPOTS)
#define INITIAL_HOLE (HOLE_ID-1)

enum picmodes
{
    PICMODE_NUMERALS = 0,
    PICMODE_INITIAL_PICTURE,
    PICMODE_DEFAULT_PICTURE,
#ifdef HAVE_ALBUMART
    PICMODE_ALBUM_ART,
#endif
//    PICMODE_RANDOM,
    PICMODE_LAST_XXX /* placeholder */
};

static const char* const picmode_descriptions[] = {
    "Numerals",
    "Viewer Picture",
    "Default Picture",
#ifdef HAVE_ALBUMART
    "Album Art",
#endif
    "Shouldn't Get Here",
};

static int spots[NUM_SPOTS];
static int hole = INITIAL_HOLE, moves;
static unsigned char s[32];
static enum picmodes picmode = PICMODE_INITIAL_PICTURE;
static int num_font = FONT_UI;
static int moves_font = FONT_UI;
static int moves_y = 0;

static unsigned char *img_buf;
static size_t img_buf_len;
#ifdef HAVE_ALBUMART
static char albumart_path[MAX_PATH+1];
#endif
static char img_buf_path[MAX_PATH+1];

static const fb_data * puzzle_bmp_ptr;
/* initial_bmp_path points to selected bitmap if this game is launched
   as a viewer for a .bmp file, or NULL if game is launched regular way */
static const char * initial_bmp_path=NULL;

#ifdef HAVE_ALBUMART
static const char * get_albumart_bmp_path(void)
{
    struct mp3entry* track = rb->audio_current_track();

    if (!track || !track->path || track->path[0] == '\0')
        return NULL;

    if (!rb->search_albumart_files(track, "", albumart_path, MAX_PATH ) )
        return NULL;

    albumart_path[ MAX_PATH ] = '\0';
    return albumart_path;
}
#endif

#if 0 /* unused */
static const char * get_random_bmp_path(void)
{
    return(initial_bmp_path);
}
#endif

static bool load_resize_bitmap(void)
{
    int rc;
    const char * filename = NULL;

    /* initially assume using the built-in default */
    puzzle_bmp_ptr = sliding_puzzle;

    switch( picmode ){
        /* some modes don't even need to touch disk and trivially succeed */
        case PICMODE_NUMERALS:
        case PICMODE_DEFAULT_PICTURE:
        default:
            return(true);

#ifdef HAVE_ALBUMART
        case PICMODE_ALBUM_ART:
            filename = get_albumart_bmp_path();
            break;
#endif
/*
        case PICMODE_RANDOM:
            if(NULL == (filename=get_random_bmp_path()) )
                filename = initial_bmp_path;
            break;
*/
        case PICMODE_INITIAL_PICTURE:
            filename = initial_bmp_path;
            break;
    };

    if( filename != NULL )
    {
        /* if we already loaded image before, don't touch disk */
        if( 0 == rb->strcmp( filename, img_buf_path ) )
        {
            puzzle_bmp_ptr = (const fb_data *)img_buf;
            return true;
        }

        struct bitmap main_bitmap;
        rb->memset(&main_bitmap,0,sizeof(struct bitmap));
        main_bitmap.data = img_buf;

        main_bitmap.width = IMAGE_WIDTH;
        main_bitmap.height = IMAGE_HEIGHT;

        rc = READ_IMAGE( filename, &main_bitmap,
                         img_buf_len,
                         FORMAT_NATIVE|FORMAT_RESIZE|FORMAT_DITHER,
                         NULL);
        if( rc > 0 )
        {
            puzzle_bmp_ptr = (const fb_data *)img_buf;
            rb->strcpy( img_buf_path, filename );
            return true;
        }
    }

    /* something must have failed. get_albumart_bmp_path could return
       NULL if albumart doesn't exist or couldn't be loaded, or
       read_bmp_file could have failed.  return false and caller should
       try the next mode (PICMODE_DEFAULT_PICTURE and PICMODE_NUMERALS will
       always succeed) */
    return false;
}

/* draws a spot at the coordinates (x,y), range of p is 1-20 */
static void draw_spot(int p, int x, int y)
{
    int w, h;

    if (p == HOLE_ID)
    {
#if LCD_DEPTH==1
        /* the bottom-right cell of the default sliding_puzzle image is
           an appropriate hole graphic */
        rb->lcd_bitmap_part(sliding_puzzle, ((p-1)%SPOTS_X)*SPOTS_WIDTH,
                    ((p-1)/SPOTS_X)*SPOTS_HEIGHT,
                    STRIDE( SCREEN_MAIN, 
                            BMPWIDTH_sliding_puzzle, BMPHEIGHT_sliding_puzzle),
                    x, y, SPOTS_WIDTH, SPOTS_HEIGHT);
#else
        /* just draw a black rectangle */
        int old_fg = rb->lcd_get_foreground();
        rb->lcd_set_foreground(LCD_BLACK);
        rb->lcd_fillrect(x,y,SPOTS_WIDTH,SPOTS_HEIGHT);
        rb->lcd_set_foreground(LCD_WHITE);
        rb->lcd_drawrect(x,y,SPOTS_WIDTH,SPOTS_HEIGHT);
        rb->lcd_set_foreground(old_fg);
#endif
    }
    else if (picmode != PICMODE_NUMERALS)
    {
        rb->lcd_bitmap_part( puzzle_bmp_ptr, ((p-1)%SPOTS_X)*SPOTS_WIDTH,
                     ((p-1)/SPOTS_X)*SPOTS_HEIGHT,
                     STRIDE( SCREEN_MAIN, 
                             BMPWIDTH_sliding_puzzle, BMPHEIGHT_sliding_puzzle), 
                     x, y, SPOTS_WIDTH, SPOTS_HEIGHT);
    } else {
        rb->lcd_drawrect(x, y, SPOTS_WIDTH, SPOTS_HEIGHT);
        rb->lcd_set_drawmode(DRMODE_SOLID|DRMODE_INVERSEVID);
        rb->lcd_fillrect(x+1, y+1, SPOTS_WIDTH-2, SPOTS_HEIGHT-2);
        rb->lcd_set_drawmode(DRMODE_SOLID);
        rb->snprintf(s, sizeof(s), "%d", p);
        rb->lcd_setfont(num_font);
        rb->lcd_getstringsize(s, &w, &h);
        rb->lcd_putsxy(x + (SPOTS_WIDTH/2) - w / 2,
                       y + (SPOTS_HEIGHT/2) - h / 2, s);
    }
}

/* check if the puzzle is solved */
static bool puzzle_finished(void)
{
    int i;
    for (i=0; i<NUM_SPOTS; i++)
    if (spots[i] != (i+1))
        return false;
    return true;
}

/* move a piece in any direction */
static void move_spot(int x, int y)
{
    int i, w;
    spots[hole] = spots[hole-x-SPOTS_X*y];
    hole -= (x+SPOTS_X*y);
    moves++;
    rb->lcd_setfont(moves_font);
#if LCD_WIDTH > LCD_HEIGHT
    rb->snprintf(s, sizeof(s), "%d", moves);
    w = rb->lcd_getstringsize(s, NULL, NULL);
    rb->lcd_putsxy((IMAGE_WIDTH+1+(LCD_WIDTH-IMAGE_WIDTH-1)/2) - w / 2,
                   moves_y, s);
#else
    (void)w;
    rb->lcd_putsxyf(3, moves_y, "Moves: %d", moves);
#endif
    for(i=1;i<=4;i++)
    {
       draw_spot(HOLE_ID,
                 (hole%SPOTS_X)*SPOTS_WIDTH,
                 (hole/SPOTS_X)*SPOTS_HEIGHT);
       draw_spot(spots[hole],
                 (hole%SPOTS_X)*SPOTS_WIDTH + (i*x*SPOTS_WIDTH)/5,
                 (hole/SPOTS_X)*SPOTS_HEIGHT + (i*y*SPOTS_HEIGHT)/5);
       rb->lcd_update();
       rb->sleep(HZ/50);
    }
    draw_spot(HOLE_ID,
              (hole%SPOTS_X)*SPOTS_WIDTH,
              (hole/SPOTS_X)*SPOTS_HEIGHT);
    draw_spot(spots[hole],
              ((hole%SPOTS_X)+x)*SPOTS_WIDTH,
              ((hole/SPOTS_X)+y)*SPOTS_HEIGHT);
    rb->lcd_update();

    spots[hole] = HOLE_ID;
}

static void draw_playfield(void)
{
    int i, w;

    rb->lcd_clear_display();
    rb->lcd_setfont(moves_font);
#if LCD_WIDTH > LCD_HEIGHT
    rb->lcd_vline(IMAGE_WIDTH, 0, LCD_HEIGHT-1);
    w = rb->lcd_getstringsize("Moves", NULL, NULL);
    rb->lcd_putsxy((IMAGE_WIDTH+1+(LCD_WIDTH-IMAGE_WIDTH-1)/2) - w / 2,
                   10, "Moves");
    rb->snprintf(s, sizeof(s), "%d", moves);
    w = rb->lcd_getstringsize(s, NULL, NULL);
    rb->lcd_putsxy((IMAGE_WIDTH+1+(LCD_WIDTH-IMAGE_WIDTH-1)/2) - w / 2,
                   moves_y, s);
#else
    (void)w;
    rb->lcd_hline(0, LCD_WIDTH-1, IMAGE_HEIGHT);
    rb->lcd_putsxyf(3, moves_y, "Moves: %d", moves);
#endif

    /* draw spots to the lcd */
    for (i=0; i<NUM_SPOTS; i++)
        draw_spot(spots[i], (i%SPOTS_X)*SPOTS_WIDTH, (i/SPOTS_X)*SPOTS_HEIGHT);

    rb->lcd_update();
}

/* initializes the puzzle */
static void puzzle_init(void)
{
    int i, r, temp, tsp[NUM_SPOTS];

    moves = 0;

    /* shuffle spots */
    for (i=NUM_SPOTS-1; i>=0; i--) {
        r = (rb->rand() % (i+1));

        temp = spots[r];
        spots[r] = spots[i];
        spots[i] = temp;

        if (spots[i]==HOLE_ID)
            hole = i;
    }

    /* test if the puzzle is solvable */
    for (i=0; i<NUM_SPOTS; i++)
        tsp[i] = spots[i];
    r=0;

    /* First, check if the problem has even or odd parity,
       depending on where the empty square is */
    if ((((SPOTS_X-1)-hole%SPOTS_X) + ((SPOTS_Y-1)-hole/SPOTS_X))%2 == 1)
        ++r;

    /* Now check how many swaps we need to solve it */
    for (i=0; i<NUM_SPOTS-1; i++) {
        while (tsp[i] != (i+1)) {
            temp = tsp[i];
            tsp[i] = tsp[temp-1];
            tsp[temp-1] = temp;
            ++r;
        }
    }

    /* if the random puzzle isn't solvable just change two spots */
    if (r%2 == 1) {
        if (spots[0]!=HOLE_ID && spots[1]!=HOLE_ID) {
            temp = spots[0];
            spots[0] = spots[1];
            spots[1] = temp;
        } else {
            temp = spots[2];
            spots[2] = spots[3];
            spots[3] = temp;
        }
    }

    draw_playfield();
}

/* the main game loop */
static int puzzle_loop(void)
{
    int button;
#if defined(PUZZLE_SHUFFLE_PICTURE_PRE)
    int lastbutton = BUTTON_NONE;
#endif
    bool load_success;

    puzzle_init();
    while(true) {
        button = rb->button_get(true);
        switch (button) {
#ifdef PUZZLE_RC_QUIT
            case PUZZLE_RC_QUIT:
#endif
            case PUZZLE_QUIT:
                /* get out of here */
                return PLUGIN_OK;

            case PUZZLE_SHUFFLE:
#ifdef PUZZLE_SHUFFLE_PICTURE_PRE
                if (lastbutton != PUZZLE_SHUFFLE_PICTURE_PRE)
                    break;
#endif
                /* mix up the pieces */
                puzzle_init();
                break;

            case PUZZLE_PICTURE:
#ifdef PUZZLE_SHUFFLE_PICTURE_PRE
                if (lastbutton != PUZZLE_SHUFFLE_PICTURE_PRE)
                    break;
#endif
                /* change picture */
                picmode = (picmode+1)%PICMODE_LAST_XXX;

                /* if load_resize_bitmap fails to load bitmap, try next picmode
                */
                do
                {
                    load_success = load_resize_bitmap();
                    if( !load_success )
                        picmode = (picmode+1)%PICMODE_LAST_XXX;
                }
                while( !load_success );

                /* tell the user what mode we picked in the end! */
                rb->splash(HZ,picmode_descriptions[ picmode ] );
                draw_playfield();
                break;

            case PUZZLE_LEFT:
                if ((hole%SPOTS_X)<(SPOTS_X-1) && !puzzle_finished())
                    move_spot(-1, 0);
                break;

            case PUZZLE_RIGHT:
                if ((hole%SPOTS_X)>0 && !puzzle_finished())
                    move_spot(1, 0);
                break;

            case PUZZLE_UP:
                if ((hole/SPOTS_X)<(SPOTS_Y-1) && !puzzle_finished())
                    move_spot(0, -1);
                break;

            case PUZZLE_DOWN:
                if ((hole/SPOTS_X)>0 && !puzzle_finished())
                    move_spot(0, 1);
                break;

            default:
                if (rb->default_event_handler(button) == SYS_USB_CONNECTED)
                    return PLUGIN_USB_CONNECTED;
                break;
        }
#if defined(PUZZLE_SHUFFLE_PICTURE_PRE)
        if (button != BUTTON_NONE)
            lastbutton = button;
#endif
    }
}

enum plugin_status plugin_start(
                                const void* parameter)
{
    int i, w, h;

    initial_bmp_path=(const char *)parameter;
    img_buf = rb->plugin_get_buffer(&img_buf_len);
    picmode = PICMODE_INITIAL_PICTURE;
    img_buf_path[0] = '\0';

    /* If launched as a viewer, just go straight to the game without
       bothering with the splash or instructions page */
    if(parameter==NULL)
    {
        /* if not launched as a viewer, use default puzzle, and show help */
        picmode = PICMODE_DEFAULT_PICTURE;

        /* print title */
        rb->lcd_getstringsize((unsigned char *)"Sliding Puzzle", &w, &h);
        w = (w+1)/2;
        h = (h+1)/2;
        rb->lcd_clear_display();
        rb->lcd_putsxy(LCD_WIDTH/2-w, (LCD_HEIGHT/2)-h,
                       (unsigned char *)"Sliding Puzzle");
        rb->lcd_update();
        rb->sleep(HZ);

        /* print instructions */
        rb->lcd_clear_display();
        rb->lcd_setfont(FONT_SYSFIXED);
#if CONFIG_KEYPAD == RECORDER_PAD || CONFIG_KEYPAD == ARCHOS_AV300_PAD
        rb->lcd_putsxy(3, 18, "[OFF] to stop");
        rb->lcd_putsxy(3, 28, "[F1] shuffle");
        rb->lcd_putsxy(3, 38, "[F2] change pic");
#elif CONFIG_KEYPAD == ONDIO_PAD
        rb->lcd_putsxy(0, 18, "[OFF] to stop");
        rb->lcd_putsxy(0, 28, "[MODE..] shuffle");
        rb->lcd_putsxy(0, 38, "[MODE] change pic");
#elif (CONFIG_KEYPAD == IPOD_4G_PAD) || \
      (CONFIG_KEYPAD == IPOD_3G_PAD) || \
      (CONFIG_KEYPAD == IPOD_1G2G_PAD)
        rb->lcd_putsxy(0, 18, "[S-MENU] to stop");
        rb->lcd_putsxy(0, 28, "[S-LEFT] shuffle");
        rb->lcd_putsxy(0, 38, "[S-RIGHT] change pic");
#elif (CONFIG_KEYPAD == IRIVER_H100_PAD) || \
      (CONFIG_KEYPAD == IRIVER_H300_PAD)
        rb->lcd_putsxy(0, 18, "[STOP] to stop");
        rb->lcd_putsxy(0, 28, "[SELECT] shuffle");
        rb->lcd_putsxy(0, 38, "[PLAY] change pic");
#elif CONFIG_KEYPAD == IAUDIO_X5M5_PAD
        rb->lcd_putsxy(0, 18, "[OFF] to stop");
        rb->lcd_putsxy(0, 28, "[REC] shuffle");
        rb->lcd_putsxy(0, 38, "[PLAY] change pic");
#elif CONFIG_KEYPAD == GIGABEAT_PAD
        rb->lcd_putsxy(0, 18, "[POWER] to stop");
        rb->lcd_putsxy(0, 28, "[SELECT] shuffle");
        rb->lcd_putsxy(0, 38, "[A] change pic");
#elif (CONFIG_KEYPAD == SANSA_E200_PAD) || \
      (CONFIG_KEYPAD == SANSA_C200_PAD)
        rb->lcd_putsxy(0, 18, "[OFF] to stop");
        rb->lcd_putsxy(0, 28, "[REC] shuffle");
        rb->lcd_putsxy(0, 38, "[SELECT] change pic");
#elif CONFIG_KEYPAD == IRIVER_H10_PAD
        rb->lcd_putsxy(0, 18, "[OFF] to stop");
        rb->lcd_putsxy(0, 28, "[REW] shuffle");
        rb->lcd_putsxy(0, 38, "[PLAY] change pic");
#elif CONFIG_KEYPAD == GIGABEAT_S_PAD || \
      CONFIG_KEYPAD == SAMSUNG_YPR0_PAD
        rb->lcd_putsxy(0, 18, "[BACK] to stop");
        rb->lcd_putsxy(0, 28, "[SELECT] shuffle");
        rb->lcd_putsxy(0, 38, "[MENU] change pic");
#elif CONFIG_KEYPAD == IAUDIO_M3_PAD
        rb->lcd_putsxy(0, 18, "[REC] to stop");
        rb->lcd_putsxy(0, 28, "[MODE] shuffle");
        rb->lcd_putsxy(0, 38, "[MENU] change pic");
#endif
#ifdef HAVE_TOUCHSCREEN
        rb->lcd_putsxy(0, 18, PUZZLE_QUIT_TEXT " to stop");
        rb->lcd_putsxy(0, 28, PUZZLE_SHUFFLE_TEXT " shuffle");
        rb->lcd_putsxy(0, 38, PUZZLE_PICTURE_TEXT " change pic");
#endif
#ifdef HAVE_ALBUMART
        rb->lcd_putsxy(0,48,"    pic->albumart->num");
#else
        rb->lcd_putsxy(0,48,"    pic<->num");
#endif
        rb->lcd_update();
        rb->button_get_w_tmo(HZ*2);
    }

    hole = INITIAL_HOLE;

    if( !load_resize_bitmap() )
    {
        rb->lcd_clear_display();
        rb->splash(HZ*2,"Failed to load bitmap!");
        return PLUGIN_OK;
    }

    /* Calculate possible font sizes and text positions */
    rb->lcd_setfont(FONT_UI);
    rb->lcd_getstringsize("15", &w, &h);
    if ((w > (SPOTS_WIDTH-2)) || (h > (SPOTS_HEIGHT-2)))
        num_font = FONT_SYSFIXED;

#if LCD_WIDTH > LCD_HEIGHT
    rb->lcd_getstringsize("Moves", &w, &h);
    if (w > (LCD_WIDTH-IMAGE_WIDTH-1))
        moves_font = FONT_SYSFIXED;
    rb->lcd_setfont(moves_font);
    rb->lcd_getstringsize("Moves", &w, &h);
    moves_y = 10 + h;
#else
    rb->lcd_getstringsize("Moves: 999", &w, &h);
    if ((w > LCD_WIDTH) || (h > (LCD_HEIGHT-IMAGE_HEIGHT-1)))
        moves_font = FONT_SYSFIXED;
    rb->lcd_setfont(moves_font);
    rb->lcd_getstringsize("Moves: 999", &w, &h);
    moves_y = (IMAGE_HEIGHT+1+(LCD_HEIGHT-IMAGE_HEIGHT-1)/2) - h / 2;
#endif
    for (i=0; i<NUM_SPOTS; i++)
        spots[i]=(i+1);

#ifdef HAVE_LCD_COLOR
    rb->lcd_set_background(LCD_BLACK);
    rb->lcd_set_foreground(LCD_WHITE);
    rb->lcd_set_backdrop(NULL);
#elif LCD_DEPTH > 1
    rb->lcd_set_background(LCD_WHITE);
    rb->lcd_set_foreground(LCD_BLACK);
    rb->lcd_set_backdrop(NULL);
#endif

    draw_playfield();
    rb->sleep(HZ*2);

    return puzzle_loop();
}
