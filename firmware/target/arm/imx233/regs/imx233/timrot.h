/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * This file was automatically generated by headergen, DO NOT EDIT it.
 * headergen version: 3.0.0
 * imx233 version: 2.4.0
 * imx233 authors: Amaury Pouly
 *
 * Copyright (C) 2015 by the authors
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
#ifndef __HEADERGEN_IMX233_TIMROT_H__
#define __HEADERGEN_IMX233_TIMROT_H__

#define HW_TIMROT_ROTCTRL                       HW(TIMROT_ROTCTRL)
#define HWA_TIMROT_ROTCTRL                      (0x80068000 + 0x0)
#define HWT_TIMROT_ROTCTRL                      HWIO_32_RW
#define HWN_TIMROT_ROTCTRL                      TIMROT_ROTCTRL
#define HWI_TIMROT_ROTCTRL                      
#define HW_TIMROT_ROTCTRL_SET                   HW(TIMROT_ROTCTRL_SET)
#define HWA_TIMROT_ROTCTRL_SET                  (HWA_TIMROT_ROTCTRL + 0x4)
#define HWT_TIMROT_ROTCTRL_SET                  HWIO_32_WO
#define HWN_TIMROT_ROTCTRL_SET                  TIMROT_ROTCTRL
#define HWI_TIMROT_ROTCTRL_SET                  
#define HW_TIMROT_ROTCTRL_CLR                   HW(TIMROT_ROTCTRL_CLR)
#define HWA_TIMROT_ROTCTRL_CLR                  (HWA_TIMROT_ROTCTRL + 0x8)
#define HWT_TIMROT_ROTCTRL_CLR                  HWIO_32_WO
#define HWN_TIMROT_ROTCTRL_CLR                  TIMROT_ROTCTRL
#define HWI_TIMROT_ROTCTRL_CLR                  
#define HW_TIMROT_ROTCTRL_TOG                   HW(TIMROT_ROTCTRL_TOG)
#define HWA_TIMROT_ROTCTRL_TOG                  (HWA_TIMROT_ROTCTRL + 0xc)
#define HWT_TIMROT_ROTCTRL_TOG                  HWIO_32_WO
#define HWN_TIMROT_ROTCTRL_TOG                  TIMROT_ROTCTRL
#define HWI_TIMROT_ROTCTRL_TOG                  
#define BP_TIMROT_ROTCTRL_SFTRST                31
#define BM_TIMROT_ROTCTRL_SFTRST                0x80000000
#define BF_TIMROT_ROTCTRL_SFTRST(v)             (((v) & 0x1) << 31)
#define BFM_TIMROT_ROTCTRL_SFTRST(v)            BM_TIMROT_ROTCTRL_SFTRST
#define BF_TIMROT_ROTCTRL_SFTRST_V(e)           BF_TIMROT_ROTCTRL_SFTRST(BV_TIMROT_ROTCTRL_SFTRST__##e)
#define BFM_TIMROT_ROTCTRL_SFTRST_V(v)          BM_TIMROT_ROTCTRL_SFTRST
#define BP_TIMROT_ROTCTRL_CLKGATE               30
#define BM_TIMROT_ROTCTRL_CLKGATE               0x40000000
#define BF_TIMROT_ROTCTRL_CLKGATE(v)            (((v) & 0x1) << 30)
#define BFM_TIMROT_ROTCTRL_CLKGATE(v)           BM_TIMROT_ROTCTRL_CLKGATE
#define BF_TIMROT_ROTCTRL_CLKGATE_V(e)          BF_TIMROT_ROTCTRL_CLKGATE(BV_TIMROT_ROTCTRL_CLKGATE__##e)
#define BFM_TIMROT_ROTCTRL_CLKGATE_V(v)         BM_TIMROT_ROTCTRL_CLKGATE
#define BP_TIMROT_ROTCTRL_ROTARY_PRESENT        29
#define BM_TIMROT_ROTCTRL_ROTARY_PRESENT        0x20000000
#define BF_TIMROT_ROTCTRL_ROTARY_PRESENT(v)     (((v) & 0x1) << 29)
#define BFM_TIMROT_ROTCTRL_ROTARY_PRESENT(v)    BM_TIMROT_ROTCTRL_ROTARY_PRESENT
#define BF_TIMROT_ROTCTRL_ROTARY_PRESENT_V(e)   BF_TIMROT_ROTCTRL_ROTARY_PRESENT(BV_TIMROT_ROTCTRL_ROTARY_PRESENT__##e)
#define BFM_TIMROT_ROTCTRL_ROTARY_PRESENT_V(v)  BM_TIMROT_ROTCTRL_ROTARY_PRESENT
#define BP_TIMROT_ROTCTRL_TIM3_PRESENT          28
#define BM_TIMROT_ROTCTRL_TIM3_PRESENT          0x10000000
#define BF_TIMROT_ROTCTRL_TIM3_PRESENT(v)       (((v) & 0x1) << 28)
#define BFM_TIMROT_ROTCTRL_TIM3_PRESENT(v)      BM_TIMROT_ROTCTRL_TIM3_PRESENT
#define BF_TIMROT_ROTCTRL_TIM3_PRESENT_V(e)     BF_TIMROT_ROTCTRL_TIM3_PRESENT(BV_TIMROT_ROTCTRL_TIM3_PRESENT__##e)
#define BFM_TIMROT_ROTCTRL_TIM3_PRESENT_V(v)    BM_TIMROT_ROTCTRL_TIM3_PRESENT
#define BP_TIMROT_ROTCTRL_TIM2_PRESENT          27
#define BM_TIMROT_ROTCTRL_TIM2_PRESENT          0x8000000
#define BF_TIMROT_ROTCTRL_TIM2_PRESENT(v)       (((v) & 0x1) << 27)
#define BFM_TIMROT_ROTCTRL_TIM2_PRESENT(v)      BM_TIMROT_ROTCTRL_TIM2_PRESENT
#define BF_TIMROT_ROTCTRL_TIM2_PRESENT_V(e)     BF_TIMROT_ROTCTRL_TIM2_PRESENT(BV_TIMROT_ROTCTRL_TIM2_PRESENT__##e)
#define BFM_TIMROT_ROTCTRL_TIM2_PRESENT_V(v)    BM_TIMROT_ROTCTRL_TIM2_PRESENT
#define BP_TIMROT_ROTCTRL_TIM1_PRESENT          26
#define BM_TIMROT_ROTCTRL_TIM1_PRESENT          0x4000000
#define BF_TIMROT_ROTCTRL_TIM1_PRESENT(v)       (((v) & 0x1) << 26)
#define BFM_TIMROT_ROTCTRL_TIM1_PRESENT(v)      BM_TIMROT_ROTCTRL_TIM1_PRESENT
#define BF_TIMROT_ROTCTRL_TIM1_PRESENT_V(e)     BF_TIMROT_ROTCTRL_TIM1_PRESENT(BV_TIMROT_ROTCTRL_TIM1_PRESENT__##e)
#define BFM_TIMROT_ROTCTRL_TIM1_PRESENT_V(v)    BM_TIMROT_ROTCTRL_TIM1_PRESENT
#define BP_TIMROT_ROTCTRL_TIM0_PRESENT          25
#define BM_TIMROT_ROTCTRL_TIM0_PRESENT          0x2000000
#define BF_TIMROT_ROTCTRL_TIM0_PRESENT(v)       (((v) & 0x1) << 25)
#define BFM_TIMROT_ROTCTRL_TIM0_PRESENT(v)      BM_TIMROT_ROTCTRL_TIM0_PRESENT
#define BF_TIMROT_ROTCTRL_TIM0_PRESENT_V(e)     BF_TIMROT_ROTCTRL_TIM0_PRESENT(BV_TIMROT_ROTCTRL_TIM0_PRESENT__##e)
#define BFM_TIMROT_ROTCTRL_TIM0_PRESENT_V(v)    BM_TIMROT_ROTCTRL_TIM0_PRESENT
#define BP_TIMROT_ROTCTRL_STATE                 22
#define BM_TIMROT_ROTCTRL_STATE                 0x1c00000
#define BF_TIMROT_ROTCTRL_STATE(v)              (((v) & 0x7) << 22)
#define BFM_TIMROT_ROTCTRL_STATE(v)             BM_TIMROT_ROTCTRL_STATE
#define BF_TIMROT_ROTCTRL_STATE_V(e)            BF_TIMROT_ROTCTRL_STATE(BV_TIMROT_ROTCTRL_STATE__##e)
#define BFM_TIMROT_ROTCTRL_STATE_V(v)           BM_TIMROT_ROTCTRL_STATE
#define BP_TIMROT_ROTCTRL_DIVIDER               16
#define BM_TIMROT_ROTCTRL_DIVIDER               0x3f0000
#define BF_TIMROT_ROTCTRL_DIVIDER(v)            (((v) & 0x3f) << 16)
#define BFM_TIMROT_ROTCTRL_DIVIDER(v)           BM_TIMROT_ROTCTRL_DIVIDER
#define BF_TIMROT_ROTCTRL_DIVIDER_V(e)          BF_TIMROT_ROTCTRL_DIVIDER(BV_TIMROT_ROTCTRL_DIVIDER__##e)
#define BFM_TIMROT_ROTCTRL_DIVIDER_V(v)         BM_TIMROT_ROTCTRL_DIVIDER
#define BP_TIMROT_ROTCTRL_RSRVD3                13
#define BM_TIMROT_ROTCTRL_RSRVD3                0xe000
#define BF_TIMROT_ROTCTRL_RSRVD3(v)             (((v) & 0x7) << 13)
#define BFM_TIMROT_ROTCTRL_RSRVD3(v)            BM_TIMROT_ROTCTRL_RSRVD3
#define BF_TIMROT_ROTCTRL_RSRVD3_V(e)           BF_TIMROT_ROTCTRL_RSRVD3(BV_TIMROT_ROTCTRL_RSRVD3__##e)
#define BFM_TIMROT_ROTCTRL_RSRVD3_V(v)          BM_TIMROT_ROTCTRL_RSRVD3
#define BP_TIMROT_ROTCTRL_RELATIVE              12
#define BM_TIMROT_ROTCTRL_RELATIVE              0x1000
#define BF_TIMROT_ROTCTRL_RELATIVE(v)           (((v) & 0x1) << 12)
#define BFM_TIMROT_ROTCTRL_RELATIVE(v)          BM_TIMROT_ROTCTRL_RELATIVE
#define BF_TIMROT_ROTCTRL_RELATIVE_V(e)         BF_TIMROT_ROTCTRL_RELATIVE(BV_TIMROT_ROTCTRL_RELATIVE__##e)
#define BFM_TIMROT_ROTCTRL_RELATIVE_V(v)        BM_TIMROT_ROTCTRL_RELATIVE
#define BP_TIMROT_ROTCTRL_OVERSAMPLE            10
#define BM_TIMROT_ROTCTRL_OVERSAMPLE            0xc00
#define BV_TIMROT_ROTCTRL_OVERSAMPLE__8X        0x0
#define BV_TIMROT_ROTCTRL_OVERSAMPLE__4X        0x1
#define BV_TIMROT_ROTCTRL_OVERSAMPLE__2X        0x2
#define BV_TIMROT_ROTCTRL_OVERSAMPLE__1X        0x3
#define BF_TIMROT_ROTCTRL_OVERSAMPLE(v)         (((v) & 0x3) << 10)
#define BFM_TIMROT_ROTCTRL_OVERSAMPLE(v)        BM_TIMROT_ROTCTRL_OVERSAMPLE
#define BF_TIMROT_ROTCTRL_OVERSAMPLE_V(e)       BF_TIMROT_ROTCTRL_OVERSAMPLE(BV_TIMROT_ROTCTRL_OVERSAMPLE__##e)
#define BFM_TIMROT_ROTCTRL_OVERSAMPLE_V(v)      BM_TIMROT_ROTCTRL_OVERSAMPLE
#define BP_TIMROT_ROTCTRL_POLARITY_B            9
#define BM_TIMROT_ROTCTRL_POLARITY_B            0x200
#define BF_TIMROT_ROTCTRL_POLARITY_B(v)         (((v) & 0x1) << 9)
#define BFM_TIMROT_ROTCTRL_POLARITY_B(v)        BM_TIMROT_ROTCTRL_POLARITY_B
#define BF_TIMROT_ROTCTRL_POLARITY_B_V(e)       BF_TIMROT_ROTCTRL_POLARITY_B(BV_TIMROT_ROTCTRL_POLARITY_B__##e)
#define BFM_TIMROT_ROTCTRL_POLARITY_B_V(v)      BM_TIMROT_ROTCTRL_POLARITY_B
#define BP_TIMROT_ROTCTRL_POLARITY_A            8
#define BM_TIMROT_ROTCTRL_POLARITY_A            0x100
#define BF_TIMROT_ROTCTRL_POLARITY_A(v)         (((v) & 0x1) << 8)
#define BFM_TIMROT_ROTCTRL_POLARITY_A(v)        BM_TIMROT_ROTCTRL_POLARITY_A
#define BF_TIMROT_ROTCTRL_POLARITY_A_V(e)       BF_TIMROT_ROTCTRL_POLARITY_A(BV_TIMROT_ROTCTRL_POLARITY_A__##e)
#define BFM_TIMROT_ROTCTRL_POLARITY_A_V(v)      BM_TIMROT_ROTCTRL_POLARITY_A
#define BP_TIMROT_ROTCTRL_RSRVD2                7
#define BM_TIMROT_ROTCTRL_RSRVD2                0x80
#define BF_TIMROT_ROTCTRL_RSRVD2(v)             (((v) & 0x1) << 7)
#define BFM_TIMROT_ROTCTRL_RSRVD2(v)            BM_TIMROT_ROTCTRL_RSRVD2
#define BF_TIMROT_ROTCTRL_RSRVD2_V(e)           BF_TIMROT_ROTCTRL_RSRVD2(BV_TIMROT_ROTCTRL_RSRVD2__##e)
#define BFM_TIMROT_ROTCTRL_RSRVD2_V(v)          BM_TIMROT_ROTCTRL_RSRVD2
#define BP_TIMROT_ROTCTRL_SELECT_B              4
#define BM_TIMROT_ROTCTRL_SELECT_B              0x70
#define BV_TIMROT_ROTCTRL_SELECT_B__NEVER_TICK  0x0
#define BV_TIMROT_ROTCTRL_SELECT_B__PWM0        0x1
#define BV_TIMROT_ROTCTRL_SELECT_B__PWM1        0x2
#define BV_TIMROT_ROTCTRL_SELECT_B__PWM2        0x3
#define BV_TIMROT_ROTCTRL_SELECT_B__PWM3        0x4
#define BV_TIMROT_ROTCTRL_SELECT_B__PWM4        0x5
#define BV_TIMROT_ROTCTRL_SELECT_B__ROTARYA     0x6
#define BV_TIMROT_ROTCTRL_SELECT_B__ROTARYB     0x7
#define BF_TIMROT_ROTCTRL_SELECT_B(v)           (((v) & 0x7) << 4)
#define BFM_TIMROT_ROTCTRL_SELECT_B(v)          BM_TIMROT_ROTCTRL_SELECT_B
#define BF_TIMROT_ROTCTRL_SELECT_B_V(e)         BF_TIMROT_ROTCTRL_SELECT_B(BV_TIMROT_ROTCTRL_SELECT_B__##e)
#define BFM_TIMROT_ROTCTRL_SELECT_B_V(v)        BM_TIMROT_ROTCTRL_SELECT_B
#define BP_TIMROT_ROTCTRL_RSRVD1                3
#define BM_TIMROT_ROTCTRL_RSRVD1                0x8
#define BF_TIMROT_ROTCTRL_RSRVD1(v)             (((v) & 0x1) << 3)
#define BFM_TIMROT_ROTCTRL_RSRVD1(v)            BM_TIMROT_ROTCTRL_RSRVD1
#define BF_TIMROT_ROTCTRL_RSRVD1_V(e)           BF_TIMROT_ROTCTRL_RSRVD1(BV_TIMROT_ROTCTRL_RSRVD1__##e)
#define BFM_TIMROT_ROTCTRL_RSRVD1_V(v)          BM_TIMROT_ROTCTRL_RSRVD1
#define BP_TIMROT_ROTCTRL_SELECT_A              0
#define BM_TIMROT_ROTCTRL_SELECT_A              0x7
#define BV_TIMROT_ROTCTRL_SELECT_A__NEVER_TICK  0x0
#define BV_TIMROT_ROTCTRL_SELECT_A__PWM0        0x1
#define BV_TIMROT_ROTCTRL_SELECT_A__PWM1        0x2
#define BV_TIMROT_ROTCTRL_SELECT_A__PWM2        0x3
#define BV_TIMROT_ROTCTRL_SELECT_A__PWM3        0x4
#define BV_TIMROT_ROTCTRL_SELECT_A__PWM4        0x5
#define BV_TIMROT_ROTCTRL_SELECT_A__ROTARYA     0x6
#define BV_TIMROT_ROTCTRL_SELECT_A__ROTARYB     0x7
#define BF_TIMROT_ROTCTRL_SELECT_A(v)           (((v) & 0x7) << 0)
#define BFM_TIMROT_ROTCTRL_SELECT_A(v)          BM_TIMROT_ROTCTRL_SELECT_A
#define BF_TIMROT_ROTCTRL_SELECT_A_V(e)         BF_TIMROT_ROTCTRL_SELECT_A(BV_TIMROT_ROTCTRL_SELECT_A__##e)
#define BFM_TIMROT_ROTCTRL_SELECT_A_V(v)        BM_TIMROT_ROTCTRL_SELECT_A

