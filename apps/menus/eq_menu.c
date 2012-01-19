/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2006 Dan Everton
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

#include "config.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "eq_menu.h"
#include "system.h"
#include "kernel.h"
#include "lcd.h"
#include "menu.h"
#include "action.h"
#include "mp3_playback.h"
#include "settings.h"
#include "screens.h"
#include "icons.h"
#include "font.h"
#include "lang.h"
#include "talk.h"
#include "misc.h"
#include "sound.h"
#include "dsp.h"
#include "tree.h"
#include "screen_access.h"
#include "keyboard.h"
#include "gui/scrollbar.h"
#include "menu_common.h"
#include "viewport.h"
#include "exported_menus.h"

/*
 * Utility functions
 */

const char* eq_q_format(char* buffer, size_t buffer_size, int value, const char* unit)
{
    snprintf(buffer, buffer_size, "%d.%d %s", value / EQ_USER_DIVISOR,
         value % EQ_USER_DIVISOR, unit);
    return buffer;
}

const char* eq_precut_format(char* buffer, size_t buffer_size, int value, const char* unit)
{
    snprintf(buffer, buffer_size, "%s%d.%d %s", value == 0 ? " " : "-",
        value / EQ_USER_DIVISOR, value % EQ_USER_DIVISOR, unit);
    return buffer;
}

/*
 * Settings functions
 */
static void eq_apply(void)
{
    int i;
    dsp_set_eq(global_settings.eq_enabled); 
    dsp_set_eq_precut(global_settings.eq_precut);    
    /* Update all bands */
    for(i = 0; i < 24; i++) {
        dsp_set_eq_coefs(i);
    }
}

static int eq_setting_callback(int action, const struct menu_item_ex *this_item)
{
    switch (action)
    {
        case ACTION_ENTER_MENUITEM:
            action = lowlatency_callback(action, this_item);
            break;
        case ACTION_EXIT_MENUITEM:
            eq_apply();
            action = lowlatency_callback(action, this_item);
            break;
    }

    return action;
}
MENUITEM_SETTING(eq_enable, &global_settings.eq_enabled, eq_setting_callback);
MENUITEM_SETTING(eq_precut, &global_settings.eq_precut, eq_setting_callback);

MENUITEM_SETTING(cutoff_0, &global_settings.eq_band0_cutoff, eq_setting_callback);
MENUITEM_SETTING(cutoff_1, &global_settings.eq_band1_cutoff, eq_setting_callback);
MENUITEM_SETTING(cutoff_2, &global_settings.eq_band2_cutoff, eq_setting_callback);
MENUITEM_SETTING(cutoff_3, &global_settings.eq_band3_cutoff, eq_setting_callback);
MENUITEM_SETTING(cutoff_4, &global_settings.eq_band4_cutoff, eq_setting_callback);
MENUITEM_SETTING(cutoff_5, &global_settings.eq_band5_cutoff, eq_setting_callback);
MENUITEM_SETTING(cutoff_6, &global_settings.eq_band6_cutoff, eq_setting_callback);
MENUITEM_SETTING(cutoff_7, &global_settings.eq_band7_cutoff, eq_setting_callback);
MENUITEM_SETTING(cutoff_8, &global_settings.eq_band8_cutoff, eq_setting_callback);
MENUITEM_SETTING(cutoff_9, &global_settings.eq_band9_cutoff, eq_setting_callback);
MENUITEM_SETTING(cutoff_10, &global_settings.eq_band10_cutoff, eq_setting_callback);
MENUITEM_SETTING(cutoff_11, &global_settings.eq_band11_cutoff, eq_setting_callback);
MENUITEM_SETTING(cutoff_12, &global_settings.eq_band12_cutoff, eq_setting_callback);
MENUITEM_SETTING(cutoff_13, &global_settings.eq_band13_cutoff, eq_setting_callback);
MENUITEM_SETTING(cutoff_14, &global_settings.eq_band14_cutoff, eq_setting_callback);
MENUITEM_SETTING(cutoff_15, &global_settings.eq_band15_cutoff, eq_setting_callback);
MENUITEM_SETTING(cutoff_16, &global_settings.eq_band16_cutoff, eq_setting_callback);
MENUITEM_SETTING(cutoff_17, &global_settings.eq_band17_cutoff, eq_setting_callback);
MENUITEM_SETTING(cutoff_18, &global_settings.eq_band18_cutoff, eq_setting_callback);
MENUITEM_SETTING(cutoff_19, &global_settings.eq_band19_cutoff, eq_setting_callback);
MENUITEM_SETTING(cutoff_20, &global_settings.eq_band20_cutoff, eq_setting_callback);
MENUITEM_SETTING(cutoff_21, &global_settings.eq_band21_cutoff, eq_setting_callback);
MENUITEM_SETTING(cutoff_22, &global_settings.eq_band22_cutoff, eq_setting_callback);
MENUITEM_SETTING(cutoff_23, &global_settings.eq_band23_cutoff, eq_setting_callback);

