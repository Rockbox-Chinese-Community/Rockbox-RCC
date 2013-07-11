/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * This file was automatically generated by headergen, DO NOT EDIT it.
 * headergen version: 2.1.7
 * XML versions: imx233:3.2.0
 *
 * Copyright (C) 2013 by Amaury Pouly
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
#ifndef __HEADERGEN__IMX233__PINCTRL__H__
#define __HEADERGEN__IMX233__PINCTRL__H__

#define REGS_PINCTRL_BASE (0x80018000)

#define REGS_PINCTRL_VERSION "3.2.0"

/**
 * Register: HW_PINCTRL_CTRL
 * Address: 0
 * SCT: yes
*/
#define HW_PINCTRL_CTRL             (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x0 + 0x0))
#define HW_PINCTRL_CTRL_SET         (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x0 + 0x4))
#define HW_PINCTRL_CTRL_CLR         (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x0 + 0x8))
#define HW_PINCTRL_CTRL_TOG         (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x0 + 0xc))
#define BP_PINCTRL_CTRL_SFTRST      31
#define BM_PINCTRL_CTRL_SFTRST      0x80000000
#define BF_PINCTRL_CTRL_SFTRST(v)   (((v) << 31) & 0x80000000)
#define BP_PINCTRL_CTRL_CLKGATE     30
#define BM_PINCTRL_CTRL_CLKGATE     0x40000000
#define BF_PINCTRL_CTRL_CLKGATE(v)  (((v) << 30) & 0x40000000)
#define BP_PINCTRL_CTRL_RSRVD2      28
#define BM_PINCTRL_CTRL_RSRVD2      0x30000000
#define BF_PINCTRL_CTRL_RSRVD2(v)   (((v) << 28) & 0x30000000)
#define BP_PINCTRL_CTRL_PRESENT3    27
#define BM_PINCTRL_CTRL_PRESENT3    0x8000000
#define BF_PINCTRL_CTRL_PRESENT3(v) (((v) << 27) & 0x8000000)
#define BP_PINCTRL_CTRL_PRESENT2    26
#define BM_PINCTRL_CTRL_PRESENT2    0x4000000
#define BF_PINCTRL_CTRL_PRESENT2(v) (((v) << 26) & 0x4000000)
#define BP_PINCTRL_CTRL_PRESENT1    25
#define BM_PINCTRL_CTRL_PRESENT1    0x2000000
#define BF_PINCTRL_CTRL_PRESENT1(v) (((v) << 25) & 0x2000000)
#define BP_PINCTRL_CTRL_PRESENT0    24
#define BM_PINCTRL_CTRL_PRESENT0    0x1000000
#define BF_PINCTRL_CTRL_PRESENT0(v) (((v) << 24) & 0x1000000)
#define BP_PINCTRL_CTRL_RSRVD1      3
#define BM_PINCTRL_CTRL_RSRVD1      0xfffff8
#define BF_PINCTRL_CTRL_RSRVD1(v)   (((v) << 3) & 0xfffff8)
#define BP_PINCTRL_CTRL_IRQOUT2     2
#define BM_PINCTRL_CTRL_IRQOUT2     0x4
#define BF_PINCTRL_CTRL_IRQOUT2(v)  (((v) << 2) & 0x4)
#define BP_PINCTRL_CTRL_IRQOUT1     1
#define BM_PINCTRL_CTRL_IRQOUT1     0x2
#define BF_PINCTRL_CTRL_IRQOUT1(v)  (((v) << 1) & 0x2)
#define BP_PINCTRL_CTRL_IRQOUT0     0
#define BM_PINCTRL_CTRL_IRQOUT0     0x1
#define BF_PINCTRL_CTRL_IRQOUT0(v)  (((v) << 0) & 0x1)

/**
 * Register: HW_PINCTRL_MUXSELn
 * Address: 0x100+n*0x10
 * SCT: yes
*/
#define HW_PINCTRL_MUXSELn(n)       (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x100+(n)*0x10 + 0x0))
#define HW_PINCTRL_MUXSELn_SET(n)   (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x100+(n)*0x10 + 0x4))
#define HW_PINCTRL_MUXSELn_CLR(n)   (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x100+(n)*0x10 + 0x8))
#define HW_PINCTRL_MUXSELn_TOG(n)   (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x100+(n)*0x10 + 0xc))
#define BP_PINCTRL_MUXSELn_BITS     0
#define BM_PINCTRL_MUXSELn_BITS     0xffffffff
#define BF_PINCTRL_MUXSELn_BITS(v)  (((v) << 0) & 0xffffffff)