#define HW_TIMROT_ROTCOUNT              HW(TIMROT_ROTCOUNT)
#define HWA_TIMROT_ROTCOUNT             (0x80068000 + 0x10)
#define HWT_TIMROT_ROTCOUNT             HWIO_32_RW
#define HWN_TIMROT_ROTCOUNT             TIMROT_ROTCOUNT
#define HWI_TIMROT_ROTCOUNT             
#define BP_TIMROT_ROTCOUNT_RSRVD1       16
#define BM_TIMROT_ROTCOUNT_RSRVD1       0xffff0000
#define BF_TIMROT_ROTCOUNT_RSRVD1(v)    (((v) & 0xffff) << 16)
#define BFM_TIMROT_ROTCOUNT_RSRVD1(v)   BM_TIMROT_ROTCOUNT_RSRVD1
#define BF_TIMROT_ROTCOUNT_RSRVD1_V(e)  BF_TIMROT_ROTCOUNT_RSRVD1(BV_TIMROT_ROTCOUNT_RSRVD1__##e)
#define BFM_TIMROT_ROTCOUNT_RSRVD1_V(v) BM_TIMROT_ROTCOUNT_RSRVD1
#define BP_TIMROT_ROTCOUNT_UPDOWN       0
#define BM_TIMROT_ROTCOUNT_UPDOWN       0xffff
#define BF_TIMROT_ROTCOUNT_UPDOWN(v)    (((v) & 0xffff) << 0)
#define BFM_TIMROT_ROTCOUNT_UPDOWN(v)   BM_TIMROT_ROTCOUNT_UPDOWN
#define BF_TIMROT_ROTCOUNT_UPDOWN_V(e)  BF_TIMROT_ROTCOUNT_UPDOWN(BV_TIMROT_ROTCOUNT_UPDOWN__##e)
#define BFM_TIMROT_ROTCOUNT_UPDOWN_V(v) BM_TIMROT_ROTCOUNT_UPDOWN