MENUITEM_SETTING(q_0, &global_settings.eq_band0_q, eq_setting_callback);
MENUITEM_SETTING(q_1, &global_settings.eq_band1_q, eq_setting_callback);
MENUITEM_SETTING(q_2, &global_settings.eq_band2_q, eq_setting_callback);
MENUITEM_SETTING(q_3, &global_settings.eq_band3_q, eq_setting_callback);
MENUITEM_SETTING(q_4, &global_settings.eq_band4_q, eq_setting_callback);
MENUITEM_SETTING(q_5, &global_settings.eq_band5_q, eq_setting_callback);
MENUITEM_SETTING(q_6, &global_settings.eq_band6_q, eq_setting_callback);
MENUITEM_SETTING(q_7, &global_settings.eq_band7_q, eq_setting_callback);
MENUITEM_SETTING(q_8, &global_settings.eq_band8_q, eq_setting_callback);
MENUITEM_SETTING(q_9, &global_settings.eq_band9_q, eq_setting_callback);
MENUITEM_SETTING(q_10, &global_settings.eq_band10_q, eq_setting_callback);
MENUITEM_SETTING(q_11, &global_settings.eq_band11_q, eq_setting_callback);
MENUITEM_SETTING(q_12, &global_settings.eq_band12_q, eq_setting_callback);
MENUITEM_SETTING(q_13, &global_settings.eq_band13_q, eq_setting_callback);
MENUITEM_SETTING(q_14, &global_settings.eq_band14_q, eq_setting_callback);
MENUITEM_SETTING(q_15, &global_settings.eq_band15_q, eq_setting_callback);
MENUITEM_SETTING(q_16, &global_settings.eq_band16_q, eq_setting_callback);
MENUITEM_SETTING(q_17, &global_settings.eq_band17_q, eq_setting_callback);
MENUITEM_SETTING(q_18, &global_settings.eq_band18_q, eq_setting_callback);
MENUITEM_SETTING(q_19, &global_settings.eq_band19_q, eq_setting_callback);
MENUITEM_SETTING(q_20, &global_settings.eq_band20_q, eq_setting_callback);
MENUITEM_SETTING(q_21, &global_settings.eq_band21_q, eq_setting_callback);
MENUITEM_SETTING(q_22, &global_settings.eq_band22_q, eq_setting_callback);
MENUITEM_SETTING(q_23, &global_settings.eq_band23_q, eq_setting_callback);

MENUITEM_SETTING(gain_0, &global_settings.eq_band0_gain, eq_setting_callback);
MENUITEM_SETTING(gain_1, &global_settings.eq_band1_gain, eq_setting_callback);
MENUITEM_SETTING(gain_2, &global_settings.eq_band2_gain, eq_setting_callback);
MENUITEM_SETTING(gain_3, &global_settings.eq_band3_gain, eq_setting_callback);
MENUITEM_SETTING(gain_4, &global_settings.eq_band4_gain, eq_setting_callback);
MENUITEM_SETTING(gain_5, &global_settings.eq_band5_gain, eq_setting_callback);
MENUITEM_SETTING(gain_6, &global_settings.eq_band6_gain, eq_setting_callback);
MENUITEM_SETTING(gain_7, &global_settings.eq_band7_gain, eq_setting_callback);
MENUITEM_SETTING(gain_8, &global_settings.eq_band8_gain, eq_setting_callback);
MENUITEM_SETTING(gain_9, &global_settings.eq_band9_gain, eq_setting_callback);
MENUITEM_SETTING(gain_10, &global_settings.eq_band10_gain, eq_setting_callback);
MENUITEM_SETTING(gain_11, &global_settings.eq_band11_gain, eq_setting_callback);
MENUITEM_SETTING(gain_12, &global_settings.eq_band12_gain, eq_setting_callback);
MENUITEM_SETTING(gain_13, &global_settings.eq_band13_gain, eq_setting_callback);
MENUITEM_SETTING(gain_14, &global_settings.eq_band14_gain, eq_setting_callback);
MENUITEM_SETTING(gain_15, &global_settings.eq_band15_gain, eq_setting_callback);
MENUITEM_SETTING(gain_16, &global_settings.eq_band16_gain, eq_setting_callback);
MENUITEM_SETTING(gain_17, &global_settings.eq_band17_gain, eq_setting_callback);
MENUITEM_SETTING(gain_18, &global_settings.eq_band18_gain, eq_setting_callback);
MENUITEM_SETTING(gain_19, &global_settings.eq_band19_gain, eq_setting_callback);
MENUITEM_SETTING(gain_20, &global_settings.eq_band20_gain, eq_setting_callback);
MENUITEM_SETTING(gain_21, &global_settings.eq_band21_gain, eq_setting_callback);
MENUITEM_SETTING(gain_22, &global_settings.eq_band22_gain, eq_setting_callback);
MENUITEM_SETTING(gain_23, &global_settings.eq_band23_gain, eq_setting_callback);

static char* gainitem_get_name(int selected_item, void * data, char *buffer)
{
    (void)selected_item;
    int *setting = (int*)data;
    snprintf(buffer, MAX_PATH, str(LANG_EQUALIZER_GAIN_ITEM), *setting);
    return buffer;
}

static int gainitem_speak_item(int selected_item, void * data)
{
    (void)selected_item;
    int *setting = (int*)data;
    talk_number(*setting, false);
    talk_id(LANG_EQUALIZER_GAIN_ITEM, true);
    return 0;
}

static int do_option(void * param)
{
    const struct menu_item_ex *setting = (const struct menu_item_ex*)param;
    lowlatency_callback(ACTION_ENTER_MENUITEM, setting);
    do_setting_from_menu(setting, NULL);
    eq_apply();
    lowlatency_callback(ACTION_EXIT_MENUITEM, setting);
    return 0;
}

MENUITEM_FUNCTION_DYNTEXT(gain_item_0, MENU_FUNC_USEPARAM,
                          do_option, (void*)&gain_0, 
                          gainitem_get_name, gainitem_speak_item,
                          &global_settings.eq_band0_cutoff, NULL, Icon_NOICON);
