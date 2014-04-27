/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2012 by Amaury Pouly
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
#ifndef _BUTTON_TARGET_H_
#define _BUTTON_TARGET_H_

#include <stdbool.h>
#include "config.h"

void button_init_device(void);
int button_read_device(void);
bool button_debug_screen(void);

#define HAS_BUTTON_HOLD

/* Main unit's buttons */
#define BUTTON_POWER                0x00000001
#define BUTTON_MENU                 0x00000002
#define BUTTON_BACK                 0x00000004
#define BUTTON_PLAY                 0x00000008
#define BUTTON_UP                   0x00000010
#define BUTTON_DOWN                 0x00000020
#define BUTTON_VOL_UP               0x00000040
#define BUTTON_VOL_DOWN             0x00000080
/* For compatibility */
#define BUTTON_LEFT  BUTTON_BACK
#define BUTTON_RIGHT BUTTON_MENU

#define BUTTON_MAIN 0xff

#define BUTTON_REMOTE 0

/* Software power-off */
#define POWEROFF_BUTTON BUTTON_POWER
#define POWEROFF_COUNT 10

#endif /* _BUTTON_TARGET_H_ */