#define HW_TIMROT_TIMCTRLn(_n1)                 HW(TIMROT_TIMCTRLn(_n1))
#define HWA_TIMROT_TIMCTRLn(_n1)                (0x80068000 + 0x20 + (_n1) * 0x20)
#define HWT_TIMROT_TIMCTRLn(_n1)                HWIO_32_RW
#define HWN_TIMROT_TIMCTRLn(_n1)                TIMROT_TIMCTRLn
#define HWI_TIMROT_TIMCTRLn(_n1)                (_n1)
#define HW_TIMROT_TIMCTRLn_SET(_n1)             HW(TIMROT_TIMCTRLn_SET(_n1))
#define HWA_TIMROT_TIMCTRLn_SET(_n1)            (HWA_TIMROT_TIMCTRLn(_n1) + 0x4)
#define HWT_TIMROT_TIMCTRLn_SET(_n1)            HWIO_32_WO
#define HWN_TIMROT_TIMCTRLn_SET(_n1)            TIMROT_TIMCTRLn
#define HWI_TIMROT_TIMCTRLn_SET(_n1)            (_n1)
#define HW_TIMROT_TIMCTRLn_CLR(_n1)             HW(TIMROT_TIMCTRLn_CLR(_n1))
#define HWA_TIMROT_TIMCTRLn_CLR(_n1)            (HWA_TIMROT_TIMCTRLn(_n1) + 0x8)
#define HWT_TIMROT_TIMCTRLn_CLR(_n1)            HWIO_32_WO
#define HWN_TIMROT_TIMCTRLn_CLR(_n1)            TIMROT_TIMCTRLn
#define HWI_TIMROT_TIMCTRLn_CLR(_n1)            (_n1)
#define HW_TIMROT_TIMCTRLn_TOG(_n1)             HW(TIMROT_TIMCTRLn_TOG(_n1))
#define HWA_TIMROT_TIMCTRLn_TOG(_n1)            (HWA_TIMROT_TIMCTRLn(_n1) + 0xc)
#define HWT_TIMROT_TIMCTRLn_TOG(_n1)            HWIO_32_WO
#define HWN_TIMROT_TIMCTRLn_TOG(_n1)            TIMROT_TIMCTRLn
#define HWI_TIMROT_TIMCTRLn_TOG(_n1)            (_n1)
#define BP_TIMROT_TIMCTRLn_RSRVD2               16
#define BM_TIMROT_TIMCTRLn_RSRVD2               0xffff0000
#define BF_TIMROT_TIMCTRLn_RSRVD2(v)            (((v) & 0xffff) << 16)
#define BFM_TIMROT_TIMCTRLn_RSRVD2(v)           BM_TIMROT_TIMCTRLn_RSRVD2
#define BF_TIMROT_TIMCTRLn_RSRVD2_V(e)          BF_TIMROT_TIMCTRLn_RSRVD2(BV_TIMROT_TIMCTRLn_RSRVD2__##e)
#define BFM_TIMROT_TIMCTRLn_RSRVD2_V(v)         BM_TIMROT_TIMCTRLn_RSRVD2
#define BP_TIMROT_TIMCTRLn_IRQ                  15
#define BM_TIMROT_TIMCTRLn_IRQ                  0x8000
#define BF_TIMROT_TIMCTRLn_IRQ(v)               (((v) & 0x1) << 15)
#define BFM_TIMROT_TIMCTRLn_IRQ(v)              BM_TIMROT_TIMCTRLn_IRQ
#define BF_TIMROT_TIMCTRLn_IRQ_V(e)             BF_TIMROT_TIMCTRLn_IRQ(BV_TIMROT_TIMCTRLn_IRQ__##e)
#define BFM_TIMROT_TIMCTRLn_IRQ_V(v)            BM_TIMROT_TIMCTRLn_IRQ
#define BP_TIMROT_TIMCTRLn_IRQ_EN               14
#define BM_TIMROT_TIMCTRLn_IRQ_EN               0x4000
#define BF_TIMROT_TIMCTRLn_IRQ_EN(v)            (((v) & 0x1) << 14)
#define BFM_TIMROT_TIMCTRLn_IRQ_EN(v)           BM_TIMROT_TIMCTRLn_IRQ_EN
#define BF_TIMROT_TIMCTRLn_IRQ_EN_V(e)          BF_TIMROT_TIMCTRLn_IRQ_EN(BV_TIMROT_TIMCTRLn_IRQ_EN__##e)
#define BFM_TIMROT_TIMCTRLn_IRQ_EN_V(v)         BM_TIMROT_TIMCTRLn_IRQ_EN
#define BP_TIMROT_TIMCTRLn_RSRVD1               9
#define BM_TIMROT_TIMCTRLn_RSRVD1               0x3e00
#define BF_TIMROT_TIMCTRLn_RSRVD1(v)            (((v) & 0x1f) << 9)
#define BFM_TIMROT_TIMCTRLn_RSRVD1(v)           BM_TIMROT_TIMCTRLn_RSRVD1
#define BF_TIMROT_TIMCTRLn_RSRVD1_V(e)          BF_TIMROT_TIMCTRLn_RSRVD1(BV_TIMROT_TIMCTRLn_RSRVD1__##e)
#define BFM_TIMROT_TIMCTRLn_RSRVD1_V(v)         BM_TIMROT_TIMCTRLn_RSRVD1
#define BP_TIMROT_TIMCTRLn_POLARITY             8
#define BM_TIMROT_TIMCTRLn_POLARITY             0x100
#define BF_TIMROT_TIMCTRLn_POLARITY(v)          (((v) & 0x1) << 8)
#define BFM_TIMROT_TIMCTRLn_POLARITY(v)         BM_TIMROT_TIMCTRLn_POLARITY
#define BF_TIMROT_TIMCTRLn_POLARITY_V(e)        BF_TIMROT_TIMCTRLn_POLARITY(BV_TIMROT_TIMCTRLn_POLARITY__##e)
#define BFM_TIMROT_TIMCTRLn_POLARITY_V(v)       BM_TIMROT_TIMCTRLn_POLARITY
#define BP_TIMROT_TIMCTRLn_UPDATE               7
#define BM_TIMROT_TIMCTRLn_UPDATE               0x80
#define BF_TIMROT_TIMCTRLn_UPDATE(v)            (((v) & 0x1) << 7)
#define BFM_TIMROT_TIMCTRLn_UPDATE(v)           BM_TIMROT_TIMCTRLn_UPDATE
#define BF_TIMROT_TIMCTRLn_UPDATE_V(e)          BF_TIMROT_TIMCTRLn_UPDATE(BV_TIMROT_TIMCTRLn_UPDATE__##e)
#define BFM_TIMROT_TIMCTRLn_UPDATE_V(v)         BM_TIMROT_TIMCTRLn_UPDATE
#define BP_TIMROT_TIMCTRLn_RELOAD               6
#define BM_TIMROT_TIMCTRLn_RELOAD               0x40
#define BF_TIMROT_TIMCTRLn_RELOAD(v)            (((v) & 0x1) << 6)
#define BFM_TIMROT_TIMCTRLn_RELOAD(v)           BM_TIMROT_TIMCTRLn_RELOAD
#define BF_TIMROT_TIMCTRLn_RELOAD_V(e)          BF_TIMROT_TIMCTRLn_RELOAD(BV_TIMROT_TIMCTRLn_RELOAD__##e)
#define BFM_TIMROT_TIMCTRLn_RELOAD_V(v)         BM_TIMROT_TIMCTRLn_RELOAD
#define BP_TIMROT_TIMCTRLn_PRESCALE             4
#define BM_TIMROT_TIMCTRLn_PRESCALE             0x30
#define BV_TIMROT_TIMCTRLn_PRESCALE__DIV_BY_1   0x0
#define BV_TIMROT_TIMCTRLn_PRESCALE__DIV_BY_2   0x1
#define BV_TIMROT_TIMCTRLn_PRESCALE__DIV_BY_4   0x2
#define BV_TIMROT_TIMCTRLn_PRESCALE__DIV_BY_8   0x3
#define BF_TIMROT_TIMCTRLn_PRESCALE(v)          (((v) & 0x3) << 4)
#define BFM_TIMROT_TIMCTRLn_PRESCALE(v)         BM_TIMROT_TIMCTRLn_PRESCALE
#define BF_TIMROT_TIMCTRLn_PRESCALE_V(e)        BF_TIMROT_TIMCTRLn_PRESCALE(BV_TIMROT_TIMCTRLn_PRESCALE__##e)
#define BFM_TIMROT_TIMCTRLn_PRESCALE_V(v)       BM_TIMROT_TIMCTRLn_PRESCALE
#define BP_TIMROT_TIMCTRLn_SELECT               0
#define BM_TIMROT_TIMCTRLn_SELECT               0xf
#define BV_TIMROT_TIMCTRLn_SELECT__NEVER_TICK   0x0
#define BV_TIMROT_TIMCTRLn_SELECT__PWM0         0x1
#define BV_TIMROT_TIMCTRLn_SELECT__PWM1         0x2
#define BV_TIMROT_TIMCTRLn_SELECT__PWM2         0x3
#define BV_TIMROT_TIMCTRLn_SELECT__PWM3         0x4
#define BV_TIMROT_TIMCTRLn_SELECT__PWM4         0x5
#define BV_TIMROT_TIMCTRLn_SELECT__ROTARYA      0x6
#define BV_TIMROT_TIMCTRLn_SELECT__ROTARYB      0x7
#define BV_TIMROT_TIMCTRLn_SELECT__32KHZ_XTAL   0x8
#define BV_TIMROT_TIMCTRLn_SELECT__8KHZ_XTAL    0x9
#define BV_TIMROT_TIMCTRLn_SELECT__4KHZ_XTAL    0xa
#define BV_TIMROT_TIMCTRLn_SELECT__1KHZ_XTAL    0xb
#define BV_TIMROT_TIMCTRLn_SELECT__TICK_ALWAYS  0xc
#define BF_TIMROT_TIMCTRLn_SELECT(v)            (((v) & 0xf) << 0)
#define BFM_TIMROT_TIMCTRLn_SELECT(v)           BM_TIMROT_TIMCTRLn_SELECT
#define BF_TIMROT_TIMCTRLn_SELECT_V(e)          BF_TIMROT_TIMCTRLn_SELECT(BV_TIMROT_TIMCTRLn_SELECT__##e)
#define BFM_TIMROT_TIMCTRLn_SELECT_V(v)         BM_TIMROT_TIMCTRLn_SELECT