MENUITEM_FUNCTION_DYNTEXT(gain_item_1, MENU_FUNC_USEPARAM,
                          do_option, (void*)&gain_1,
                          gainitem_get_name, gainitem_speak_item,
                          &global_settings.eq_band1_cutoff, NULL, Icon_NOICON);
MENUITEM_FUNCTION_DYNTEXT(gain_item_2, MENU_FUNC_USEPARAM,
                          do_option, (void*)&gain_2,
                          gainitem_get_name, gainitem_speak_item,
                          &global_settings.eq_band2_cutoff, NULL, Icon_NOICON);
MENUITEM_FUNCTION_DYNTEXT(gain_item_3, MENU_FUNC_USEPARAM,
                          do_option, (void*)&gain_3,
                          gainitem_get_name, gainitem_speak_item,
                          &global_settings.eq_band3_cutoff, NULL, Icon_NOICON);
MENUITEM_FUNCTION_DYNTEXT(gain_item_4, MENU_FUNC_USEPARAM,
                          do_option, (void*)&gain_4,
                          gainitem_get_name, gainitem_speak_item,
                          &global_settings.eq_band4_cutoff, NULL, Icon_NOICON);
MENUITEM_FUNCTION_DYNTEXT(gain_item_5, MENU_FUNC_USEPARAM,
                          do_option, (void*)&gain_5, 
                          gainitem_get_name, gainitem_speak_item,
                          &global_settings.eq_band5_cutoff, NULL, Icon_NOICON);
MENUITEM_FUNCTION_DYNTEXT(gain_item_6, MENU_FUNC_USEPARAM,
                          do_option, (void*)&gain_6,
                          gainitem_get_name, gainitem_speak_item,
                          &global_settings.eq_band6_cutoff, NULL, Icon_NOICON);
MENUITEM_FUNCTION_DYNTEXT(gain_item_7, MENU_FUNC_USEPARAM,                                                                                      
                          do_option, (void*)&gain_7,                                                                                            
                          gainitem_get_name, gainitem_speak_item,                                                                               
                          &global_settings.eq_band7_cutoff, NULL, Icon_NOICON);                                                                 
MENUITEM_FUNCTION_DYNTEXT(gain_item_8, MENU_FUNC_USEPARAM,                                                                                      
                          do_option, (void*)&gain_8,                                                                                            
                          gainitem_get_name, gainitem_speak_item,                                                                               
                          &global_settings.eq_band8_cutoff, NULL, Icon_NOICON);                                                                 
MENUITEM_FUNCTION_DYNTEXT(gain_item_9, MENU_FUNC_USEPARAM,                                                                                      
                          do_option, (void*)&gain_9,                                                                                            
                          gainitem_get_name, gainitem_speak_item,                                                                               
                          &global_settings.eq_band9_cutoff, NULL, Icon_NOICON);                                                                 
MENUITEM_FUNCTION_DYNTEXT(gain_item_10, MENU_FUNC_USEPARAM,
                          do_option, (void*)&gain_10, 
                          gainitem_get_name, gainitem_speak_item,
                          &global_settings.eq_band10_cutoff, NULL, Icon_NOICON);
MENUITEM_FUNCTION_DYNTEXT(gain_item_11, MENU_FUNC_USEPARAM,
                          do_option, (void*)&gain_11,
                          gainitem_get_name, gainitem_speak_item,
                          &global_settings.eq_band11_cutoff, NULL, Icon_NOICON);
MENUITEM_FUNCTION_DYNTEXT(gain_item_12, MENU_FUNC_USEPARAM,
                          do_option, (void*)&gain_12,
                          gainitem_get_name, gainitem_speak_item,
                          &global_settings.eq_band12_cutoff, NULL, Icon_NOICON);
MENUITEM_FUNCTION_DYNTEXT(gain_item_13, MENU_FUNC_USEPARAM,
                          do_option, (void*)&gain_13,
                          gainitem_get_name, gainitem_speak_item,
                          &global_settings.eq_band13_cutoff, NULL, Icon_NOICON);
MENUITEM_FUNCTION_DYNTEXT(gain_item_14, MENU_FUNC_USEPARAM,
                          do_option, (void*)&gain_14,
                          gainitem_get_name, gainitem_speak_item,
                          &global_settings.eq_band14_cutoff, NULL, Icon_NOICON);
MENUITEM_FUNCTION_DYNTEXT(gain_item_15, MENU_FUNC_USEPARAM,
                          do_option, (void*)&gain_15, 
                          gainitem_get_name, gainitem_speak_item,
                          &global_settings.eq_band15_cutoff, NULL, Icon_NOICON);
MENUITEM_FUNCTION_DYNTEXT(gain_item_16, MENU_FUNC_USEPARAM,
                          do_option, (void*)&gain_16,
                          gainitem_get_name, gainitem_speak_item,
                          &global_settings.eq_band16_cutoff, NULL, Icon_NOICON);
MENUITEM_FUNCTION_DYNTEXT(gain_item_17, MENU_FUNC_USEPARAM,                                                                                      
                          do_option, (void*)&gain_17,                                                                                            
                          gainitem_get_name, gainitem_speak_item,                                                                               
                          &global_settings.eq_band17_cutoff, NULL, Icon_NOICON);                                                                 
MENUITEM_FUNCTION_DYNTEXT(gain_item_18, MENU_FUNC_USEPARAM,                                                                                      
                          do_option, (void*)&gain_18,                                                                                            
                          gainitem_get_name, gainitem_speak_item,                                                                               
                          &global_settings.eq_band18_cutoff, NULL, Icon_NOICON);                                                                 
