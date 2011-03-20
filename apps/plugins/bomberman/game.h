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

#ifndef _GAME_H
#define _GAME_H

/*
 * Global game params.
 */

#define MAP_W 17
#define MAP_H 11
#define MAX_PLAYERS 4
#define MAX_BOMBS 15

/*
 * Animation params.
 */

#define CYCLETIME 30

#define BOMB_DELAY_DET    (HZ * 4    / (CYCLETIME / 10)) /* Delay before bomb detonates. */
#define BOMB_DELAY_PHASE1 (HZ * 4.02 / (CYCLETIME / 10))
#define BOMB_DELAY_PHASE2 (HZ * 4.03 / (CYCLETIME / 10))
#define BOMB_DELAY_PHASE3 (HZ * 4.05 / (CYCLETIME / 10))
#define BOMB_DELAY_PHASE4 (HZ * 4.06 / (CYCLETIME / 10))

#define BOX_DELAY_EXPLOSION_ANIM (HZ * 0.04 / (CYCLETIME / 10))

#define PLAYER_DELAY_DEATH_ANIM (HZ * 0.09 / (CYCLETIME / 10))
#define PLAYER_DELAY_WIN_ANIM (HZ * 0.1 / (CYCLETIME / 10))
#define PLAYER_DELAY_WIN_ANIM_DUR (HZ * 5 / (CYCLETIME / 10))

#define AFTERGAME_DUR (HZ * 3 / (CYCLETIME / 10))

/*
 * Two types of ticks.
 */

#define get_tick() (*rb->current_tick)
extern unsigned long tick;

/*
 * Enums and structs.
 */

enum square_type {
	SQUARE_FREE = 0,
	SQUARE_BOX,
	SQUARE_BLOCK,
	SQUARE_BOMB
};

enum look_side {
	LOOK_UP = 0,
	LOOK_DOWN,
	LOOK_RIGHT,
	LOOK_LEFT
};

enum bomb_power {
	BOMB_PWR_SINGLE = 0,
	BOMB_PWR_DOUBLE,
	BOMB_PWR_TRIPLE,
	BOMB_PWR_QUAD,
	BOMB_PWR_KILLER
};

enum player_state {
	ALIVE = 0,
	GONNA_DIE,
	EXPL_PHASE1,
	EXPL_PHASE2,
	EXPL_PHASE3,
	EXPL_PHASE4,
	DEAD,
	WIN_PHASE1,
	WIN_PHASE2
};

struct player_status {
        enum player_state state;
	unsigned long time_of_death;
};

struct player_t {
        struct player_status status;
	
	int xpos, ypos;
        enum look_side look;
	int speed;
	
	int bombs_max;
	int bombs_placed;
        enum bomb_power power;
        bool isFullPower;
	
	int rxpos, rypos;
	bool ismove;
	int move_phase;
	unsigned long move_start_time;
	
        bool isAI;
	
	int num;
};

/*
 * rxpos & rypos - position of player in single cell
 * 
 __________________
 | -1  |  0  |  1  |
 | -1  | -1  | -1  |
 |_____|_____|_____|
 | -1  |  0  |  1  |
 |  0  |  0  |  0  |   <--- one cell
 |_____|_____|_____|
 | -1  |  0  |  1  |
 |  1  |  1  |  1  |
 |_____|_____|_____|
 */

/*
 * Binary masks for fire map stuff.
 */

#define BITMASK_ALL_DIRS    0x000FFFFF
#define BITMASK_ALL_PHASES  0x0000000F
#define BITMASK_SING_PHASE  0x00000001

#define BITMASK_RIGHT       0x00000001
#define BITMASK_DOWN        0x00000010
#define BITMASK_LEFT        0x00000100
#define BITMASK_UP          0x00001000
#define BITMASK_CENTER      0x00010000

#define BITMASK_IS_END      0x00100000

enum bomb_state {
        BOMB_NONE = 0,
        BOMB_PLACED,
        BOMB_EXPL_PHASE1,
        BOMB_EXPL_PHASE2,
        BOMB_EXPL_PHASE3,
        BOMB_EXPL_PHASE4
};

struct bomb_t {
        enum bomb_state state;
	int xpos, ypos;
        enum bomb_power power;
	unsigned long place_time;
        struct player_t *owner;
};

enum fire_dir {
	FIRE_RIGNT = 0,
	FIRE_DOWN,
	FIRE_LEFT,
	FIRE_UP,
	FIRE_CENTER
};

enum bomb_detonation {
	DET_PHASE1 = 0,
	DET_PHASE2,
	DET_PHASE3
};

enum box_state {
	HUNKY = 0,
	BOX_EXPL_PHASE1,
	BOX_EXPL_PHASE2,
	BOX_EXPL_PHASE3,
	BOX_EXPL_PHASE4,
	BOX_EXPL_PHASE5
};

struct box_detonation {
        enum box_state state;
	unsigned long expl_time;
};

enum bonus_type {
        BONUS_EXTRABOMB, /* Extra bomb. */
        BONUS_POWER,     /* Increase bomb explosion radius. */
        BONUS_SPEEDUP,   /* Increase player speed. */
        BONUS_FULLPOWER, /* Destroy all destroyable objects in radius. */
        BONUS_NONE       /* No bonus. */
};

struct field_t {
        enum square_type map[MAP_W][MAP_H];
        struct bomb_t bombs[MAX_BOMBS];
        volatile uint32_t firemap[MAP_W][MAP_H];
        enum bomb_detonation det[MAP_W][MAP_H];
        struct box_detonation boxes[MAP_W][MAP_H];
        enum bonus_type bonuses[MAP_W][MAP_H];
};

enum game_state {
	GAME_GAME = 0,
	GAME_GAMEOVER,
	GAME_WON
};

struct game_t {
        enum game_state state;
        struct field_t field;
        struct player_t players[MAX_PLAYERS];
        struct player_t *draw_order[MAX_PLAYERS];
	int nplayers;
	int bomb_rad[5];
        int max_move_phase[3];
        int score;
        int level;
};

/*
 * Player control functions.
 */

void PlayerMoveUp(struct game_t *game, struct player_t *player);
void PlayerMoveDown(struct game_t *game, struct player_t *player);
void PlayerMoveRight(struct game_t *game, struct player_t *player);
void PlayerMoveLeft(struct game_t *game, struct player_t *player);
void PlayerPlaceBomb(struct game_t *game, struct player_t *player);

/*
 * Game update functions.
 */

int  UpdatePlayer(struct game_t *game, struct player_t *player);
void UpdateBombs(struct game_t *game);
void UpdateBoxes(struct game_t *game);
void UpdateAftergame(struct game_t *game);

#endif /* _GAME_H */