#define HW_TIMROT_TIMCOUNTn(_n1)                HW(TIMROT_TIMCOUNTn(_n1))
#define HWA_TIMROT_TIMCOUNTn(_n1)               (0x80068000 + 0x30 + (_n1) * 0x20)
#define HWT_TIMROT_TIMCOUNTn(_n1)               HWIO_32_RW
#define HWN_TIMROT_TIMCOUNTn(_n1)               TIMROT_TIMCOUNTn
#define HWI_TIMROT_TIMCOUNTn(_n1)               (_n1)
#define BP_TIMROT_TIMCOUNTn_RUNNING_COUNT       16
#define BM_TIMROT_TIMCOUNTn_RUNNING_COUNT       0xffff0000
#define BF_TIMROT_TIMCOUNTn_RUNNING_COUNT(v)    (((v) & 0xffff) << 16)
#define BFM_TIMROT_TIMCOUNTn_RUNNING_COUNT(v)   BM_TIMROT_TIMCOUNTn_RUNNING_COUNT
#define BF_TIMROT_TIMCOUNTn_RUNNING_COUNT_V(e)  BF_TIMROT_TIMCOUNTn_RUNNING_COUNT(BV_TIMROT_TIMCOUNTn_RUNNING_COUNT__##e)
#define BFM_TIMROT_TIMCOUNTn_RUNNING_COUNT_V(v) BM_TIMROT_TIMCOUNTn_RUNNING_COUNT
#define BP_TIMROT_TIMCOUNTn_FIXED_COUNT         0
#define BM_TIMROT_TIMCOUNTn_FIXED_COUNT         0xffff
#define BF_TIMROT_TIMCOUNTn_FIXED_COUNT(v)      (((v) & 0xffff) << 0)
#define BFM_TIMROT_TIMCOUNTn_FIXED_COUNT(v)     BM_TIMROT_TIMCOUNTn_FIXED_COUNT
#define BF_TIMROT_TIMCOUNTn_FIXED_COUNT_V(e)    BF_TIMROT_TIMCOUNTn_FIXED_COUNT(BV_TIMROT_TIMCOUNTn_FIXED_COUNT__##e)
#define BFM_TIMROT_TIMCOUNTn_FIXED_COUNT_V(v)   BM_TIMROT_TIMCOUNTn_FIXED_COUNT