MENUITEM_FUNCTION_DYNTEXT(gain_item_19, MENU_FUNC_USEPARAM,                                                                                      
                          do_option, (void*)&gain_19,                                                                                            
                          gainitem_get_name, gainitem_speak_item,                                                                               
                          &global_settings.eq_band19_cutoff, NULL, Icon_NOICON);                                                                 
MENUITEM_FUNCTION_DYNTEXT(gain_item_20, MENU_FUNC_USEPARAM,
                          do_option, (void*)&gain_20, 
                          gainitem_get_name, gainitem_speak_item,
                          &global_settings.eq_band20_cutoff, NULL, Icon_NOICON);
MENUITEM_FUNCTION_DYNTEXT(gain_item_21, MENU_FUNC_USEPARAM,
                          do_option, (void*)&gain_21,
                          gainitem_get_name, gainitem_speak_item,
                          &global_settings.eq_band21_cutoff, NULL, Icon_NOICON);
MENUITEM_FUNCTION_DYNTEXT(gain_item_22, MENU_FUNC_USEPARAM,
                          do_option, (void*)&gain_22,
                          gainitem_get_name, gainitem_speak_item,
                          &global_settings.eq_band22_cutoff, NULL, Icon_NOICON);
MENUITEM_FUNCTION_DYNTEXT(gain_item_23, MENU_FUNC_USEPARAM,
                          do_option, (void*)&gain_23,
                          gainitem_get_name, gainitem_speak_item,
                          &global_settings.eq_band23_cutoff, NULL, Icon_NOICON);
                                                                                                                                                
MAKE_MENU(gain_menu, ID2P(LANG_EQUALIZER_GAIN), NULL, Icon_NOICON, &gain_item_0,                                                                
            &gain_item_1, &gain_item_2, &gain_item_3, &gain_item_4, &gain_item_5, &gain_item_6, &gain_item_7, &gain_item_8, &gain_item_9, &gain_item_10, &gain_item_11, &gain_item_12, &gain_item_13, &gain_item_14, &gain_item_15, &gain_item_16, &gain_item_17, &gain_item_18, &gain_item_19, &gain_item_20, &gain_item_21, &gain_item_22, &gain_item_23);      
                                                                                                                                                
static const struct menu_item_ex *band_items[22][3] = {                                                                                          
    { &cutoff_1, &q_1, &gain_1 },                                                                                                               
    { &cutoff_2, &q_2, &gain_2 },                                                                                                               
    { &cutoff_3, &q_3, &gain_3 },                                                                                                               
    { &cutoff_4, &q_4, &gain_4 },                                                                                                               
    { &cutoff_5, &q_5, &gain_5 },                                                                                                               
    { &cutoff_6, &q_6, &gain_6 },                                                                                                               
    { &cutoff_7, &q_7, &gain_7 },                                                                                                               
    { &cutoff_8, &q_8, &gain_8 },                                                                                                                
    { &cutoff_9, &q_9, &gain_9 },                                                                                                               
    { &cutoff_10, &q_10, &gain_10 },
    { &cutoff_11, &q_11, &gain_11 },                                                                                                               
    { &cutoff_12, &q_12, &gain_12 },                                                                                                               
    { &cutoff_13, &q_13, &gain_13 },                                                                                                               
    { &cutoff_14, &q_14, &gain_14 },                                                                                                               
    { &cutoff_15, &q_15, &gain_15 },                                                                                                               
    { &cutoff_16, &q_16, &gain_16 },                                                                                                               
    { &cutoff_17, &q_17, &gain_17 },                                                                                                               
    { &cutoff_18, &q_18, &gain_18 },                                                                                                                
    { &cutoff_19, &q_19, &gain_19 },                                                                                                               
    { &cutoff_20, &q_20, &gain_20 },                                                                                                               
    { &cutoff_21, &q_21, &gain_21 },                                                                                                               
    { &cutoff_22, &q_22, &gain_22 }                                                                                                               
};                                                                                                                                              
                                                                                                                                               
static char* centerband_get_name(int selected_item, void * data, char *buffer)
{
    (void)selected_item;
    int band = (intptr_t)data;
    snprintf(buffer, MAX_PATH, str(LANG_EQUALIZER_BAND_PEAK), band);
    return buffer;
}

static int centerband_speak_item(int selected_item, void * data)
{
    (void)selected_item;
    int band = (intptr_t)data;
    talk_id(LANG_EQUALIZER_BAND_PEAK, false);
    talk_number(band, true);
    return 0;
}

static int do_center_band_menu(void* param)
{
    int band = (intptr_t)param;
    struct menu_item_ex menu;
    struct menu_callback_with_desc cb_and_desc;
    char desc[MAX_PATH];
    
    cb_and_desc.menu_callback = NULL;
    snprintf(desc, MAX_PATH, str(LANG_EQUALIZER_BAND_PEAK), band);
    cb_and_desc.desc = desc;
    cb_and_desc.icon_id = Icon_EQ;
    menu.flags = MT_MENU|(3<<MENU_COUNT_SHIFT)|MENU_HAS_DESC;
    menu.submenus = band_items[band-1];
    menu.callback_and_desc = &cb_and_desc;
    do_menu(&menu, NULL, NULL, false);
    return 0;
}

