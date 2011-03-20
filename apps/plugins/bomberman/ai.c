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

#include "game.h"

#define USE_PATH_CACHE 0
#if USE_PATH_CACHE
#define PATH_CACHE_UPD_TIME 50
#endif /* USE_PATH_CACHE */

#define MAX_PATH_LEN 50
#define UNREAL_F 999
#define PATH_OFFSET 2
#define MOVE_COST 10

struct node_t {
    bool IsWalkable;
    bool IsOnOpen;
    bool IsOnClose;
    int G, H, F;
    int ParentX, ParentY;
};

struct path_elem {
    int X, Y;
};

struct path_t {
    struct path_elem Path[MAP_W * MAP_H];
    int Distance;
};

struct ai_vars {
    int ClosestPlayer;
    bool Danger;
    struct path_elem SafetyPlace;
    int Bombs;

#if USE_PATH_CACHE
    struct path_t PathCache;
    unsigned int PathCacheUpdTime;
#endif /* USE_PATH_CACHE */
};

static struct node_t Nodes[MAP_W][MAP_H];
static struct ai_vars AI[MAX_PLAYERS];

inline static bool GetNode(struct field_t *field, int x, int y, bool UseBoxes)
{
    return (field->map[x][y] == SQUARE_FREE || (UseBoxes && field->map[x][y] == SQUARE_BOX));
}

static void InitNodes(struct field_t *F, bool UseBoxes)
{
    int x, y;

    for (x = 0; x < MAP_W; x++)
        for (y = 0; y < MAP_H; y++)
        {
	    Nodes[x][y].IsWalkable = GetNode(F, x, y, UseBoxes);
	    Nodes[x][y].IsOnOpen = false;
	    Nodes[x][y].IsOnClose = false;
	    Nodes[x][y].G = 0;
	    Nodes[x][y].H = 0;
	    Nodes[x][y].F = 0;
	    Nodes[x][y].ParentX = 0;
	    Nodes[x][y].ParentY = 0;
	}
}