#define HW_TIMROT_TIMCTRL3                          HW(TIMROT_TIMCTRL3)
#define HWA_TIMROT_TIMCTRL3                         (0x80068000 + 0x80)
#define HWT_TIMROT_TIMCTRL3                         HWIO_32_RW
#define HWN_TIMROT_TIMCTRL3                         TIMROT_TIMCTRL3
#define HWI_TIMROT_TIMCTRL3                         
#define HW_TIMROT_TIMCTRL3_SET                      HW(TIMROT_TIMCTRL3_SET)
#define HWA_TIMROT_TIMCTRL3_SET                     (HWA_TIMROT_TIMCTRL3 + 0x4)
#define HWT_TIMROT_TIMCTRL3_SET                     HWIO_32_WO
#define HWN_TIMROT_TIMCTRL3_SET                     TIMROT_TIMCTRL3
#define HWI_TIMROT_TIMCTRL3_SET                     
#define HW_TIMROT_TIMCTRL3_CLR                      HW(TIMROT_TIMCTRL3_CLR)
#define HWA_TIMROT_TIMCTRL3_CLR                     (HWA_TIMROT_TIMCTRL3 + 0x8)
#define HWT_TIMROT_TIMCTRL3_CLR                     HWIO_32_WO
#define HWN_TIMROT_TIMCTRL3_CLR                     TIMROT_TIMCTRL3
#define HWI_TIMROT_TIMCTRL3_CLR                     
#define HW_TIMROT_TIMCTRL3_TOG                      HW(TIMROT_TIMCTRL3_TOG)
#define HWA_TIMROT_TIMCTRL3_TOG                     (HWA_TIMROT_TIMCTRL3 + 0xc)
#define HWT_TIMROT_TIMCTRL3_TOG                     HWIO_32_WO
#define HWN_TIMROT_TIMCTRL3_TOG                     TIMROT_TIMCTRL3
#define HWI_TIMROT_TIMCTRL3_TOG                     
#define BP_TIMROT_TIMCTRL3_RSRVD2                   20
#define BM_TIMROT_TIMCTRL3_RSRVD2                   0xfff00000
#define BF_TIMROT_TIMCTRL3_RSRVD2(v)                (((v) & 0xfff) << 20)
#define BFM_TIMROT_TIMCTRL3_RSRVD2(v)               BM_TIMROT_TIMCTRL3_RSRVD2
#define BF_TIMROT_TIMCTRL3_RSRVD2_V(e)              BF_TIMROT_TIMCTRL3_RSRVD2(BV_TIMROT_TIMCTRL3_RSRVD2__##e)
#define BFM_TIMROT_TIMCTRL3_RSRVD2_V(v)             BM_TIMROT_TIMCTRL3_RSRVD2
#define BP_TIMROT_TIMCTRL3_TEST_SIGNAL              16
#define BM_TIMROT_TIMCTRL3_TEST_SIGNAL              0xf0000
#define BV_TIMROT_TIMCTRL3_TEST_SIGNAL__NEVER_TICK  0x0
#define BV_TIMROT_TIMCTRL3_TEST_SIGNAL__PWM0        0x1
#define BV_TIMROT_TIMCTRL3_TEST_SIGNAL__PWM1        0x2
#define BV_TIMROT_TIMCTRL3_TEST_SIGNAL__PWM2        0x3
#define BV_TIMROT_TIMCTRL3_TEST_SIGNAL__PWM3        0x4
#define BV_TIMROT_TIMCTRL3_TEST_SIGNAL__PWM4        0x5
#define BV_TIMROT_TIMCTRL3_TEST_SIGNAL__ROTARYA     0x6
#define BV_TIMROT_TIMCTRL3_TEST_SIGNAL__ROTARYB     0x7
#define BV_TIMROT_TIMCTRL3_TEST_SIGNAL__32KHZ_XTAL  0x8
#define BV_TIMROT_TIMCTRL3_TEST_SIGNAL__8KHZ_XTAL   0x9
#define BV_TIMROT_TIMCTRL3_TEST_SIGNAL__4KHZ_XTAL   0xa
#define BV_TIMROT_TIMCTRL3_TEST_SIGNAL__1KHZ_XTAL   0xb
#define BV_TIMROT_TIMCTRL3_TEST_SIGNAL__TICK_ALWAYS 0xc
#define BF_TIMROT_TIMCTRL3_TEST_SIGNAL(v)           (((v) & 0xf) << 16)
#define BFM_TIMROT_TIMCTRL3_TEST_SIGNAL(v)          BM_TIMROT_TIMCTRL3_TEST_SIGNAL
#define BF_TIMROT_TIMCTRL3_TEST_SIGNAL_V(e)         BF_TIMROT_TIMCTRL3_TEST_SIGNAL(BV_TIMROT_TIMCTRL3_TEST_SIGNAL__##e)
#define BFM_TIMROT_TIMCTRL3_TEST_SIGNAL_V(v)        BM_TIMROT_TIMCTRL3_TEST_SIGNAL
#define BP_TIMROT_TIMCTRL3_IRQ                      15
#define BM_TIMROT_TIMCTRL3_IRQ                      0x8000
#define BF_TIMROT_TIMCTRL3_IRQ(v)                   (((v) & 0x1) << 15)
#define BFM_TIMROT_TIMCTRL3_IRQ(v)                  BM_TIMROT_TIMCTRL3_IRQ
#define BF_TIMROT_TIMCTRL3_IRQ_V(e)                 BF_TIMROT_TIMCTRL3_IRQ(BV_TIMROT_TIMCTRL3_IRQ__##e)
#define BFM_TIMROT_TIMCTRL3_IRQ_V(v)                BM_TIMROT_TIMCTRL3_IRQ
#define BP_TIMROT_TIMCTRL3_IRQ_EN                   14
#define BM_TIMROT_TIMCTRL3_IRQ_EN                   0x4000
#define BF_TIMROT_TIMCTRL3_IRQ_EN(v)                (((v) & 0x1) << 14)
#define BFM_TIMROT_TIMCTRL3_IRQ_EN(v)               BM_TIMROT_TIMCTRL3_IRQ_EN
#define BF_TIMROT_TIMCTRL3_IRQ_EN_V(e)              BF_TIMROT_TIMCTRL3_IRQ_EN(BV_TIMROT_TIMCTRL3_IRQ_EN__##e)
#define BFM_TIMROT_TIMCTRL3_IRQ_EN_V(v)             BM_TIMROT_TIMCTRL3_IRQ_EN
#define BP_TIMROT_TIMCTRL3_RSRVD1                   11
#define BM_TIMROT_TIMCTRL3_RSRVD1                   0x3800
#define BF_TIMROT_TIMCTRL3_RSRVD1(v)                (((v) & 0x7) << 11)
#define BFM_TIMROT_TIMCTRL3_RSRVD1(v)               BM_TIMROT_TIMCTRL3_RSRVD1
#define BF_TIMROT_TIMCTRL3_RSRVD1_V(e)              BF_TIMROT_TIMCTRL3_RSRVD1(BV_TIMROT_TIMCTRL3_RSRVD1__##e)
#define BFM_TIMROT_TIMCTRL3_RSRVD1_V(v)             BM_TIMROT_TIMCTRL3_RSRVD1
#define BP_TIMROT_TIMCTRL3_DUTY_VALID               10
#define BM_TIMROT_TIMCTRL3_DUTY_VALID               0x400
#define BF_TIMROT_TIMCTRL3_DUTY_VALID(v)            (((v) & 0x1) << 10)
#define BFM_TIMROT_TIMCTRL3_DUTY_VALID(v)           BM_TIMROT_TIMCTRL3_DUTY_VALID
#define BF_TIMROT_TIMCTRL3_DUTY_VALID_V(e)          BF_TIMROT_TIMCTRL3_DUTY_VALID(BV_TIMROT_TIMCTRL3_DUTY_VALID__##e)
#define BFM_TIMROT_TIMCTRL3_DUTY_VALID_V(v)         BM_TIMROT_TIMCTRL3_DUTY_VALID
#define BP_TIMROT_TIMCTRL3_DUTY_CYCLE               9
#define BM_TIMROT_TIMCTRL3_DUTY_CYCLE               0x200
#define BF_TIMROT_TIMCTRL3_DUTY_CYCLE(v)            (((v) & 0x1) << 9)
#define BFM_TIMROT_TIMCTRL3_DUTY_CYCLE(v)           BM_TIMROT_TIMCTRL3_DUTY_CYCLE
#define BF_TIMROT_TIMCTRL3_DUTY_CYCLE_V(e)          BF_TIMROT_TIMCTRL3_DUTY_CYCLE(BV_TIMROT_TIMCTRL3_DUTY_CYCLE__##e)
#define BFM_TIMROT_TIMCTRL3_DUTY_CYCLE_V(v)         BM_TIMROT_TIMCTRL3_DUTY_CYCLE
#define BP_TIMROT_TIMCTRL3_POLARITY                 8
#define BM_TIMROT_TIMCTRL3_POLARITY                 0x100
#define BF_TIMROT_TIMCTRL3_POLARITY(v)              (((v) & 0x1) << 8)
#define BFM_TIMROT_TIMCTRL3_POLARITY(v)             BM_TIMROT_TIMCTRL3_POLARITY
#define BF_TIMROT_TIMCTRL3_POLARITY_V(e)            BF_TIMROT_TIMCTRL3_POLARITY(BV_TIMROT_TIMCTRL3_POLARITY__##e)
#define BFM_TIMROT_TIMCTRL3_POLARITY_V(v)           BM_TIMROT_TIMCTRL3_POLARITY
#define BP_TIMROT_TIMCTRL3_UPDATE                   7
#define BM_TIMROT_TIMCTRL3_UPDATE                   0x80
#define BF_TIMROT_TIMCTRL3_UPDATE(v)                (((v) & 0x1) << 7)
#define BFM_TIMROT_TIMCTRL3_UPDATE(v)               BM_TIMROT_TIMCTRL3_UPDATE
#define BF_TIMROT_TIMCTRL3_UPDATE_V(e)              BF_TIMROT_TIMCTRL3_UPDATE(BV_TIMROT_TIMCTRL3_UPDATE__##e)
#define BFM_TIMROT_TIMCTRL3_UPDATE_V(v)             BM_TIMROT_TIMCTRL3_UPDATE
#define BP_TIMROT_TIMCTRL3_RELOAD                   6
#define BM_TIMROT_TIMCTRL3_RELOAD                   0x40
#define BF_TIMROT_TIMCTRL3_RELOAD(v)                (((v) & 0x1) << 6)
#define BFM_TIMROT_TIMCTRL3_RELOAD(v)               BM_TIMROT_TIMCTRL3_RELOAD
#define BF_TIMROT_TIMCTRL3_RELOAD_V(e)              BF_TIMROT_TIMCTRL3_RELOAD(BV_TIMROT_TIMCTRL3_RELOAD__##e)
#define BFM_TIMROT_TIMCTRL3_RELOAD_V(v)             BM_TIMROT_TIMCTRL3_RELOAD
#define BP_TIMROT_TIMCTRL3_PRESCALE                 4
#define BM_TIMROT_TIMCTRL3_PRESCALE                 0x30
#define BV_TIMROT_TIMCTRL3_PRESCALE__DIV_BY_1       0x0
#define BV_TIMROT_TIMCTRL3_PRESCALE__DIV_BY_2       0x1
#define BV_TIMROT_TIMCTRL3_PRESCALE__DIV_BY_4       0x2
#define BV_TIMROT_TIMCTRL3_PRESCALE__DIV_BY_8       0x3
#define BF_TIMROT_TIMCTRL3_PRESCALE(v)              (((v) & 0x3) << 4)
#define BFM_TIMROT_TIMCTRL3_PRESCALE(v)             BM_TIMROT_TIMCTRL3_PRESCALE
#define BF_TIMROT_TIMCTRL3_PRESCALE_V(e)            BF_TIMROT_TIMCTRL3_PRESCALE(BV_TIMROT_TIMCTRL3_PRESCALE__##e)
#define BFM_TIMROT_TIMCTRL3_PRESCALE_V(v)           BM_TIMROT_TIMCTRL3_PRESCALE
#define BP_TIMROT_TIMCTRL3_SELECT                   0
#define BM_TIMROT_TIMCTRL3_SELECT                   0xf
#define BV_TIMROT_TIMCTRL3_SELECT__NEVER_TICK       0x0
#define BV_TIMROT_TIMCTRL3_SELECT__PWM0             0x1
#define BV_TIMROT_TIMCTRL3_SELECT__PWM1             0x2
#define BV_TIMROT_TIMCTRL3_SELECT__PWM2             0x3
#define BV_TIMROT_TIMCTRL3_SELECT__PWM3             0x4
#define BV_TIMROT_TIMCTRL3_SELECT__PWM4             0x5
#define BV_TIMROT_TIMCTRL3_SELECT__ROTARYA          0x6
#define BV_TIMROT_TIMCTRL3_SELECT__ROTARYB          0x7
#define BV_TIMROT_TIMCTRL3_SELECT__32KHZ_XTAL       0x8
#define BV_TIMROT_TIMCTRL3_SELECT__8KHZ_XTAL        0x9
#define BV_TIMROT_TIMCTRL3_SELECT__4KHZ_XTAL        0xa
#define BV_TIMROT_TIMCTRL3_SELECT__1KHZ_XTAL        0xb
#define BV_TIMROT_TIMCTRL3_SELECT__TICK_ALWAYS      0xc
#define BF_TIMROT_TIMCTRL3_SELECT(v)                (((v) & 0xf) << 0)
#define BFM_TIMROT_TIMCTRL3_SELECT(v)               BM_TIMROT_TIMCTRL3_SELECT
#define BF_TIMROT_TIMCTRL3_SELECT_V(e)              BF_TIMROT_TIMCTRL3_SELECT(BV_TIMROT_TIMCTRL3_SELECT__##e)
#define BFM_TIMROT_TIMCTRL3_SELECT_V(v)             BM_TIMROT_TIMCTRL3_SELECT

