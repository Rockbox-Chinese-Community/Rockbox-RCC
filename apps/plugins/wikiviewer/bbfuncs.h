/***************************************************************************
 *
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 *
 *
 * Copyright by ???
 *
 *
 * All files in this archive are subject to the GNU General Public License.
 * See the file COPYING in the source tree root for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/

#ifndef BBFUNCS_H
#define BBFUNCS_H

void error_die(const char* msg);
void error_msg(const char* msg);
size_t safe_read(int fd, void *buf, size_t count);
ssize_t full_read(int fd, void *buf, size_t len);
void xread(int fd, void *buf, ssize_t count);
unsigned char xread_char(int fd);
void check_header_gzip(int src_fd);

#endif