/**
 * Register: HW_PINCTRL_DRIVEn
 * Address: 0x200+n*0x10
 * SCT: yes
*/
#define HW_PINCTRL_DRIVEn(n)        (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x200+(n)*0x10 + 0x0))
#define HW_PINCTRL_DRIVEn_SET(n)    (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x200+(n)*0x10 + 0x4))
#define HW_PINCTRL_DRIVEn_CLR(n)    (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x200+(n)*0x10 + 0x8))
#define HW_PINCTRL_DRIVEn_TOG(n)    (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x200+(n)*0x10 + 0xc))
#define BP_PINCTRL_DRIVEn_BITS      0
#define BM_PINCTRL_DRIVEn_BITS      0xffffffff
#define BF_PINCTRL_DRIVEn_BITS(v)   (((v) << 0) & 0xffffffff)

/**
 * Register: HW_PINCTRL_PULLn
 * Address: 0x400+n*0x10
 * SCT: yes
*/
#define HW_PINCTRL_PULLn(n)         (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x400+(n)*0x10 + 0x0))
#define HW_PINCTRL_PULLn_SET(n)     (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x400+(n)*0x10 + 0x4))
#define HW_PINCTRL_PULLn_CLR(n)     (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x400+(n)*0x10 + 0x8))
#define HW_PINCTRL_PULLn_TOG(n)     (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x400+(n)*0x10 + 0xc))
#define BP_PINCTRL_PULLn_BITS       0
#define BM_PINCTRL_PULLn_BITS       0xffffffff
#define BF_PINCTRL_PULLn_BITS(v)    (((v) << 0) & 0xffffffff)

/**
 * Register: HW_PINCTRL_DOUTn
 * Address: 0x500+n*0x10
 * SCT: yes
*/
#define HW_PINCTRL_DOUTn(n)         (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x500+(n)*0x10 + 0x0))
#define HW_PINCTRL_DOUTn_SET(n)     (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x500+(n)*0x10 + 0x4))
#define HW_PINCTRL_DOUTn_CLR(n)     (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x500+(n)*0x10 + 0x8))
#define HW_PINCTRL_DOUTn_TOG(n)     (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x500+(n)*0x10 + 0xc))
#define BP_PINCTRL_DOUTn_BITS       0
#define BM_PINCTRL_DOUTn_BITS       0xffffffff
#define BF_PINCTRL_DOUTn_BITS(v)    (((v) << 0) & 0xffffffff)

/**
 * Register: HW_PINCTRL_DINn
 * Address: 0x600+n*0x10
 * SCT: yes
*/
#define HW_PINCTRL_DINn(n)      (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x600+(n)*0x10 + 0x0))
#define HW_PINCTRL_DINn_SET(n)  (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x600+(n)*0x10 + 0x4))
#define HW_PINCTRL_DINn_CLR(n)  (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x600+(n)*0x10 + 0x8))
#define HW_PINCTRL_DINn_TOG(n)  (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x600+(n)*0x10 + 0xc))
#define BP_PINCTRL_DINn_BITS    0
#define BM_PINCTRL_DINn_BITS    0xffffffff
#define BF_PINCTRL_DINn_BITS(v) (((v) << 0) & 0xffffffff)

/**
 * Register: HW_PINCTRL_DOEn
 * Address: 0x700+n*0x10
 * SCT: yes
*/
#define HW_PINCTRL_DOEn(n)      (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x700+(n)*0x10 + 0x0))
#define HW_PINCTRL_DOEn_SET(n)  (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x700+(n)*0x10 + 0x4))
#define HW_PINCTRL_DOEn_CLR(n)  (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x700+(n)*0x10 + 0x8))
#define HW_PINCTRL_DOEn_TOG(n)  (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x700+(n)*0x10 + 0xc))
#define BP_PINCTRL_DOEn_BITS    0
#define BM_PINCTRL_DOEn_BITS    0xffffffff
#define BF_PINCTRL_DOEn_BITS(v) (((v) << 0) & 0xffffffff)

