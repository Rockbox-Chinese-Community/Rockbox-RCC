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
#ifndef ICOLL_IMX233_H
#define ICOLL_IMX233_H

#include "config.h"
#include "system.h"

#include "regs/regs-icoll.h"

#define INT_SRC_VDD5V       3
#define INT_SRC_DAC_DMA     5
#define INT_SRC_DAC_ERROR   6
#define INT_SRC_ADC_DMA     7
#define INT_SRC_ADC_ERROR   8
#define INT_SRC_USB_CTRL    11
#define INT_SRC_SSP1_DMA    14
#define INT_SRC_SSP1_ERROR  15
#define INT_SRC_GPIO0       16
#define INT_SRC_GPIO1       17
#define INT_SRC_GPIO2       18
#define INT_SRC_GPIO(i)     (INT_SRC_GPIO0 + (i))
#define INT_SRC_I2C_DMA     26
#define INT_SRC_I2C_ERROR   27
#define INT_SRC_TIMER(nr)   (28 + (nr))
#define INT_SRC_TOUCH_DETECT    36
#define INT_SRC_LRADC_CHx(x)    (37 + (x))
#define INT_SRC_RTC_1MSEC   48
#define INT_SRC_NR_SOURCES  64
/* STMP3700+ specific */
#if IMX233_SUBTARGET >= 3700
#define INT_SRC_SSP2_ERROR  2
#define INT_SRC_SSP2_DMA    20
#define INT_SRC_LCDIF_DMA   45
#define INT_SRC_LCDIF_ERROR 46
#define INT_SRC_DCP         54
#endif
/* STMP3780+ specific */
#if IMX233_SUBTARGET >= 3780

#endif

/* helpers */
#if IMX233_SUBTARGET >= 3600 && IMX233_SUBTARGET < 3780
#define BP_ICOLL_PRIORITYn_ENABLEx(x)   (2 + 8 * (x))
#define BM_ICOLL_PRIORITYn_ENABLEx(x)   (1 << (2 + 8 * (x)))
#endif

struct imx233_icoll_irq_info_t
{
    bool enabled;
    unsigned freq;
};

void imx233_icoll_init(void);
void imx233_icoll_enable_interrupt(int src, bool enable);
struct imx233_icoll_irq_info_t imx233_icoll_get_irq_info(int src);

#endif /* ICOLL_IMX233_H */