MAKE_MENU(band_0_menu, ID2P(LANG_EQUALIZER_BAND_LOW_SHELF), NULL, 
            Icon_EQ, &cutoff_0, &q_0, &gain_0);
MENUITEM_FUNCTION_DYNTEXT(band_1_menu, MENU_FUNC_USEPARAM,
                          do_center_band_menu, (void*)1,
                          centerband_get_name, centerband_speak_item,
                          (void*)1, NULL, Icon_EQ);
MENUITEM_FUNCTION_DYNTEXT(band_2_menu, MENU_FUNC_USEPARAM, 
                          do_center_band_menu, (void*)2, 
                          centerband_get_name, centerband_speak_item,
                          (void*)2, NULL, Icon_EQ);
MENUITEM_FUNCTION_DYNTEXT(band_3_menu, MENU_FUNC_USEPARAM, 
                          do_center_band_menu, (void*)3, 
                          centerband_get_name, centerband_speak_item,
                          (void*)3, NULL, Icon_EQ);
MENUITEM_FUNCTION_DYNTEXT(band_4_menu, MENU_FUNC_USEPARAM, 
                          do_center_band_menu, (void*)4, 
                          centerband_get_name, centerband_speak_item,
                          (void*)4, NULL, Icon_EQ);
MENUITEM_FUNCTION_DYNTEXT(band_5_menu, MENU_FUNC_USEPARAM, 
                          do_center_band_menu, (void*)5, 
                          centerband_get_name, centerband_speak_item,
                          (void*)5, NULL, Icon_EQ);
MENUITEM_FUNCTION_DYNTEXT(band_6_menu, MENU_FUNC_USEPARAM, 
                          do_center_band_menu, (void*)6, 
                          centerband_get_name, centerband_speak_item,
                          (void*)6, NULL, Icon_EQ);
MENUITEM_FUNCTION_DYNTEXT(band_7_menu, MENU_FUNC_USEPARAM,                                                                                              
                          do_center_band_menu, (void*)7,                                                                                                
                          centerband_get_name, centerband_speak_item,                                                                                   
                          (void*)7, NULL, Icon_EQ);                                                                                                     
MENUITEM_FUNCTION_DYNTEXT(band_8_menu, MENU_FUNC_USEPARAM,                                                                                              
                          do_center_band_menu, (void*)8,                                                                                                
                          centerband_get_name, centerband_speak_item,                                                                                   
                          (void*)8, NULL, Icon_EQ);                                                                                                     
MENUITEM_FUNCTION_DYNTEXT(band_9_menu, MENU_FUNC_USEPARAM, 
                          do_center_band_menu, (void*)9, 
                          centerband_get_name, centerband_speak_item,
                          (void*)9, NULL, Icon_EQ);
MENUITEM_FUNCTION_DYNTEXT(band_10_menu, MENU_FUNC_USEPARAM, 
                          do_center_band_menu, (void*)10, 
                          centerband_get_name, centerband_speak_item,
                          (void*)10, NULL, Icon_EQ);
MENUITEM_FUNCTION_DYNTEXT(band_11_menu, MENU_FUNC_USEPARAM,
                          do_center_band_menu, (void*)11,
                          centerband_get_name, centerband_speak_item,
                          (void*)11, NULL, Icon_EQ);
MENUITEM_FUNCTION_DYNTEXT(band_12_menu, MENU_FUNC_USEPARAM, 
                          do_center_band_menu, (void*)12, 
                          centerband_get_name, centerband_speak_item,
                          (void*)12, NULL, Icon_EQ);
MENUITEM_FUNCTION_DYNTEXT(band_13_menu, MENU_FUNC_USEPARAM, 
                          do_center_band_menu, (void*)13, 
                          centerband_get_name, centerband_speak_item,
                          (void*)13, NULL, Icon_EQ);
MENUITEM_FUNCTION_DYNTEXT(band_14_menu, MENU_FUNC_USEPARAM, 
                          do_center_band_menu, (void*)14, 
                          centerband_get_name, centerband_speak_item,
                          (void*)14, NULL, Icon_EQ);
MENUITEM_FUNCTION_DYNTEXT(band_15_menu, MENU_FUNC_USEPARAM, 
                          do_center_band_menu, (void*)15, 
                          centerband_get_name, centerband_speak_item,
                          (void*)15, NULL, Icon_EQ);
MENUITEM_FUNCTION_DYNTEXT(band_16_menu, MENU_FUNC_USEPARAM, 
                          do_center_band_menu, (void*)16, 
                          centerband_get_name, centerband_speak_item,
                          (void*)16, NULL, Icon_EQ);
MENUITEM_FUNCTION_DYNTEXT(band_17_menu, MENU_FUNC_USEPARAM,                                                                                              
                          do_center_band_menu, (void*)17,                                                                                                
                          centerband_get_name, centerband_speak_item,                                                                                   
                          (void*)17, NULL, Icon_EQ);                                                                                                     
MENUITEM_FUNCTION_DYNTEXT(band_18_menu, MENU_FUNC_USEPARAM,                                                                                              
                          do_center_band_menu, (void*)18,                                                                                                
                          centerband_get_name, centerband_speak_item,                                                                                   
                          (void*)18, NULL, Icon_EQ);                                                                                                     
MENUITEM_FUNCTION_DYNTEXT(band_19_menu, MENU_FUNC_USEPARAM, 
                          do_center_band_menu, (void*)19, 
                          centerband_get_name, centerband_speak_item,
                          (void*)19, NULL, Icon_EQ);