static int FindPath(struct game_t *G, struct path_t *Path, int StartX, int StartY,
                                         int EndX, int EndY, bool UseBoxes)
{
    int x = 0, y = 0; /* for running through the nodes */
    int dx, dy; /* for the 8 (4) squares adjacent to each node */
    int cx = StartX, cy = StartY;
    int lowestf = UNREAL_F; /* start with the lowest being the highest */
    int count = 0;

#ifdef __DEBUG
     int desc;
     char logStr[100] = "\n";

     if ((desc = rb->open(PLUGIN_GAMES_DIR "/path.txt", O_WRONLY | O_APPEND | O_CREAT, 0666)) < 0)
     {
       rb->splash(HZ, "Cant open");
       return;
     }
     rb->write(desc, logStr, rb->strlen(logStr));
     rb->memset(logStr, 0, 100);
     rb->snprintf(logStr, 5, "%i %i\n", n, m);
     rb->write(desc, logStr, rb->strlen(logStr));
#endif /* #ifdef __DEBUG */

    InitNodes(&G->field, UseBoxes);
    /* add starting node to open list */
    Nodes[StartX][StartY].IsOnOpen = true;
    Nodes[StartX][StartY].IsOnClose = false;

    while (cx != EndX || cy != EndY)
    {
	count++;
	
        if (count > MAX_PATH_LEN)
	{
#ifdef __DEBUG
            rb->memset(logStr, 0, 100);
            rb->snprintf(logStr, 8, "%s\n", "Return");
            rb->write(desc, logStr, rb->strlen(logStr));
#endif /* #ifdef __DEBUG */

            return 0;
        }

        /* look for lowest F cost node on open list - this becomes the current node */
	lowestf = UNREAL_F;
	for (x = 0; x < MAP_W; x++)
	{
            for (y = 0; y < MAP_H; y++)
            {
		Nodes[x][y].F = Nodes[x][y].G + Nodes[x][y].H;
		if (Nodes[x][y].IsOnOpen)
		{
                    if (Nodes[x][y].F < lowestf)
                    {
                        cx = x;
                        cy = y;
                        lowestf = Nodes[x][y].F;
                    }
		}
            }
	}
	
	/* we found it, so now put that node on the closed list */
	Nodes[cx][cy].IsOnOpen = false;
	Nodes[cx][cy].IsOnClose = true;
	
	/* for each of the 8 (4) adjacent node */
	for (dx = -1; dx <= 1; dx++)
	{
            for (dy = -1; dy <= 1; dy++)
            {
		/* we don't want to walk by diagonals in bomberman */
		if (dx != -dy && dx != dy)
		{
                    if ((cx + dx) < MAP_W && (cx + dx) > -1 &&
                        (cy + dy) < MAP_H && (cy + dy) > -1)
                    {
                        /* if its walkable and not on the closed list */
                        if (Nodes[cx + dx][cy + dy].IsWalkable == true
				&& Nodes[cx + dx][cy + dy].IsOnClose == false)
                        {
			    /* if its not on open list */
                            if (Nodes[cx + dx][cy + dy].IsOnOpen == false)
                            {
                                /* add it to open list */
                                Nodes[cx + dx][cy + dy].IsOnOpen = true;
                                Nodes[cx + dx][cy + dy].IsOnClose = false;
                                /* make the current node its parent */
                                Nodes[cx + dx][cy + dy].ParentX = cx;
                                Nodes[cx + dx][cy + dy].ParentY = cy;

#ifdef __DEBUG
                                rb->memset(logStr, 0, 100);
                                rb->snprintf(logStr, 21, "%i C: %i %i P: %i %i\n",
                                             n, cx + dx, cy + dy, cx, cy);
                                rb->write(desc, logStr, rb->strlen(logStr));
#endif /* #ifdef __DEBUG */

                                /* work out G */
                                Nodes[cx + dx][cy + dy].G = MOVE_COST; /* straights cost 10 */
                                /* work out H */
                                /* MANHATTAN METHOD */
                                Nodes[cx + dx][cy + dy].H =
                                        (abs(EndX - (cx + dx)) +
                                         abs(EndY - (cy + dy))) * MOVE_COST;
                                Nodes[cx + dx][cy + dy].F =
                                        Nodes[cx + dx][cy + dy].G +
                                        Nodes[cx + dx][cy + dy].H;
                            }
			} /* end if walkable and not on closed list */
		    }
		}
            }
	}
    }

    /* follow all the parents back to the start */
    cx = EndX;
    cy = EndY;
    Path->Distance = 0;
    Path->Path[Path->Distance].X = cx;
    Path->Path[Path->Distance].Y = cy;
    Path->Distance++;

    while (cx != StartX || cy != StartY)
    {
	Path->Path[Path->Distance].X = Nodes[cx][cy].ParentX;
	Path->Path[Path->Distance].Y = Nodes[cx][cy].ParentY;
	cx = Path->Path[Path->Distance].X;
	cy = Path->Path[Path->Distance].Y;
	
#ifdef __DEBUG
        rb->memset(logStr, 0, 100);
        rb->snprintf(logStr, 12 , "%i PP: %i %i\n", n, Path->Path[Path->Distance].X,
                      Path->Path[Path->Distance].Y);
        rb->write(desc, logStr, rb->strlen(logStr));
#endif /* #ifdef __DEBUG */
	
	Path->Distance++;
        if (Path->Distance > MAX_PATH_LEN)
            return 0;
    }

#ifdef __DEBUG
    rb->close(desc);
#endif /* #ifdef __DEBUG */

    return 1;
}

#ifdef __DEBUG
void LogPath(struct path_t *P)
{
    int file;
    int i;
    char logStr[100] = "\n";

    if ((file = rb->open(PLUGIN_GAMES_DIR "/safe_path.txt", 
                         O_WRONLY|O_CREAT|O_APPEND, 0666)) < 0)
    {
	return;
    }
    
    rb->write(file, logStr, 100);
    
    for (i = 0; i < P->Distance; i++)
    {
	rb->memset(logStr, 0, 100);
	rb->snprintf(logStr, 7, "%i %i\n", P->Path[i].X, P->Path[i].Y);
	rb->write(file, logStr, 100);
    }

    rb->close(file);
}
#endif /* #ifdef __DEBUG */

