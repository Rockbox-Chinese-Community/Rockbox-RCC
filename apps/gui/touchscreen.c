/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id: touchscreen.c $
 *
 * Copyright (C) 2009 by Carsten Schreiter
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
#include "viewport.h"
#include "touchscreen.h"


/* draws a button */
void draw_button(struct screen *screen, struct button *button)
{
    int txt_w, txt_h;

    screen->set_viewport(button->vp);
    
    /* draw button */
    if (button->clicked){
        screen->fillrect(button->x, button->y, button->width, button->height);
    }
    else{
        screen->drawrect(button->x, button->y, button->width, button->height);
    }

    /* clear corner pixels of button */
    screen->set_drawmode(DRMODE_SOLID | DRMODE_INVERSEVID);
    screen->drawpixel(button->x, button->y);
    screen->drawpixel(button->x + button->width - 1, button->y);
    screen->drawpixel(button->x, button->y + button->height - 1);
    screen->drawpixel(button->x + button->width -1, button->y + button->height -1);
    screen->set_drawmode(DRMODE_SOLID);

    /* draw button text */
    screen->getstringsize(button->txt, &txt_w, &txt_h);
    if (button->clicked){
        screen->set_drawmode(DRMODE_SOLID | DRMODE_INVERSEVID);
    }
    else{
        screen->set_drawmode(DRMODE_SOLID);
    }    
    screen->putsxy(button->x + (button->width - txt_w)/2 ,
                        button->y + (button->height - txt_h)/2, button->txt);
    screen->set_drawmode(DRMODE_SOLID);
    button->clicked = false;
    return;
}

/* checks if a button has been clicked */
bool check_button_click(struct button *button, short touch_x, short touch_y)
{
    if (viewport_point_within_vp(button->vp, (int)touch_x, (int)touch_y))
    {   /* convert touch into VP coordinates */
        touch_x -= button->vp->x;
        touch_y -= button->vp->y;
    
        if (touch_y >= button->y &&
            touch_y <= button->y + button->height &&
            touch_x >= button->x &&
            touch_x <= button->x + button->width)
        {
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}