MENUITEM_FUNCTION_DYNTEXT(band_20_menu, MENU_FUNC_USEPARAM, 
                          do_center_band_menu, (void*)20, 
                          centerband_get_name, centerband_speak_item,
                          (void*)20, NULL, Icon_EQ);
MENUITEM_FUNCTION_DYNTEXT(band_21_menu, MENU_FUNC_USEPARAM, 
                          do_center_band_menu, (void*)21, 
                          centerband_get_name, centerband_speak_item,
                          (void*)21, NULL, Icon_EQ);
MENUITEM_FUNCTION_DYNTEXT(band_22_menu, MENU_FUNC_USEPARAM, 
                          do_center_band_menu, (void*)22, 
                          centerband_get_name, centerband_speak_item,
                          (void*)22, NULL, Icon_EQ);
MAKE_MENU(band_23_menu, ID2P(LANG_EQUALIZER_BAND_HIGH_SHELF), NULL,                                                                                      
            Icon_EQ, &cutoff_23, &q_23, &gain_23);                                                                                                         
                                                                                                                                                        
MAKE_MENU(advanced_eq_menu_, ID2P(LANG_EQUALIZER_ADVANCED), NULL, Icon_EQ,                                                                              
            &band_0_menu, &band_1_menu, &band_2_menu, &band_3_menu, &band_4_menu, &band_5_menu, &band_6_menu, &band_7_menu, &band_8_menu, &band_9_menu, &band_10_menu, &band_11_menu, &band_12_menu, &band_13_menu, &band_14_menu, &band_15_menu, &band_16_menu, &band_17_menu, &band_18_menu, &band_19_menu, &band_20_menu, &band_21_menu, &band_22_menu, &band_23_menu);


enum eq_slider_mode {
    GAIN,
    CUTOFF,
    Q,
};

enum eq_type {
    LOW_SHELF,
    PEAK,
    HIGH_SHELF
};

/* Size of just the slider/srollbar */
#define SCROLLBAR_SIZE 18

/* Draw the UI for a whole EQ band */
static int draw_eq_slider(struct screen * screen, int x, int y,
    int width, int cutoff, int q, int gain, bool selected,
    enum eq_slider_mode mode, int band)
{
    char buf[26];
    int steps, min_item, max_item;
    int abs_gain = abs(gain);
    int x1, x2, y1, total_height;
    int w, h;

    switch(mode) {
    case Q:
        steps = EQ_Q_MAX - EQ_Q_MIN;
        min_item = q - EQ_Q_STEP - EQ_Q_MIN;
        max_item = q + EQ_Q_STEP - EQ_Q_MIN;
        break;        
    case CUTOFF:
        steps = EQ_CUTOFF_MAX - EQ_CUTOFF_MIN;
        min_item = cutoff - EQ_CUTOFF_FAST_STEP * 2;
        max_item = cutoff + EQ_CUTOFF_FAST_STEP * 2;
        break;         
    case GAIN:
    default:
        steps = EQ_GAIN_MAX - EQ_GAIN_MIN;
        min_item = abs(EQ_GAIN_MIN) + gain - EQ_GAIN_STEP * 5;
        max_item = abs(EQ_GAIN_MIN) + gain + EQ_GAIN_STEP * 5;
        break;
    }

    /* Start two pixels in, one for border, one for margin */
    x1 = x + 2;
    y1 = y + 2;

    /* Print out the band label */
    if (band == 0) {
        screen->putsxy(x1, y1, "LS: ");
        screen->getstringsize("LS:", &w, &h);
    } else if (band == 23) {
        screen->putsxy(x1, y1, "HS: ");
        screen->getstringsize("HS:", &w, &h);
    } else {
        snprintf(buf, sizeof(buf),  "PK%d:", band);
        screen->putsxy(x1, y1, buf);
        screen->getstringsize(buf, &w, &h);
    }

    screen->getstringsize("A", &w, &h);
    x1 += 5*w; /* 4 chars for label + 1 space = 5 */

    /* Print out gain part of status line (left justify after label) */
    if (mode == GAIN && selected)
        screen->set_drawmode(DRMODE_SOLID | DRMODE_INVERSEVID);
    else
        screen->set_drawmode(DRMODE_SOLID);

    snprintf(buf, sizeof(buf), "%s%2d.%d%s", gain < 0 ? "-" : " ",
        abs_gain / EQ_USER_DIVISOR, abs_gain % EQ_USER_DIVISOR,
        screen->lcdwidth >= 160 ? "dB" : "");
    screen->putsxy(x1, y1, buf);
    screen->getstringsize(buf, &w, &h);
    x1 += w;

    /* Print out Q part of status line (right justify) */
    if (mode == Q && selected)
        screen->set_drawmode(DRMODE_SOLID | DRMODE_INVERSEVID);
    else
        screen->set_drawmode(DRMODE_SOLID);

    snprintf(buf, sizeof(buf), "%d.%d%s", q / EQ_USER_DIVISOR,
             q % EQ_USER_DIVISOR, screen->lcdwidth >= 160 ? " Q" : "");
    screen->getstringsize(buf, &w, &h);
    x2 = x + width - w - 2;
    screen->putsxy(x2, y1, buf);

    /* Print out cutoff part of status line (center between gain & Q) */
    if (mode == CUTOFF && selected)
        screen->set_drawmode(DRMODE_SOLID | DRMODE_INVERSEVID);
    else
        screen->set_drawmode(DRMODE_SOLID);
    
