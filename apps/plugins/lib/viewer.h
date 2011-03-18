/***************************************************************************
*             __________               __   ___.
*   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
*   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
*   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
*   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
*                     \/            \/     \/    \/            \/
* $Id: viewer.h 12008 2007-01-14 13:48:09Z dave $
*
* Copyright (C) 2007 Timo Horstschäfer
*
*
* All files in this archive are subject to the GNU General Public License.
* See the file COPYING in the source tree root for full license agreement.
*
* This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
* KIND, either express or implied.
*
****************************************************************************/

#include "plugin.h"

#if (CONFIG_KEYPAD == IRIVER_H100_PAD) || \
    (CONFIG_KEYPAD == IRIVER_H300_PAD) || \
    (CONFIG_KEYPAD == SANSA_E200_PAD)
#define VIEWER_HAS_SHORTCUT
#define BUTTON_VIEWER_SHORTCUT BUTTON_REC
#endif

/** \brief Shows a text viewer.
 *
 * How to view some text:
 *  viewer(api, textptr);       // Text must be zero-terminated.
 *
 * For full control, call each function seperately:
 *  viewer_init(rb);            // Must be done once in a plugin.
 *  viewer_set_text(bufferptr, textptr);
 *  viewer_run();               // Display the text.
 *
 * viewer() is just a short form for all three calls.
 *
 * @param text Zero-terminated string.
 */
int viewer(const struct plugin_api *api, const char *text);
#define VIEWER_ATTACHED_USB (-2)
#define VIEWER_EXIT         0
#define VIEWER_CUSTOM       (VIEWER_EXIT+1)
/**< Use values starting with VIEWER_CUSTOM for your own return values. */

void viewer_init(const struct plugin_api *api);
int viewer_run(void);

/** Get the line to which the address belongs to. */
int viewer_line(const char *p);

/** Initialize the viewer with a new text.
 *
 * @param buffer A small buffer.
 * @param text   The new text to be displayed.
 * @return       Needed size of the buffer.
 */
int viewer_set_text(void *buffer, const char *txt);
void viewer_set_callback(void (*callback)(int button));
void viewer_set_scroll(int newscroll);
void viewer_set_pos(int line);
void viewer_set_backlight(int newbacklight);
void viewer_set_shortcut(int shortcutindex);

int viewer_get_scroll(void);
int viewer_get_pos(void);
int viewer_get_backlight(void);
int viewer_get_shortcut(void);

/** Functions to control the viewer in the callback function. */
void viewer_up(void);
void viewer_down(void);
bool viewer_search(void);
bool viewer_menu(void);
void viewer_redraw(void);
void (*viewer_shortcut)(void);
bool viewer_menu_search(void);
bool viewer_menu_scroll(void);
bool viewer_menu_backlight(void);
void viewer_exit(int retval);
