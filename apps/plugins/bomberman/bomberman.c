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
#include "lib/pluginlib_actions.h"
#include "lib/pluginlib_exit.h"

#include "lib/helper.h"
#include "lib/display_text.h"
#include "lib/highscore.h"
#include "lib/playback_control.h"

#include "game.h"
#include "draw.h"
#include "ai.h"

const struct button_mapping *plugin_contexts[] = {
    pla_main_ctx,
    #if defined(HAVE_REMOTE_LCD)
    pla_remote_ctx,
    #endif
};

#define NB_ACTION_CONTEXTS \
    (sizeof(plugin_contexts) / sizeof(struct button_mapping*))

/*
 * Files.
 */

#define SAVE_FILE  PLUGIN_GAMES_DIR "/bomberman.save"
#define SCORE_FILE PLUGIN_GAMES_DIR "/bomberman.score"

/*
 * Some globals.
 */

unsigned long tick = 0;
static struct game_t game;
static bool resume = false;
static bool resume_file = false;

#define NUM_SCORES 5
static struct highscore highscores[NUM_SCORES];

/*
 * Main code.
 */

void InitGame(struct game_t *game)
{
    int i, j;

    /*int DefaultMap[MAP_H][MAP_W] = {
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,0,0,1,1,1,0,1,0,1,0,1,0,1,0,0,2},
        {2,0,2,1,2,1,2,0,2,1,2,0,2,1,2,0,2},
        {2,0,0,1,1,1,1,1,0,1,0,1,0,1,0,0,2},
        {2,0,2,1,2,1,2,0,2,1,2,0,2,1,2,0,2},
        {2,0,0,1,1,1,1,1,0,1,0,1,0,1,0,0,2},
        {2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2},
        {2,0,0,1,1,1,1,1,0,1,0,1,0,1,0,0,2},
        {2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2}
        };*/
    /*int DefaultMap[MAP_H][MAP_W] = {
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,0,0,1,1,1,0,1,0,1,0,1,0,1,0,0,2},
        {2,0,2,1,1,1,1,1,1,1,1,1,1,1,2,0,2},
        {2,0,1,0,0,0,0,0,0,1,0,1,0,1,1,0,2},
        {2,0,1,1,2,1,2,0,0,1,2,0,2,1,1,0,2},
        {2,0,1,1,1,1,1,1,0,0,0,0,0,1,1,0,2},
        {2,0,1,0,0,0,0,0,2,1,2,0,2,0,1,0,2},
        {2,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,2},
        {2,0,2,1,1,1,1,1,1,1,1,1,1,1,2,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2}
        };*/
    /*int DefaultMap[MAP_H][MAP_W] = {
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,0,0,0,1,1,0,1,0,1,0,1,1,0,0,0,2},
        {2,0,2,0,1,1,1,1,1,1,1,1,1,0,2,0,2},
        {2,1,1,1,1,0,0,0,0,1,0,1,1,1,1,1,2},
        {2,0,1,1,2,1,2,0,0,1,2,0,2,1,1,0,2},
        {2,0,1,1,1,1,1,1,0,0,0,0,0,1,1,0,2},
        {2,0,1,0,0,0,0,0,2,1,2,0,2,0,1,0,2},
        {2,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,2},
        {2,0,2,0,1,1,1,1,1,1,1,1,1,0,2,0,2},
        {2,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2}
        };*/
    int DefaultMap[MAP_H][MAP_W] = {
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,2},
        {2,0,2,1,2,1,2,1,2,1,2,1,2,1,2,0,2},
        {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
        {2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2},
        {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
        {2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2},
        {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
        {2,0,2,1,2,1,2,1,2,1,2,1,2,1,2,0,2},
        {2,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2}
        };
    /*int DefaultMap[MAP_H][MAP_W] = {
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2}
        };*/
    /*int DefaultMap[MAP_H][MAP_W] = {
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2}
        };
        */

    for (i = 0; i < MAP_W; i++)
        for (j = 0; j < MAP_H; j++)
        {
            game->field.map[i][j] = DefaultMap[j][i];
            game->field.boxes[i][j].state = HUNKY;
            game->field.bonuses[i][j] = BONUS_NONE;
        }

    for (i = 0; i < MAX_BOMBS; i++)
        game->field.bombs[i].state = BOMB_NONE;

    game->nplayers = MAX_PLAYERS;

    for (i = 0; i < MAX_PLAYERS; i++)
        game->draw_order[i] = &game->players[i];

    /* Set radius of explosion for each bomb power. */
    game->bomb_rad[BOMB_PWR_SINGLE] = 1;
    game->bomb_rad[BOMB_PWR_DOUBLE] = 2;
    game->bomb_rad[BOMB_PWR_TRIPLE] = 4;
    game->bomb_rad[BOMB_PWR_QUAD]   = 6;
    game->bomb_rad[BOMB_PWR_KILLER] = MAX(MAP_W, MAP_H);

    /* Set player maximum move phase for each speed. */
    game->max_move_phase[0] = 5;
    game->max_move_phase[1] = 2;
    game->max_move_phase[2] = 1;

    /* Place bonuses. */
    int nboxes = 0, nbonuses;
    for (i = 0; i < MAP_W; i++)
        for (j = 0; j < MAP_H; j++)
            if (game->field.map[i][j] == SQUARE_BOX)
                nboxes++;
    /* Not all boxes consist a bonus. */
    nbonuses = nboxes / 2;
    while (nbonuses)
    {
        i = rb->rand() % MAP_W;
        j = rb->rand() % MAP_H;

        if (game->field.map[i][j] == SQUARE_BOX && game->field.bonuses[i][j] == BONUS_NONE)
        {
            /* Choose a random bonus for this box. */
            game->field.bonuses[i][j] = rb->rand() % (BONUS_NONE);
            nbonuses--;
        }
    }

    /* Let the game begin! */
    game->state = GAME_GAME;
}

void InitPlayer(struct player_t *player, int num, int x, int y, bool isAI)
{
    player->status.state = ALIVE;
    player->xpos = x;
    player->ypos = y;
    player->look = LOOK_DOWN;
    player->speed = 0;
    player->bombs_max = 1;
    player->bombs_placed = 0;
    player->power = BOMB_PWR_DOUBLE;
    player->isFullPower = false;

    player->rxpos = 0;
    player->rypos = 0;
    player->ismove = false;
    player->move_phase = 0;

    player->isAI = isAI;

    player->num = num;
}

static void bomberman_loadgame(void)
{
    int fd;

    resume = false;

    /* Open game file. */
    fd = rb->open(SAVE_FILE, O_RDONLY);
    if (fd < 0) return;

    /* Read in saved game. */
    if((rb->read(fd, &game, sizeof(struct game_t)) <= 0))
    {
        rb->splash(HZ/2, "Failed to load game");
    }
    else
    {
        resume = true;
    }

    rb->close(fd);
}

static void bomberman_savegame(void)
{
    int fd;

    /* Write out the game state to the save file. */
    fd = rb->open(SAVE_FILE, O_WRONLY|O_CREAT, 0666);
    if (fd < 0) return;

    if ((rb->write(fd, &game, sizeof(struct game_t)) <= 0))
    {
        rb->close(fd);
        rb->remove(SAVE_FILE);
        rb->splash(HZ/2, "Failed to save game");
        return;
    }

    rb->close(fd);
}

static int bomberman_help(void)
{
    static char *help_text[] = {
        "Bomberman", "", "Aim", "",
        "Destroy", "all", "the", "enemies", "by", "exploding", "bombs!"
    };
    static struct style_text formation[]={
        { 0, TEXT_CENTER|TEXT_UNDERLINE },
        { 2, C_RED },
        { 19, C_RED },
        { 37, C_RED },
        { 39, C_BLUE },
        { 46, C_RED },
        { 52, C_GREEN },
        { 59, C_ORANGE },
        { 67, C_GREEN },
        { 74, C_YELLOW },
        { 80, C_RED },
        LAST_STYLE_ITEM
    };

    rb->lcd_setfont(FONT_UI);
#ifdef HAVE_LCD_COLOR
    rb->lcd_set_background(LCD_BLACK);
    rb->lcd_set_foreground(LCD_WHITE);
#endif
    if (display_text(ARRAYLEN(help_text), help_text, formation, NULL, true))
        return 1;
    rb->lcd_setfont(FONT_SYSFIXED);

    return 0;
}

static int bomberman_menu_cb(int action, const struct menu_item_ex *this_item)
{
    int i = ((intptr_t)this_item);
    if (action == ACTION_REQUEST_MENUITEM && !resume && (i == 0 || i == 5))
        return ACTION_EXIT_MENUITEM;
    return action;
}

static int bomberman_menu(void)
{
    int selected = 0;

    MENUITEM_STRINGLIST(main_menu, "Bomberman Menu", bomberman_menu_cb,
                        "Resume Game", "Start New Game",
                        "Help", "High Scores",
                        "Playback Control",
                        "Quit without Saving", "Quit");

    rb->button_clear_queue();
    while (true) {
        switch (rb->do_menu(&main_menu, &selected, NULL, false)) {
            case 0: /* Resume Game */
                if (resume_file)
                    rb->remove(SAVE_FILE);
                return 0;
            case 1: /* Start New Game*/
                InitGame(&game);
                InitPlayer(&game.players[0], 0, 1, 1, false);
                InitPlayer(&game.players[1], 1, 1, MAP_H - 3, true);
                InitPlayer(&game.players[2], 2, MAP_W - 3, 1, true);
                InitPlayer(&game.players[3], 3, MAP_W - 3, MAP_H - 2, true);
                return 0;
            case 2: /* Help */
                if (bomberman_help())
                    return 1;
                break;
            case 3: /* High Scores */
                highscore_show(-1, highscores, NUM_SCORES, true);
                break;
            case 4: /* Playback Control */
                if (playback_control(NULL))
                    return 1;
                break;
            case 5: /* Quit without Saving */
                return 1;
            case 6: /* Quit */
                if (resume) {
                    rb->splash(HZ*1, "Saving game ...");
                    bomberman_savegame();
                }
                return 1;
            case MENU_ATTACHED_USB:
                return 1;
            default:
                break;
        }
    }
}

static int bomberman_game_loop(void)
{
    int i;

#ifdef HAVE_ADJUSTABLE_CPU_FREQ
    rb->cpu_boost(false);
#endif

    if (bomberman_menu())
        return 1;

    resume = false;
    resume_file = false;

#ifdef HAVE_ADJUSTABLE_CPU_FREQ
    rb->cpu_boost(true);
#endif

    /* Main loop. */
    while (true)
    {
        int end = get_tick() + HZ / CYCLETIME;

        Draw(&game);

        UpdateBombs(&game);
        UpdateBoxes(&game);
        UpdateAI(&game, game.players);

        int action = pluginlib_getaction(TIMEOUT_NOBLOCK, plugin_contexts, NB_ACTION_CONTEXTS);
        switch (action)
        {
            case PLA_CANCEL:
            case PLA_EXIT:
                resume = true;
                return 0;

            case PLA_UP:
            case PLA_UP_REPEAT:
                PlayerMoveUp(&game, &game.players[0]);
                break;

            case PLA_DOWN:
            case PLA_DOWN_REPEAT:
                PlayerMoveDown(&game, &game.players[0]);
                break;

            case PLA_RIGHT:
            case PLA_RIGHT_REPEAT:
                PlayerMoveRight(&game, &game.players[0]);
                break;

            case PLA_LEFT:
            case PLA_LEFT_REPEAT:
                PlayerMoveLeft(&game, &game.players[0]);
                break;

            case PLA_SELECT:
                PlayerPlaceBomb(&game, &game.players[0]);
                break;

            default:
                if (rb->default_event_handler(action) == SYS_USB_CONNECTED)
                    return 1;
                break;
        }

        if (game.state == GAME_GAME)
        {
            for (i = 0; i < MAX_PLAYERS; i++)
            {
                int upd = UpdatePlayer(&game, &game.players[i]);
                if (upd == DEAD)
                {
                    game.nplayers--;
                    if (game.nplayers == 1 || !game.players[i].isAI)
                    {
                        for (i = 0; i < MAX_PLAYERS; i++)
                        {
                            if (game.players[i].status.state == ALIVE) {
                                game.players[i].status.state = WIN_PHASE1;
                            }
                        }
                    }
                }
                else if (upd == -GAME_GAMEOVER || upd == -GAME_WON)
                {
                    game.state = -upd;
                    tick = 0;
                }
            }
        }

        if (game.state == GAME_GAMEOVER || game.state == GAME_WON)
            if (tick >= AFTERGAME_DUR)
                return 0;

        rb->yield();

        if (TIME_BEFORE(get_tick(), end))
            rb->sleep(end - get_tick());

        tick++;
    }
}

int main(void)
{
    highscore_load(SCORE_FILE, highscores, NUM_SCORES);

    bomberman_loadgame();
    resume_file = resume;

    while (!bomberman_game_loop())
    {
        if (!resume)
        {
            int position = highscore_update(game.score, game.level + 1, "",
                                            highscores, NUM_SCORES);
            if (position != -1)
            {
                if (position == 0)
                    rb->splash(HZ*2, "New High Score");
                highscore_show(position, highscores, NUM_SCORES, true);
            }
            else
            {
                //brickmania_sleep(3);
            }
        }
    }

    highscore_save(SCORE_FILE, highscores, NUM_SCORES);

    return PLUGIN_OK;
}

static void cleanup(void)
{
    /* This is handled by plugin api. Remove in future. */
    /* rb->lcd_setfont(FONT_UI); */

    /* Turn on backlight timeout (revert to settings) */
    backlight_use_settings(); /* Backlight control in lib/helper.c */
#ifdef HAVE_REMOTE_LCD
    remote_backlight_use_settings();
#endif
}

/* This is the plugin entry point. */
enum plugin_status plugin_start(const void* parameter)
{
    int ret;

    (void)parameter;
    atexit(cleanup);

    rb->lcd_setfont(FONT_SYSFIXED);
#if LCD_DEPTH > 1
    rb->lcd_set_backdrop(NULL);
#endif
    /* Turn off backlight timeout. */
    backlight_force_on(); /* Backlight control in lib/helper.c */
#ifdef HAVE_REMOTE_LCD
    remote_backlight_force_on(); /* Remote backlight control in lib/helper.c */
#endif

    rb->srand(get_tick());

    ret = main();

    return ret;
}
