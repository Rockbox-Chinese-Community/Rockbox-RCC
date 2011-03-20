/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2010-2011 Lev Panov, Nick Petrov
 *
 * Bomberman plugin
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

/* Bitmaps. */
#include "pluginbitmaps/bomberman_player.h"
#include "pluginbitmaps/bomberman_box.h"
#include "pluginbitmaps/bomberman_block.h"
#include "pluginbitmaps/bomberman_bomb.h"
#include "pluginbitmaps/bomberman_explode.h"
#include "pluginbitmaps/bomberman_player_move.h"
#include "pluginbitmaps/bomberman_ai1_move.h"
#include "pluginbitmaps/bomberman_ai2_move.h"
#include "pluginbitmaps/bomberman_ai3_move.h"
#include "pluginbitmaps/bomberman_ai4_move.h"
#include "pluginbitmaps/bomberman_player_death.h"
#include "pluginbitmaps/bomberman_player_win.h"
#include "pluginbitmaps/bomberman_ai1_win.h"
#include "pluginbitmaps/bomberman_ai2_win.h"
#include "pluginbitmaps/bomberman_ai3_win.h"
#include "pluginbitmaps/bomberman_ai4_win.h"
#include "pluginbitmaps/bomberman_bonus.h"
#include "pluginbitmaps/bomberman_gameover.h"
#include "pluginbitmaps/bomberman_youwon.h"

#include "game.h"
#include "draw.h"

#define SQUARE_SIZE 16
#define XMAPOFFSET 25
#define YMAPOFFSET 30

/* Win bitmaps for each player number. */
static const fb_data *win_bitmaps[5] = {bomberman_player_win,
                                        bomberman_ai1_win,
                                        bomberman_ai2_win,
                                        bomberman_ai3_win,
                                        bomberman_ai4_win};
/* Move bitmaps for each player number. */
static const fb_data *move_bitmaps[5] = {bomberman_player_move,
                                         bomberman_ai1_move,
                                         bomberman_ai2_move,
                                         bomberman_ai3_move,
                                         bomberman_ai4_move};

/* Offsets (in pixels) for each position of player in one square. */
//int xcoord[3] = {1, 6, 12};
static const int xcoord[3] = {-4, 0, 4};
//int ycoord[3] = {3, 9, 14};
//int ycoord[3] = {1, 6, 12};
//int ycoord[3] = {12, 6, 1};
static const int ycoord[3] = {9, 3, -2};

/* Some magic constants for right player's movement drawing. */
static const int xcoord_k[4] = {0, 0, 1, -1};
static const int ycoord_k[4] = {-1, 1, 0, 0};

/* Drawing move phase for each actual move phase of player. */
static const int move_cur_phases[6] = {0, 1, 2, 3, 2, 0};

/* Binary offsets for each direction (used in explosion drawing) */
static const int dir_offsets[5] = {0, 4, 8, 12, 16};