    snprintf(buf, sizeof(buf),  "%5d%s", cutoff,
             screen->lcdwidth >= 160 ? "Hz" : "");
    screen->getstringsize(buf, &w, &h);
    x1 = x1 + (x2 - x1 - w)/2;
    screen->putsxy(x1, y1, buf);

    /* Draw selection box */
    total_height = 3 + h + 1 + SCROLLBAR_SIZE + 3;
    screen->set_drawmode(DRMODE_SOLID);
    if (selected) {
        screen->drawrect(x, y, width, total_height);
    }

    /* Draw horizontal slider. Reuse scrollbar for this */
    gui_scrollbar_draw(screen, x + 3, y1 + h + 1, width - 6, SCROLLBAR_SIZE,
                       steps, min_item, max_item, HORIZONTAL);

    return total_height;
}

/* Draw's all the EQ sliders. Returns the total height of the sliders drawn */
static void draw_eq_sliders(struct screen * screen, int x, int y,
                            int nb_eq_sliders, int start_item,
                            int current_band, enum eq_slider_mode mode)
{
    int i, gain, q, cutoff;
    int height = y;
    int *setting = &global_settings.eq_band0_cutoff;

    start_item = MIN(start_item, 24-nb_eq_sliders);
    for (i=0; i<24; i++) {
        cutoff = *setting++;
        q = *setting++;
        gain = *setting++;

        if (i == start_item + nb_eq_sliders)
            break;

        if (i >= start_item) {
            height += draw_eq_slider(screen, x, height, screen->lcdwidth - x - 1,
                                     cutoff, q, gain, i == current_band, mode,
                                     i);
            /* add a margin */
            height++;
        }
    }
    if (nb_eq_sliders != 24)
        gui_scrollbar_draw(screen, 0, y, SCROLLBAR_SIZE - 1,
                           screen->lcdheight - y, 24,
                           start_item, start_item + nb_eq_sliders,
                           VERTICAL);
    return;
}

/* Provides a graphical means of editing the EQ settings */
bool eq_menu_graphical(void)
{   

#if (CONFIG_PLATFORM&PLATFORM_ANDROID)
#ifdef HAVE_TOUCHSCREEN
    touchscreen_set_mode(TOUCHSCREEN_BUTTON);
#endif
#endif
    bool exit_request = false;
    bool result = true;
    bool has_changed = false;
    int button;
    int *setting;
    int current_band, x, y, step, fast_step, min, max;
    enum eq_slider_mode mode;
    char buf[24];
    int w, h, height, start_item, nb_eq_sliders[NB_SCREENS];
    FOR_NB_SCREENS(i)
        viewportmanager_theme_enable(i, false, NULL);


    FOR_NB_SCREENS(i) {
        screens[i].set_viewport(NULL);
        screens[i].setfont(FONT_UI);
        screens[i].clear_display();

        /* Figure out how many sliders can be drawn on the screen */
        screens[i].getstringsize("A", &w, &h);

        /* Total height includes margins (1), text, slider, and line selector (1) */
        height = 3 + h + 1 + SCROLLBAR_SIZE + 3;
        nb_eq_sliders[i] = screens[i].lcdheight / height;

        /* Make sure the "Edit Mode" text fits too */
        height = nb_eq_sliders[i]*height + h + 2;
        if (height > screens[i].lcdheight)
            nb_eq_sliders[i]--;

        if (nb_eq_sliders[i] > 24)
            nb_eq_sliders[i] = 24;
    }

    y = h + 1;
    
    /* Start off editing gain on the first band */
    mode = GAIN;
    current_band = 0;
    
    while (!exit_request) {
        FOR_NB_SCREENS(i)
        {
            screens[i].clear_display();

            /* Set pointer to the band data currently editable */
            if (mode == GAIN) {
                /* gain */
                setting = &global_settings.eq_band0_gain;
                setting += current_band * 3;

                step = EQ_GAIN_STEP;
                fast_step = EQ_GAIN_FAST_STEP;
                min = EQ_GAIN_MIN;
                max = EQ_GAIN_MAX;
            
                snprintf(buf, sizeof(buf), str(LANG_SYSFONT_EQUALIZER_EDIT_MODE),
                         str(LANG_SYSFONT_GAIN), "(dB)");

                screens[i].putsxy(0, 0, buf);
            } else if (mode == CUTOFF) {
                /* cutoff */
                setting = &global_settings.eq_band0_cutoff;
                setting += current_band * 3;

                step = EQ_CUTOFF_STEP;
                fast_step = EQ_CUTOFF_FAST_STEP;
                min = EQ_CUTOFF_MIN;
                max = EQ_CUTOFF_MAX;

                snprintf(buf, sizeof(buf), str(LANG_SYSFONT_EQUALIZER_EDIT_MODE),
                         str(LANG_SYSFONT_EQUALIZER_BAND_CUTOFF), "(Hz)");

                screens[i].putsxy(0, 0, buf);
            } else {
                /* Q */
                setting = &global_settings.eq_band0_q;
                setting += current_band * 3;

                step = EQ_Q_STEP;
                fast_step = EQ_Q_FAST_STEP;
                min = EQ_Q_MIN;
                max = EQ_Q_MAX;

                snprintf(buf, sizeof(buf), str(LANG_SYSFONT_EQUALIZER_EDIT_MODE),
                         str(LANG_EQUALIZER_BAND_Q), "");

                screens[i].putsxy(0, 0, buf);
            }

            /* Draw scrollbar if needed */
            if (nb_eq_sliders[i] != 24)
            {
                if (current_band == 0) {
                    start_item = 0;
                } else if (current_band == 23) {
                    start_item = 24 - nb_eq_sliders[i];
                } else {
                    start_item = current_band - 1;
                }
                x = SCROLLBAR_SIZE;
            } else {
                x = 1;
                start_item = 0;
            }
            /* Draw equalizer band details */
            draw_eq_sliders(&screens[i], x, y, nb_eq_sliders[i], start_item,
                            current_band, mode);

            screens[i].update();
        }
        
        button = get_action(CONTEXT_SETTINGS_EQ,TIMEOUT_BLOCK);

        switch (button) {
        case ACTION_SETTINGS_DEC:
        case ACTION_SETTINGS_DECREPEAT:
            *(setting) -= step;
            has_changed = true;
            if (*(setting) < min)
                *(setting) = min;
            break;

        case ACTION_SETTINGS_INC:
        case ACTION_SETTINGS_INCREPEAT:
            *(setting) += step;
            has_changed = true;
            if (*(setting) > max)
                *(setting) = max;
            break;

        case ACTION_SETTINGS_INCBIGSTEP:
            *(setting) += fast_step;
            has_changed = true;
            if (*(setting) > max)
                *(setting) = max;
            break;

        case ACTION_SETTINGS_DECBIGSTEP:
            *(setting) -= fast_step;
            has_changed = true;
            if (*(setting) < min)
                *(setting) = min;
            break;

        case ACTION_STD_PREV:
        case ACTION_STD_PREVREPEAT:
            current_band--;
            if (current_band < 0)
                current_band = 23; /* wrap around */
            break;

        case ACTION_STD_NEXT:
        case ACTION_STD_NEXTREPEAT:
            current_band++;
            if (current_band > 23)
                current_band = 0; /* wrap around */
            break;

        case ACTION_STD_OK:
            mode++;
            if (mode > Q)
                mode = GAIN; /* wrap around */
            break;

        case ACTION_STD_CANCEL:
            exit_request = true;
            result = false;
            break;
        default:
            if(default_event_handler(button) == SYS_USB_CONNECTED) {
                exit_request = true;
                result = true;
            }
            break;
        }
        
        /* Update the filter if the user changed something */
        if (has_changed) {
            dsp_set_eq_coefs(current_band);
            has_changed = false;
        }
    }
    /* Reset screen settings */
    FOR_NB_SCREENS(i) 
    {       
	screens[i].setfont(FONT_UI);
        screens[i].clear_display();
        screens[i].set_viewport(NULL);
        viewportmanager_theme_undo(i, false);
#if (CONFIG_PLATFORM&PLATFORM_ANDROID)
#ifdef HAVE_TOUCHSCREEN
        touchscreen_set_mode(TOUCHSCREEN_POINT);
#endif
#endif
    }
    return result;
}