#define HW_TIMROT_TIMCOUNT3                         HW(TIMROT_TIMCOUNT3)
#define HWA_TIMROT_TIMCOUNT3                        (0x80068000 + 0x90)
#define HWT_TIMROT_TIMCOUNT3                        HWIO_32_RW
#define HWN_TIMROT_TIMCOUNT3                        TIMROT_TIMCOUNT3
#define HWI_TIMROT_TIMCOUNT3                        
#define BP_TIMROT_TIMCOUNT3_LOW_RUNNING_COUNT       16
#define BM_TIMROT_TIMCOUNT3_LOW_RUNNING_COUNT       0xffff0000
#define BF_TIMROT_TIMCOUNT3_LOW_RUNNING_COUNT(v)    (((v) & 0xffff) << 16)
#define BFM_TIMROT_TIMCOUNT3_LOW_RUNNING_COUNT(v)   BM_TIMROT_TIMCOUNT3_LOW_RUNNING_COUNT
#define BF_TIMROT_TIMCOUNT3_LOW_RUNNING_COUNT_V(e)  BF_TIMROT_TIMCOUNT3_LOW_RUNNING_COUNT(BV_TIMROT_TIMCOUNT3_LOW_RUNNING_COUNT__##e)
#define BFM_TIMROT_TIMCOUNT3_LOW_RUNNING_COUNT_V(v) BM_TIMROT_TIMCOUNT3_LOW_RUNNING_COUNT
#define BP_TIMROT_TIMCOUNT3_HIGH_FIXED_COUNT        0
#define BM_TIMROT_TIMCOUNT3_HIGH_FIXED_COUNT        0xffff
#define BF_TIMROT_TIMCOUNT3_HIGH_FIXED_COUNT(v)     (((v) & 0xffff) << 0)
#define BFM_TIMROT_TIMCOUNT3_HIGH_FIXED_COUNT(v)    BM_TIMROT_TIMCOUNT3_HIGH_FIXED_COUNT
#define BF_TIMROT_TIMCOUNT3_HIGH_FIXED_COUNT_V(e)   BF_TIMROT_TIMCOUNT3_HIGH_FIXED_COUNT(BV_TIMROT_TIMCOUNT3_HIGH_FIXED_COUNT__##e)
#define BFM_TIMROT_TIMCOUNT3_HIGH_FIXED_COUNT_V(v)  BM_TIMROT_TIMCOUNT3_HIGH_FIXED_COUNT