static int FoundDangerBombs(struct game_t *G, int x, int y)
{
    int i, j;
    int ProtectWalls;
    bool Checked, BombPlaced;

    if (x < 0 || y < 0 || x >= MAP_W || y >= MAP_H)
        return -1;

    for (i = 0; i < MAX_BOMBS; i++)
    {
        ProtectWalls = 0;
        Checked = false;
        BombPlaced = false;
        if (G->field.bombs[i].state >= BOMB_PLACED)
        {
            BombPlaced = true;

            if (G->field.bombs[i].ypos == y)
            {
                Checked = true;

                if (abs(G->field.bombs[i].xpos - x) > G->bomb_rad[G->field.bombs[i].power])
                    ProtectWalls++;
                else if (G->field.bombs[i].xpos < x)
                {
                    for (j = G->field.bombs[i].xpos; j < x; j++)
                    {
                        if (G->field.map[j][y] == SQUARE_BLOCK ||
                            G->field.map[j][y] == SQUARE_BOX)
                        {
                            ProtectWalls++;
                            break;
                        }
                    }
                }
                else if (G->field.bombs[i].xpos > x)
                {
                    for (j = x; j < G->field.bombs[i].xpos; j++)
                    {
                        if (G->field.map[j][y] == SQUARE_BLOCK ||
                            G->field.map[j][y] == SQUARE_BOX)
                        {
                            ProtectWalls++;
                            break;
                        }
                    }
                }
            }

            /* else ? */
            if (G->field.bombs[i].xpos == x)
            {
                Checked = true;

                if (abs(G->field.bombs[i].ypos - y) > G->bomb_rad[G->field.bombs[i].power])
                    ProtectWalls++;
                else if (G->field.bombs[i].ypos < y)
                {
                    for (j = G->field.bombs[i].ypos; j < y; j++)
                    {
                        if (G->field.map[x][j] == SQUARE_BLOCK ||
                            G->field.map[x][j] == SQUARE_BOX)
                        {
                            ProtectWalls++;
                            break;
                        }
                    }
                }
                else if (G->field.bombs[i].ypos > y)
                {
                    for (j = y; j < G->field.bombs[i].ypos; j++)
                    {
                        if (G->field.map[x][j] == SQUARE_BLOCK ||
                            G->field.map[x][j] == SQUARE_BOX)
                        {
                            ProtectWalls++;
                            break;
                        }
                    }
                }
            }

            if (ProtectWalls >= 1 || (!Checked && BombPlaced))
                continue;

            return 1;
        }
    }

    return 0;
}

inline static bool IsPlayerNearPlayer(struct game_t *G, struct player_t *P1, struct player_t *P2)
{
    if (P1->xpos == P2->xpos)
    {
        if (abs(P1->ypos - P2->ypos) <= G->bomb_rad[P1->power])
            return true;
    }
    else if (P1->ypos == P2->ypos)
    {
        if (abs(P1->xpos - P2->xpos) <= G->bomb_rad[P1->power])
            return true;
    }

    return false;
}

static bool FindSafetyPlace(struct game_t *G, struct ai_vars *P,  struct path_t *Path, struct player_t *Pl)
{
    int dx, dy;
    int MinDist = UNREAL_F;
    struct path_elem Tmp;
    int x = Pl->xpos, y = Pl->ypos;

    Tmp.X = 0;
    Tmp.Y = 0;
    for (dx = -MAP_W; dx <= MAP_W; dx++)
        for (dy = -MAP_H; dy <= MAP_H; dy++)
        {
            if ((x + dx) < MAP_W && (x + dx) > -1 &&
                (y + dy) < MAP_H && (y + dy) > -1)
            {
                if (G->field.map[x + dx][y + dy] == SQUARE_FREE)
                {
                    if (FoundDangerBombs(G, x + dx, y + dy) == 0)
                    {
                        if (FindPath(G, Path, x, y, x + dx, y + dy, false))
                        {
                            if (Path->Distance < MinDist)
                            {
                                MinDist = Path->Distance;
                                Tmp.X = x + dx;
                                Tmp.Y = y + dy;
                            }
                        }
                    }
                }
            }
        }

    if (MinDist < UNREAL_F)
    {
        P->SafetyPlace.X = Tmp.X;
        P->SafetyPlace.Y = Tmp.Y;
        return true;
    }

    return false;
}

inline static bool IsABox(struct game_t *G, struct path_elem *P)
{
    return (G->field.map[P->X][P->Y] == SQUARE_BOX);
}