/**
 * Register: HW_PINCTRL_PIN2IRQn
 * Address: 0x800+n*0x10
 * SCT: yes
*/
#define HW_PINCTRL_PIN2IRQn(n)      (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x800+(n)*0x10 + 0x0))
#define HW_PINCTRL_PIN2IRQn_SET(n)  (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x800+(n)*0x10 + 0x4))
#define HW_PINCTRL_PIN2IRQn_CLR(n)  (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x800+(n)*0x10 + 0x8))
#define HW_PINCTRL_PIN2IRQn_TOG(n)  (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x800+(n)*0x10 + 0xc))
#define BP_PINCTRL_PIN2IRQn_BITS    0
#define BM_PINCTRL_PIN2IRQn_BITS    0xffffffff
#define BF_PINCTRL_PIN2IRQn_BITS(v) (((v) << 0) & 0xffffffff)

/**
 * Register: HW_PINCTRL_IRQENn
 * Address: 0x900+n*0x10
 * SCT: yes
*/
#define HW_PINCTRL_IRQENn(n)        (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x900+(n)*0x10 + 0x0))
#define HW_PINCTRL_IRQENn_SET(n)    (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x900+(n)*0x10 + 0x4))
#define HW_PINCTRL_IRQENn_CLR(n)    (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x900+(n)*0x10 + 0x8))
#define HW_PINCTRL_IRQENn_TOG(n)    (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0x900+(n)*0x10 + 0xc))
#define BP_PINCTRL_IRQENn_BITS      0
#define BM_PINCTRL_IRQENn_BITS      0xffffffff
#define BF_PINCTRL_IRQENn_BITS(v)   (((v) << 0) & 0xffffffff)

/**
 * Register: HW_PINCTRL_IRQLEVELn
 * Address: 0xa00+n*0x10
 * SCT: yes
*/
#define HW_PINCTRL_IRQLEVELn(n)         (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0xa00+(n)*0x10 + 0x0))
#define HW_PINCTRL_IRQLEVELn_SET(n)     (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0xa00+(n)*0x10 + 0x4))
#define HW_PINCTRL_IRQLEVELn_CLR(n)     (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0xa00+(n)*0x10 + 0x8))
#define HW_PINCTRL_IRQLEVELn_TOG(n)     (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0xa00+(n)*0x10 + 0xc))
#define BP_PINCTRL_IRQLEVELn_BITS       0
#define BM_PINCTRL_IRQLEVELn_BITS       0xffffffff
#define BF_PINCTRL_IRQLEVELn_BITS(v)    (((v) << 0) & 0xffffffff)

/**
 * Register: HW_PINCTRL_IRQPOLn
 * Address: 0xb00+n*0x10
 * SCT: yes
*/
#define HW_PINCTRL_IRQPOLn(n)       (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0xb00+(n)*0x10 + 0x0))
#define HW_PINCTRL_IRQPOLn_SET(n)   (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0xb00+(n)*0x10 + 0x4))
#define HW_PINCTRL_IRQPOLn_CLR(n)   (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0xb00+(n)*0x10 + 0x8))
#define HW_PINCTRL_IRQPOLn_TOG(n)   (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0xb00+(n)*0x10 + 0xc))
#define BP_PINCTRL_IRQPOLn_BITS     0
#define BM_PINCTRL_IRQPOLn_BITS     0xffffffff
#define BF_PINCTRL_IRQPOLn_BITS(v)  (((v) << 0) & 0xffffffff)

/**
 * Register: HW_PINCTRL_IRQSTATn
 * Address: 0xc00+n*0x10
 * SCT: yes
*/
#define HW_PINCTRL_IRQSTATn(n)      (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0xc00+(n)*0x10 + 0x0))
#define HW_PINCTRL_IRQSTATn_SET(n)  (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0xc00+(n)*0x10 + 0x4))
#define HW_PINCTRL_IRQSTATn_CLR(n)  (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0xc00+(n)*0x10 + 0x8))
#define HW_PINCTRL_IRQSTATn_TOG(n)  (*(volatile unsigned long *)(REGS_PINCTRL_BASE + 0xc00+(n)*0x10 + 0xc))
#define BP_PINCTRL_IRQSTATn_BITS    0
#define BM_PINCTRL_IRQSTATn_BITS    0xffffffff
#define BF_PINCTRL_IRQSTATn_BITS(v) (((v) << 0) & 0xffffffff)

#endif /* __HEADERGEN__IMX233__PINCTRL__H__ */