static bool eq_save_preset(void)
{
    /* make sure that the eq is enabled for setting saving */
    bool enabled = global_settings.eq_enabled;
    global_settings.eq_enabled = true;
    
    bool result = settings_save_config(SETTINGS_SAVE_EQPRESET);
    
    global_settings.eq_enabled = enabled;

    return result;
}

/* Allows browsing of preset files */
static void eq_reset_gain(void)
{
    global_settings.eq_band0_gain = 0;
    global_settings.eq_band1_gain = 0;
    global_settings.eq_band2_gain = 0;
    global_settings.eq_band3_gain = 0;
    global_settings.eq_band4_gain = 0;
    global_settings.eq_band5_gain = 0;
    global_settings.eq_band6_gain = 0;
    global_settings.eq_band7_gain = 0;
    global_settings.eq_band8_gain = 0;
    global_settings.eq_band9_gain = 0;
    global_settings.eq_band10_gain = 0;
    global_settings.eq_band11_gain = 0;
    global_settings.eq_band12_gain = 0;
    global_settings.eq_band13_gain = 0;
    global_settings.eq_band14_gain = 0;
    global_settings.eq_band15_gain = 0;
    global_settings.eq_band16_gain = 0;
    global_settings.eq_band17_gain = 0;
    global_settings.eq_band18_gain = 0;
    global_settings.eq_band19_gain = 0;
    global_settings.eq_band20_gain = 0;
    global_settings.eq_band21_gain = 0;
    global_settings.eq_band22_gain = 0;
    global_settings.eq_band23_gain = 0;
    return eq_apply();
}

/* Allows browsing of preset files */
static bool eq_browse_preset(void)
{
    static struct browse_folder_info eqs = { EQS_DIR, SHOW_CFG };

    /*Reset all the bands gain before loding presets*/
    eq_reset_gain();
    return browse_folder((void*)&eqs);
}

MENUITEM_FUNCTION(eq_graphical, 0, ID2P(LANG_EQUALIZER_GRAPHICAL),
                    (int(*)(void))eq_menu_graphical, NULL, lowlatency_callback, 
                    Icon_EQ);
MENUITEM_FUNCTION(eq_save, 0, ID2P(LANG_EQUALIZER_SAVE),
                    (int(*)(void))eq_save_preset, NULL, NULL, Icon_NOICON);
MENUITEM_FUNCTION(eq_browse, 0, ID2P(LANG_EQUALIZER_BROWSE),
                    (int(*)(void))eq_browse_preset, NULL, NULL,
                    Icon_NOICON);

MAKE_MENU(equalizer_menu, ID2P(LANG_EQUALIZER), NULL, Icon_EQ,
        &eq_enable, &eq_graphical, &eq_precut, &gain_menu, 
        &advanced_eq_menu_, &eq_save, &eq_browse);