/* Draw function. */
void Draw(struct game_t *game)
{
    int i, j;

    if (game->state == GAME_GAME)
	rb->lcd_clear_display();

    if (game->state == GAME_GAME)
    {
        /* Different objects on the field. */
	for (i = 0; i < MAP_W; i++)
            for (j = 0; j < MAP_H; j++)
                switch (game->field.map[i][j])
                {
                case SQUARE_FREE:
                    if (game->field.bonuses[i][j] != BONUS_NONE)
                    {
                        rb->lcd_bitmap_transparent_part(bomberman_bonus,
                            0,
                            game->field.bonuses[i][j] * SQUARE_SIZE,
                            STRIDE(SCREEN_MAIN,
                                   BMPWIDTH_bomberman_bonus,
                                   BMPHEIGHT_bomberman_bonus),
                            i * SQUARE_SIZE + XMAPOFFSET,
                            j * SQUARE_SIZE + YMAPOFFSET,
                            SQUARE_SIZE,
                            SQUARE_SIZE);
                    }
                    break;
                case SQUARE_BOX:
                    /*
                    rb->lcd_bitmap_transparent(bomberman_box,
                                               i * SQUARE_SIZE + XMAPOFFSET,
                                               j * SQUARE_SIZE + YMAPOFFSET,
                                               BMPWIDTH_bomberman_box,
                                               BMPHEIGHT_bomberman_box);
                    */
                    rb->lcd_bitmap_transparent_part(bomberman_box,
                        game->field.boxes[i][j].state * SQUARE_SIZE,
                        0,
                        STRIDE(SCREEN_MAIN,
                               BMPWIDTH_bomberman_box,
                               BMPHEIGHT_bomberman_box),
                        i * SQUARE_SIZE + XMAPOFFSET,
                        j * SQUARE_SIZE + YMAPOFFSET,
                        SQUARE_SIZE,
                        SQUARE_SIZE);
                    break;
                case SQUARE_BLOCK:
                    rb->lcd_bitmap_transparent(bomberman_block,
                       i * SQUARE_SIZE + XMAPOFFSET,
                       j * SQUARE_SIZE + YMAPOFFSET,
                       BMPWIDTH_bomberman_block,
                       BMPHEIGHT_bomberman_block);
                    break;
                case SQUARE_BOMB:
                     /*
                     rb->lcd_bitmap_transparent(bomberman_bomb,
                                                i * SQUARE_SIZE + XMAPOFFSET,
                                                j * SQUARE_SIZE + YMAPOFFSET,
                                                BMPWIDTH_bomberman_bomb,
                                                BMPHEIGHT_bomberman_bomb);
                     */
                    rb->lcd_bitmap_transparent_part(bomberman_bomb,
                        game->field.det[i][j] * SQUARE_SIZE,
                        0,
                        STRIDE(SCREEN_MAIN,
                               BMPWIDTH_bomberman_bomb,
                               BMPHEIGHT_bomberman_bomb),
                        i * SQUARE_SIZE + XMAPOFFSET,
                        j * SQUARE_SIZE + YMAPOFFSET,
                        SQUARE_SIZE,
                        SQUARE_SIZE);
                    break;
                }

        /* Player without movement. */
        /*
        rb->lcd_bitmap_transparent(bomberman_player,
            game->players[i].xpos * SQUARE_SIZE + XMAPOFFSET,
            game->players[i].ypos * SQUARE_SIZE + YMAPOFFSET -
            (BMPHEIGHT_bomberman_player - SQUARE_SIZE),
            BMPWIDTH_bomberman_player,
            BMPHEIGHT_bomberman_player);
        */

	/* Player and ai's (with movement animation) */
	for (i = 0; i < MAX_PLAYERS; i++)
	{
            if (game->draw_order[i]->status.state > GONNA_DIE)
            {
                if (game->draw_order[i]->status.state < DEAD)
                {
                    rb->lcd_bitmap_transparent_part(bomberman_player_death,
                        (game->draw_order[i]->status.state - 2) * BMPWIDTH_bomberman_player,
                        0,
                        STRIDE(SCREEN_MAIN,
                               BMPWIDTH_bomberman_player_death,
                               BMPHEIGHT_bomberman_player_death),
                        game->draw_order[i]->xpos * SQUARE_SIZE + XMAPOFFSET +
                                        xcoord[game->draw_order[i]->rxpos + 1],
                        game->draw_order[i]->ypos * SQUARE_SIZE + YMAPOFFSET -
                        (BMPHEIGHT_bomberman_player - SQUARE_SIZE) -
                                        ycoord[game->draw_order[i]->rypos + 1],
                        BMPWIDTH_bomberman_player,
                        BMPHEIGHT_bomberman_player);
                }
                else if (game->draw_order[i]->status.state > DEAD)
                {
                    rb->lcd_bitmap_transparent_part(win_bitmaps[game->draw_order[i]->num],
                        (game->draw_order[i]->status.state % 2) * BMPWIDTH_bomberman_player,
                        0,
                        STRIDE(SCREEN_MAIN,
                               BMPWIDTH_bomberman_player_win,
                               BMPHEIGHT_bomberman_player_win),
                        game->draw_order[i]->xpos * SQUARE_SIZE + XMAPOFFSET +
                                        xcoord[game->draw_order[i]->rxpos + 1],
                        game->draw_order[i]->ypos * SQUARE_SIZE + YMAPOFFSET -
                                        (BMPHEIGHT_bomberman_player - SQUARE_SIZE) -
                                        ycoord[game->draw_order[i]->rypos + 1],
                        BMPWIDTH_bomberman_player,
                        BMPHEIGHT_bomberman_player);
                }
            }
            else
            {
                if (game->draw_order[i]->ismove)
                {
                    int curphase = move_cur_phases[game->draw_order[i]->move_phase];

                    rb->lcd_bitmap_transparent_part(move_bitmaps[game->draw_order[i]->num],
                        curphase * BMPWIDTH_bomberman_player,
                        game->draw_order[i]->look * BMPHEIGHT_bomberman_player,
                        STRIDE(SCREEN_MAIN, BMPWIDTH_bomberman_player_move, BMPHEIGHT_bomberman_player_move),
                        game->draw_order[i]->xpos * SQUARE_SIZE + XMAPOFFSET +
                            xcoord[game->draw_order[i]->rxpos + 1] +
                            xcoord_k[game->draw_order[i]->look] * game->draw_order[i]->move_phase,
                        game->draw_order[i]->ypos * SQUARE_SIZE + YMAPOFFSET -
                            (BMPHEIGHT_bomberman_player - SQUARE_SIZE) -
                            ycoord[game->draw_order[i]->rypos + 1] +
                            ycoord_k[game->draw_order[i]->look] * game->draw_order[i]->move_phase,
                        BMPWIDTH_bomberman_player,
                        BMPHEIGHT_bomberman_player);
                }
                else
                {
                    rb->lcd_bitmap_transparent_part(move_bitmaps[game->draw_order[i]->num],
                        0,
                        game->draw_order[i]->look * BMPHEIGHT_bomberman_player,
                        STRIDE(SCREEN_MAIN, BMPWIDTH_bomberman_player_move, BMPHEIGHT_bomberman_player_move),
                        game->draw_order[i]->xpos * SQUARE_SIZE + XMAPOFFSET +
                        xcoord[game->draw_order[i]->rxpos + 1],
                        game->draw_order[i]->ypos * SQUARE_SIZE + YMAPOFFSET -
                        (BMPHEIGHT_bomberman_player - SQUARE_SIZE) -
                        ycoord[game->draw_order[i]->rypos + 1],
                        BMPWIDTH_bomberman_player,
                        BMPHEIGHT_bomberman_player);
                }
            }
	}

        /* Explosions :) */
        for (i = 0; i < MAP_W; i++)
            for (j = 0; j < MAP_H; j++)
            {
                int dir;

                for (dir = 0; dir < 5; dir++)
                {
                    bool is_set = game->field.firemap[i][j] & (BITMASK_ALL_PHASES << dir_offsets[dir]);

                    if (is_set)
                    {
                        bool is_end = game->field.firemap[i][j] & (BITMASK_IS_END << dir);
                        int phase;

                        for (phase = 0; phase < 4; phase++)
                        {
                            if (game->field.firemap[i][j] &
                                    ((BITMASK_SING_PHASE << dir_offsets[dir]) << phase))
                                break;
                        }

                        if (dir == FIRE_CENTER)
                        {
                            rb->lcd_bitmap_transparent_part(bomberman_explode,
                                phase * SQUARE_SIZE,
                                0,
                                STRIDE(SCREEN_MAIN,
                                       BMPWIDTH_bomberman_explode,
                                       BMPHEIGHT_bomberman_explode),
                                i * SQUARE_SIZE + XMAPOFFSET,
                                j * SQUARE_SIZE + YMAPOFFSET,
                                SQUARE_SIZE,
                                SQUARE_SIZE);
                        }
                        else
                        {
                            rb->lcd_bitmap_transparent_part(bomberman_explode,
                                dir * SQUARE_SIZE,
                                SQUARE_SIZE + SQUARE_SIZE * phase * 2 +
                                SQUARE_SIZE * is_end,
                                STRIDE(SCREEN_MAIN,
                                       BMPWIDTH_bomberman_explode,
                                       BMPHEIGHT_bomberman_explode),
                                i * SQUARE_SIZE + XMAPOFFSET,
                                j * SQUARE_SIZE + YMAPOFFSET,
                                SQUARE_SIZE,
                                SQUARE_SIZE);
                        }
                    }
                }
            }
    }
    else if (game->state == GAME_GAMEOVER)
    {
        rb->lcd_bitmap(bomberman_gameover, 0, 0,
                       BMPWIDTH_bomberman_gameover, BMPHEIGHT_bomberman_gameover);
    }
    else if (game->state == GAME_WON)
    {
        rb->lcd_bitmap(bomberman_youwon, 0, 0,
                       BMPWIDTH_bomberman_youwon, BMPHEIGHT_bomberman_youwon);
    }

    /* Update the lcd. */
    rb->lcd_update();
}