#define HW_TIMROT_VERSION               HW(TIMROT_VERSION)
#define HWA_TIMROT_VERSION              (0x80068000 + 0xa0)
#define HWT_TIMROT_VERSION              HWIO_32_RW
#define HWN_TIMROT_VERSION              TIMROT_VERSION
#define HWI_TIMROT_VERSION              
#define BP_TIMROT_VERSION_MAJOR         24
#define BM_TIMROT_VERSION_MAJOR         0xff000000
#define BF_TIMROT_VERSION_MAJOR(v)      (((v) & 0xff) << 24)
#define BFM_TIMROT_VERSION_MAJOR(v)     BM_TIMROT_VERSION_MAJOR
#define BF_TIMROT_VERSION_MAJOR_V(e)    BF_TIMROT_VERSION_MAJOR(BV_TIMROT_VERSION_MAJOR__##e)
#define BFM_TIMROT_VERSION_MAJOR_V(v)   BM_TIMROT_VERSION_MAJOR
#define BP_TIMROT_VERSION_MINOR         16
#define BM_TIMROT_VERSION_MINOR         0xff0000
#define BF_TIMROT_VERSION_MINOR(v)      (((v) & 0xff) << 16)
#define BFM_TIMROT_VERSION_MINOR(v)     BM_TIMROT_VERSION_MINOR
#define BF_TIMROT_VERSION_MINOR_V(e)    BF_TIMROT_VERSION_MINOR(BV_TIMROT_VERSION_MINOR__##e)
#define BFM_TIMROT_VERSION_MINOR_V(v)   BM_TIMROT_VERSION_MINOR
#define BP_TIMROT_VERSION_STEP          0
#define BM_TIMROT_VERSION_STEP          0xffff
#define BF_TIMROT_VERSION_STEP(v)       (((v) & 0xffff) << 0)
#define BFM_TIMROT_VERSION_STEP(v)      BM_TIMROT_VERSION_STEP
#define BF_TIMROT_VERSION_STEP_V(e)     BF_TIMROT_VERSION_STEP(BV_TIMROT_VERSION_STEP__##e)
#define BFM_TIMROT_VERSION_STEP_V(v)    BM_TIMROT_VERSION_STEP

#endif /* __HEADERGEN_IMX233_TIMROT_H__*/