inline static void MovePlayer(struct game_t *G, struct player_t *P, struct path_t *Path)
{
    if (Path->Distance > 1)
    {
        int index = Path->Distance - PATH_OFFSET; /* index is positive */
        if (P->xpos < Path->Path[index].X)
	    PlayerMoveRight(G, P);
        else if (P->xpos > Path->Path[index].X)
	    PlayerMoveLeft(G, P);
        else if (P->ypos < Path->Path[index].Y)
	    PlayerMoveDown(G, P);
        else if (P->ypos > Path->Path[index].Y)
	    PlayerMoveUp(G, P);  
    }
}

inline static int CheckFire(struct game_t *G, int x, int y)
{
    return G->field.firemap[x][y] & BITMASK_ALL_DIRS;
}

void UpdateAI(struct game_t *G, struct player_t *Players)
{
    int i, j;
    bool isDanger;
    struct path_t Path, PathToClosestPlayer;
    int MinDist = UNREAL_F;

    for (i = 0; i < MAX_PLAYERS; i++)
    {
        if (Players[i].isAI && !Players[i].ismove && Players[i].status.state == ALIVE)
        {
#if USE_PATH_CACHE
            if (get_tick() - AI[i].PathCacheUpdTime < PATH_CACHE_UPD_TIME)
            {
                MovePlayer(G, &Players[i], &AI[i].PathCache);
                if (AI[i].PathCache.Distance > PATH_OFFSET)
                    AI[i].PathCache.Distance--;
                //rb->splash(HZ/20, "USE CACHE!");
                return;
            }
#endif /* USE_PATH_CACHE */

            isDanger = false;
            MinDist = UNREAL_F;

            if (FoundDangerBombs(G, Players[i].xpos, Players[i].ypos) == 1)
                isDanger = true;

            if (!isDanger)
            {
                AI[i].Danger = false;
                for (j = 0; j < MAX_PLAYERS; j++)
                {
                    if (j == i || Players[j].status.state > ALIVE)
                        continue;

                    FindPath(G, &Path, Players[i].xpos, Players[i].ypos,
                                       Players[j].xpos, Players[j].ypos, true);

                    if (Path.Distance < MinDist)
                    {
                        MinDist = Path.Distance;
                        memcpy(&PathToClosestPlayer, &Path, sizeof(struct path_t));
                        AI[i].ClosestPlayer = j;
                    }
                }
            }
            else /* Danger */
            {
                if (FindSafetyPlace(G, &AI[i], &Path, &Players[i]))
                    AI[i].Danger = true;
                else
                  continue;
            }

            if (AI[i].Danger)
            {
                if (FoundDangerBombs(G, Players[i].xpos, Players[i].ypos) == 1)
                    FindSafetyPlace(G, &AI[i], &Path, &Players[i]);

                if (FindPath(G, &Path, Players[i].xpos, Players[i].ypos,
                             AI[i].SafetyPlace.X, AI[i].SafetyPlace.Y, false))
                {
                    MovePlayer(G, &Players[i], &Path);

#if USE_PATH_CACHE
                    /* create path cache */
                    rb->memcpy(&AI[i].PathCache, &Path, sizeof(struct path_t));
                    if (AI[i].PathCache.Distance > PATH_OFFSET)
                        AI[i].PathCache.Distance--;
                    AI[i].PathCacheUpdTime = get_tick();
#endif /* USE_PATH_CACHE */
                }
            }
            else if (IsPlayerNearPlayer(G, &Players[i], &Players[AI[i].ClosestPlayer]))
            {
                PlayerPlaceBomb(G, &Players[i]);
            }
            else
            {
                int index = Path.Distance - PATH_OFFSET;
                if (index < 0) index = 0;
                else if (index >= MAP_W * MAP_H) index = 0;
                if (!CheckFire(G, Path.Path[index].X, Path.Path[index].Y) &&
                        !Players[i].bombs_placed)
                {
                    if (IsABox(G, &Path.Path[index]) && AI[i].Danger == false)
                    {
                        PlayerPlaceBomb(G, &Players[i]);
                    }
                    else
                    {
                        MovePlayer(G, &Players[i], &PathToClosestPlayer);

#if USE_PATH_CACHE
                        /* create path cache */
                        rb->memcpy(&AI[i].PathCache, &PathToClosestPlayer, sizeof(struct path_t));
                        if (AI[i].PathCache.Distance > PATH_OFFSET)
                            AI[i].PathCache.Distance--;
                        AI[i].PathCacheUpdTime = get_tick();
#endif /* USE_PATH_CACHE */
                    }
                }
            }
        }
    }
}
