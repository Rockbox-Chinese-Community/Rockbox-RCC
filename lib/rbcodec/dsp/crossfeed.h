/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2006 Thom Johansen
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
#ifndef CROSSFEED_H
#define CROSSFEED_H

enum crossfeed_type
{
    CROSSFEED_TYPE_NONE,
    CROSSFEED_TYPE_MEIER,
    CROSSFEED_TYPE_CUSTOM,
    CROSSFEED_TYPE_LNX,
    CROSSFEED_TYPE_LNX2,
    CROSSFEED_TYPE_LNX3,
    CROSSFEED_TYPE_LNX4,
    CROSSFEED_TYPE_LNX5,
    CROSSFEED_TYPE_LNX6,
    CROSSFEED_TYPE_LNX7,
};

void dsp_set_crossfeed_type(int type);
void dsp_set_crossfeed_direct_gain(int gain);
void dsp_set_crossfeed_cross_params(long lf_gain, long hf_gain, long cutoff);
void dsp_set_crossfeed_field(int level);
#endif /* CROSSFEED_H */
