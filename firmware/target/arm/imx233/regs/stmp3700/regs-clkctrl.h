/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * This file was automatically generated by headergen, DO NOT EDIT it.
 * headergen version: 2.1.7
 * XML versions: stmp3700:3.2.0
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
#ifndef __HEADERGEN__STMP3700__CLKCTRL__H__
#define __HEADERGEN__STMP3700__CLKCTRL__H__

#define REGS_CLKCTRL_BASE (0x80040000)

#define REGS_CLKCTRL_VERSION "3.2.0"

/**
 * Register: HW_CLKCTRL_PLLCTRL0
 * Address: 0
 * SCT: yes
*/
#define HW_CLKCTRL_PLLCTRL0                     (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x0 + 0x0))
#define HW_CLKCTRL_PLLCTRL0_SET                 (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x0 + 0x4))
#define HW_CLKCTRL_PLLCTRL0_CLR                 (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x0 + 0x8))
#define HW_CLKCTRL_PLLCTRL0_TOG                 (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x0 + 0xc))
#define BP_CLKCTRL_PLLCTRL0_LFR_SEL             28
#define BM_CLKCTRL_PLLCTRL0_LFR_SEL             0x30000000
#define BV_CLKCTRL_PLLCTRL0_LFR_SEL__DEFAULT    0x0
#define BV_CLKCTRL_PLLCTRL0_LFR_SEL__TIMES_2    0x1
#define BV_CLKCTRL_PLLCTRL0_LFR_SEL__TIMES_05   0x2
#define BV_CLKCTRL_PLLCTRL0_LFR_SEL__UNDEFINED  0x3
#define BF_CLKCTRL_PLLCTRL0_LFR_SEL(v)          (((v) << 28) & 0x30000000)
#define BF_CLKCTRL_PLLCTRL0_LFR_SEL_V(v)        ((BV_CLKCTRL_PLLCTRL0_LFR_SEL__##v << 28) & 0x30000000)
#define BP_CLKCTRL_PLLCTRL0_CP_SEL              24
#define BM_CLKCTRL_PLLCTRL0_CP_SEL              0x3000000
#define BV_CLKCTRL_PLLCTRL0_CP_SEL__DEFAULT     0x0
#define BV_CLKCTRL_PLLCTRL0_CP_SEL__TIMES_2     0x1
#define BV_CLKCTRL_PLLCTRL0_CP_SEL__TIMES_05    0x2
#define BV_CLKCTRL_PLLCTRL0_CP_SEL__UNDEFINED   0x3
#define BF_CLKCTRL_PLLCTRL0_CP_SEL(v)           (((v) << 24) & 0x3000000)
#define BF_CLKCTRL_PLLCTRL0_CP_SEL_V(v)         ((BV_CLKCTRL_PLLCTRL0_CP_SEL__##v << 24) & 0x3000000)
#define BP_CLKCTRL_PLLCTRL0_DIV_SEL             20
#define BM_CLKCTRL_PLLCTRL0_DIV_SEL             0x300000
#define BV_CLKCTRL_PLLCTRL0_DIV_SEL__DEFAULT    0x0
#define BV_CLKCTRL_PLLCTRL0_DIV_SEL__LOWER      0x1
#define BV_CLKCTRL_PLLCTRL0_DIV_SEL__LOWEST     0x2
#define BV_CLKCTRL_PLLCTRL0_DIV_SEL__UNDEFINED  0x3
#define BF_CLKCTRL_PLLCTRL0_DIV_SEL(v)          (((v) << 20) & 0x300000)
#define BF_CLKCTRL_PLLCTRL0_DIV_SEL_V(v)        ((BV_CLKCTRL_PLLCTRL0_DIV_SEL__##v << 20) & 0x300000)
#define BP_CLKCTRL_PLLCTRL0_EN_USB_CLKS         18
#define BM_CLKCTRL_PLLCTRL0_EN_USB_CLKS         0x40000
#define BF_CLKCTRL_PLLCTRL0_EN_USB_CLKS(v)      (((v) << 18) & 0x40000)
#define BP_CLKCTRL_PLLCTRL0_POWER               16
#define BM_CLKCTRL_PLLCTRL0_POWER               0x10000
#define BF_CLKCTRL_PLLCTRL0_POWER(v)            (((v) << 16) & 0x10000)

/**
 * Register: HW_CLKCTRL_PLLCTRL1
 * Address: 0x10
 * SCT: no
*/
#define HW_CLKCTRL_PLLCTRL1                 (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x10))
#define BP_CLKCTRL_PLLCTRL1_LOCK            31
#define BM_CLKCTRL_PLLCTRL1_LOCK            0x80000000
#define BF_CLKCTRL_PLLCTRL1_LOCK(v)         (((v) << 31) & 0x80000000)
#define BP_CLKCTRL_PLLCTRL1_FORCE_LOCK      30
#define BM_CLKCTRL_PLLCTRL1_FORCE_LOCK      0x40000000
#define BF_CLKCTRL_PLLCTRL1_FORCE_LOCK(v)   (((v) << 30) & 0x40000000)
#define BP_CLKCTRL_PLLCTRL1_LOCK_COUNT      0
#define BM_CLKCTRL_PLLCTRL1_LOCK_COUNT      0xffff
#define BF_CLKCTRL_PLLCTRL1_LOCK_COUNT(v)   (((v) << 0) & 0xffff)

/**
 * Register: HW_CLKCTRL_CPU
 * Address: 0x20
 * SCT: yes
*/
#define HW_CLKCTRL_CPU                      (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x20 + 0x0))
#define HW_CLKCTRL_CPU_SET                  (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x20 + 0x4))
#define HW_CLKCTRL_CPU_CLR                  (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x20 + 0x8))
#define HW_CLKCTRL_CPU_TOG                  (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x20 + 0xc))
#define BP_CLKCTRL_CPU_BUSY_REF_XTAL        29
#define BM_CLKCTRL_CPU_BUSY_REF_XTAL        0x20000000
#define BF_CLKCTRL_CPU_BUSY_REF_XTAL(v)     (((v) << 29) & 0x20000000)
#define BP_CLKCTRL_CPU_BUSY_REF_CPU         28
#define BM_CLKCTRL_CPU_BUSY_REF_CPU         0x10000000
#define BF_CLKCTRL_CPU_BUSY_REF_CPU(v)      (((v) << 28) & 0x10000000)
#define BP_CLKCTRL_CPU_DIV_XTAL_FRAC_EN     26
#define BM_CLKCTRL_CPU_DIV_XTAL_FRAC_EN     0x4000000
#define BF_CLKCTRL_CPU_DIV_XTAL_FRAC_EN(v)  (((v) << 26) & 0x4000000)
#define BP_CLKCTRL_CPU_DIV_XTAL             16
#define BM_CLKCTRL_CPU_DIV_XTAL             0x3ff0000
#define BF_CLKCTRL_CPU_DIV_XTAL(v)          (((v) << 16) & 0x3ff0000)
#define BP_CLKCTRL_CPU_INTERRUPT_WAIT       12
#define BM_CLKCTRL_CPU_INTERRUPT_WAIT       0x1000
#define BF_CLKCTRL_CPU_INTERRUPT_WAIT(v)    (((v) << 12) & 0x1000)
#define BP_CLKCTRL_CPU_DIV_CPU_FRAC_EN      10
#define BM_CLKCTRL_CPU_DIV_CPU_FRAC_EN      0x400
#define BF_CLKCTRL_CPU_DIV_CPU_FRAC_EN(v)   (((v) << 10) & 0x400)
#define BP_CLKCTRL_CPU_DIV_CPU              0
#define BM_CLKCTRL_CPU_DIV_CPU              0x3ff
#define BF_CLKCTRL_CPU_DIV_CPU(v)           (((v) << 0) & 0x3ff)

/**
 * Register: HW_CLKCTRL_HBUS
 * Address: 0x30
 * SCT: yes
*/
#define HW_CLKCTRL_HBUS                             (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x30 + 0x0))
#define HW_CLKCTRL_HBUS_SET                         (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x30 + 0x4))
#define HW_CLKCTRL_HBUS_CLR                         (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x30 + 0x8))
#define HW_CLKCTRL_HBUS_TOG                         (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x30 + 0xc))
#define BP_CLKCTRL_HBUS_BUSY                        29
#define BM_CLKCTRL_HBUS_BUSY                        0x20000000
#define BF_CLKCTRL_HBUS_BUSY(v)                     (((v) << 29) & 0x20000000)
#define BP_CLKCTRL_HBUS_APBHDMA_AS_ENABLE           26
#define BM_CLKCTRL_HBUS_APBHDMA_AS_ENABLE           0x4000000
#define BF_CLKCTRL_HBUS_APBHDMA_AS_ENABLE(v)        (((v) << 26) & 0x4000000)
#define BP_CLKCTRL_HBUS_APBXDMA_AS_ENABLE           25
#define BM_CLKCTRL_HBUS_APBXDMA_AS_ENABLE           0x2000000
#define BF_CLKCTRL_HBUS_APBXDMA_AS_ENABLE(v)        (((v) << 25) & 0x2000000)
#define BP_CLKCTRL_HBUS_TRAFFIC_JAM_AS_ENABLE       24
#define BM_CLKCTRL_HBUS_TRAFFIC_JAM_AS_ENABLE       0x1000000
#define BF_CLKCTRL_HBUS_TRAFFIC_JAM_AS_ENABLE(v)    (((v) << 24) & 0x1000000)
#define BP_CLKCTRL_HBUS_TRAFFIC_AS_ENABLE           23
#define BM_CLKCTRL_HBUS_TRAFFIC_AS_ENABLE           0x800000
#define BF_CLKCTRL_HBUS_TRAFFIC_AS_ENABLE(v)        (((v) << 23) & 0x800000)
#define BP_CLKCTRL_HBUS_CPU_DATA_AS_ENABLE          22
#define BM_CLKCTRL_HBUS_CPU_DATA_AS_ENABLE          0x400000
#define BF_CLKCTRL_HBUS_CPU_DATA_AS_ENABLE(v)       (((v) << 22) & 0x400000)
#define BP_CLKCTRL_HBUS_CPU_INSTR_AS_ENABLE         21
#define BM_CLKCTRL_HBUS_CPU_INSTR_AS_ENABLE         0x200000
#define BF_CLKCTRL_HBUS_CPU_INSTR_AS_ENABLE(v)      (((v) << 21) & 0x200000)
#define BP_CLKCTRL_HBUS_AUTO_SLOW_MODE              20
#define BM_CLKCTRL_HBUS_AUTO_SLOW_MODE              0x100000
#define BF_CLKCTRL_HBUS_AUTO_SLOW_MODE(v)           (((v) << 20) & 0x100000)
#define BP_CLKCTRL_HBUS_SLOW_DIV                    16
#define BM_CLKCTRL_HBUS_SLOW_DIV                    0x70000
#define BV_CLKCTRL_HBUS_SLOW_DIV__BY1               0x0
#define BV_CLKCTRL_HBUS_SLOW_DIV__BY2               0x1
#define BV_CLKCTRL_HBUS_SLOW_DIV__BY4               0x2
#define BV_CLKCTRL_HBUS_SLOW_DIV__BY8               0x3
#define BV_CLKCTRL_HBUS_SLOW_DIV__BY16              0x4
#define BV_CLKCTRL_HBUS_SLOW_DIV__BY32              0x5
#define BF_CLKCTRL_HBUS_SLOW_DIV(v)                 (((v) << 16) & 0x70000)
#define BF_CLKCTRL_HBUS_SLOW_DIV_V(v)               ((BV_CLKCTRL_HBUS_SLOW_DIV__##v << 16) & 0x70000)
#define BP_CLKCTRL_HBUS_DIV_FRAC_EN                 5
#define BM_CLKCTRL_HBUS_DIV_FRAC_EN                 0x20
#define BF_CLKCTRL_HBUS_DIV_FRAC_EN(v)              (((v) << 5) & 0x20)
#define BP_CLKCTRL_HBUS_DIV                         0
#define BM_CLKCTRL_HBUS_DIV                         0x1f
#define BF_CLKCTRL_HBUS_DIV(v)                      (((v) << 0) & 0x1f)

/**
 * Register: HW_CLKCTRL_XBUS
 * Address: 0x40
 * SCT: no
*/
#define HW_CLKCTRL_XBUS                 (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x40))
#define BP_CLKCTRL_XBUS_BUSY            31
#define BM_CLKCTRL_XBUS_BUSY            0x80000000
#define BF_CLKCTRL_XBUS_BUSY(v)         (((v) << 31) & 0x80000000)
#define BP_CLKCTRL_XBUS_DIV_FRAC_EN     10
#define BM_CLKCTRL_XBUS_DIV_FRAC_EN     0x400
#define BF_CLKCTRL_XBUS_DIV_FRAC_EN(v)  (((v) << 10) & 0x400)
#define BP_CLKCTRL_XBUS_DIV             0
#define BM_CLKCTRL_XBUS_DIV             0x3ff
#define BF_CLKCTRL_XBUS_DIV(v)          (((v) << 0) & 0x3ff)

/**
 * Register: HW_CLKCTRL_XTAL
 * Address: 0x50
 * SCT: yes
*/
#define HW_CLKCTRL_XTAL                         (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x50 + 0x0))
#define HW_CLKCTRL_XTAL_SET                     (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x50 + 0x4))
#define HW_CLKCTRL_XTAL_CLR                     (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x50 + 0x8))
#define HW_CLKCTRL_XTAL_TOG                     (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x50 + 0xc))
#define BP_CLKCTRL_XTAL_UART_CLK_GATE           31
#define BM_CLKCTRL_XTAL_UART_CLK_GATE           0x80000000
#define BF_CLKCTRL_XTAL_UART_CLK_GATE(v)        (((v) << 31) & 0x80000000)
#define BP_CLKCTRL_XTAL_FILT_CLK24M_GATE        30
#define BM_CLKCTRL_XTAL_FILT_CLK24M_GATE        0x40000000
#define BF_CLKCTRL_XTAL_FILT_CLK24M_GATE(v)     (((v) << 30) & 0x40000000)
#define BP_CLKCTRL_XTAL_PWM_CLK24M_GATE         29
#define BM_CLKCTRL_XTAL_PWM_CLK24M_GATE         0x20000000
#define BF_CLKCTRL_XTAL_PWM_CLK24M_GATE(v)      (((v) << 29) & 0x20000000)
#define BP_CLKCTRL_XTAL_DRI_CLK24M_GATE         28
#define BM_CLKCTRL_XTAL_DRI_CLK24M_GATE         0x10000000
#define BF_CLKCTRL_XTAL_DRI_CLK24M_GATE(v)      (((v) << 28) & 0x10000000)
#define BP_CLKCTRL_XTAL_DIGCTRL_CLK1M_GATE      27
#define BM_CLKCTRL_XTAL_DIGCTRL_CLK1M_GATE      0x8000000
#define BF_CLKCTRL_XTAL_DIGCTRL_CLK1M_GATE(v)   (((v) << 27) & 0x8000000)
#define BP_CLKCTRL_XTAL_TIMROT_CLK32K_GATE      26
#define BM_CLKCTRL_XTAL_TIMROT_CLK32K_GATE      0x4000000
#define BF_CLKCTRL_XTAL_TIMROT_CLK32K_GATE(v)   (((v) << 26) & 0x4000000)
#define BP_CLKCTRL_XTAL_DIV_UART                0
#define BM_CLKCTRL_XTAL_DIV_UART                0x3
#define BF_CLKCTRL_XTAL_DIV_UART(v)             (((v) << 0) & 0x3)

/**
 * Register: HW_CLKCTRL_PIX
 * Address: 0x60
 * SCT: no
*/
#define HW_CLKCTRL_PIX                  (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x60))
#define BP_CLKCTRL_PIX_CLKGATE          31
#define BM_CLKCTRL_PIX_CLKGATE          0x80000000
#define BF_CLKCTRL_PIX_CLKGATE(v)       (((v) << 31) & 0x80000000)
#define BP_CLKCTRL_PIX_BUSY             29
#define BM_CLKCTRL_PIX_BUSY             0x20000000
#define BF_CLKCTRL_PIX_BUSY(v)          (((v) << 29) & 0x20000000)
#define BP_CLKCTRL_PIX_DIV_FRAC_EN      15
#define BM_CLKCTRL_PIX_DIV_FRAC_EN      0x8000
#define BF_CLKCTRL_PIX_DIV_FRAC_EN(v)   (((v) << 15) & 0x8000)
#define BP_CLKCTRL_PIX_DIV              0
#define BM_CLKCTRL_PIX_DIV              0x7fff
#define BF_CLKCTRL_PIX_DIV(v)           (((v) << 0) & 0x7fff)

/**
 * Register: HW_CLKCTRL_SSP
 * Address: 0x70
 * SCT: no
*/
#define HW_CLKCTRL_SSP                  (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x70))
#define BP_CLKCTRL_SSP_CLKGATE          31
#define BM_CLKCTRL_SSP_CLKGATE          0x80000000
#define BF_CLKCTRL_SSP_CLKGATE(v)       (((v) << 31) & 0x80000000)
#define BP_CLKCTRL_SSP_BUSY             29
#define BM_CLKCTRL_SSP_BUSY             0x20000000
#define BF_CLKCTRL_SSP_BUSY(v)          (((v) << 29) & 0x20000000)
#define BP_CLKCTRL_SSP_DIV_FRAC_EN      9
#define BM_CLKCTRL_SSP_DIV_FRAC_EN      0x200
#define BF_CLKCTRL_SSP_DIV_FRAC_EN(v)   (((v) << 9) & 0x200)
#define BP_CLKCTRL_SSP_DIV              0
#define BM_CLKCTRL_SSP_DIV              0x1ff
#define BF_CLKCTRL_SSP_DIV(v)           (((v) << 0) & 0x1ff)

/**
 * Register: HW_CLKCTRL_GPMI
 * Address: 0x80
 * SCT: no
*/
#define HW_CLKCTRL_GPMI                 (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x80))
#define BP_CLKCTRL_GPMI_CLKGATE         31
#define BM_CLKCTRL_GPMI_CLKGATE         0x80000000
#define BF_CLKCTRL_GPMI_CLKGATE(v)      (((v) << 31) & 0x80000000)
#define BP_CLKCTRL_GPMI_BUSY            29
#define BM_CLKCTRL_GPMI_BUSY            0x20000000
#define BF_CLKCTRL_GPMI_BUSY(v)         (((v) << 29) & 0x20000000)
#define BP_CLKCTRL_GPMI_DIV_FRAC_EN     10
#define BM_CLKCTRL_GPMI_DIV_FRAC_EN     0x400
#define BF_CLKCTRL_GPMI_DIV_FRAC_EN(v)  (((v) << 10) & 0x400)
#define BP_CLKCTRL_GPMI_DIV             0
#define BM_CLKCTRL_GPMI_DIV             0x3ff
#define BF_CLKCTRL_GPMI_DIV(v)          (((v) << 0) & 0x3ff)

/**
 * Register: HW_CLKCTRL_SPDIF
 * Address: 0x90
 * SCT: no
*/
#define HW_CLKCTRL_SPDIF            (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x90))
#define BP_CLKCTRL_SPDIF_CLKGATE    31
#define BM_CLKCTRL_SPDIF_CLKGATE    0x80000000
#define BF_CLKCTRL_SPDIF_CLKGATE(v) (((v) << 31) & 0x80000000)

/**
 * Register: HW_CLKCTRL_EMI
 * Address: 0xa0
 * SCT: no
*/
#define HW_CLKCTRL_EMI                      (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0xa0))
#define BP_CLKCTRL_EMI_CLKGATE              31
#define BM_CLKCTRL_EMI_CLKGATE              0x80000000
#define BF_CLKCTRL_EMI_CLKGATE(v)           (((v) << 31) & 0x80000000)
#define BP_CLKCTRL_EMI_BUSY_REF_XTAL        29
#define BM_CLKCTRL_EMI_BUSY_REF_XTAL        0x20000000
#define BF_CLKCTRL_EMI_BUSY_REF_XTAL(v)     (((v) << 29) & 0x20000000)
#define BP_CLKCTRL_EMI_BUSY_REF_EMI         28
#define BM_CLKCTRL_EMI_BUSY_REF_EMI         0x10000000
#define BF_CLKCTRL_EMI_BUSY_REF_EMI(v)      (((v) << 28) & 0x10000000)
#define BP_CLKCTRL_EMI_BUSY_DCC_RESYNC      17
#define BM_CLKCTRL_EMI_BUSY_DCC_RESYNC      0x20000
#define BF_CLKCTRL_EMI_BUSY_DCC_RESYNC(v)   (((v) << 17) & 0x20000)
#define BP_CLKCTRL_EMI_DCC_RESYNC_ENABLE    16
#define BM_CLKCTRL_EMI_DCC_RESYNC_ENABLE    0x10000
#define BF_CLKCTRL_EMI_DCC_RESYNC_ENABLE(v) (((v) << 16) & 0x10000)
#define BP_CLKCTRL_EMI_DIV_XTAL             8
#define BM_CLKCTRL_EMI_DIV_XTAL             0xf00
#define BF_CLKCTRL_EMI_DIV_XTAL(v)          (((v) << 8) & 0xf00)
#define BP_CLKCTRL_EMI_DIV_EMI              0
#define BM_CLKCTRL_EMI_DIV_EMI              0x3f
#define BF_CLKCTRL_EMI_DIV_EMI(v)           (((v) << 0) & 0x3f)

/**
 * Register: HW_CLKCTRL_IR
 * Address: 0xb0
 * SCT: no
*/
#define HW_CLKCTRL_IR               (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0xb0))
#define BP_CLKCTRL_IR_CLKGATE       31
#define BM_CLKCTRL_IR_CLKGATE       0x80000000
#define BF_CLKCTRL_IR_CLKGATE(v)    (((v) << 31) & 0x80000000)
#define BP_CLKCTRL_IR_AUTO_DIV      29
#define BM_CLKCTRL_IR_AUTO_DIV      0x20000000
#define BF_CLKCTRL_IR_AUTO_DIV(v)   (((v) << 29) & 0x20000000)
#define BP_CLKCTRL_IR_IR_BUSY       28
#define BM_CLKCTRL_IR_IR_BUSY       0x10000000
#define BF_CLKCTRL_IR_IR_BUSY(v)    (((v) << 28) & 0x10000000)
#define BP_CLKCTRL_IR_IROV_BUSY     27
#define BM_CLKCTRL_IR_IROV_BUSY     0x8000000
#define BF_CLKCTRL_IR_IROV_BUSY(v)  (((v) << 27) & 0x8000000)
#define BP_CLKCTRL_IR_IROV_DIV      16
#define BM_CLKCTRL_IR_IROV_DIV      0x1ff0000
#define BF_CLKCTRL_IR_IROV_DIV(v)   (((v) << 16) & 0x1ff0000)
#define BP_CLKCTRL_IR_IR_DIV        0
#define BM_CLKCTRL_IR_IR_DIV        0x3ff
#define BF_CLKCTRL_IR_IR_DIV(v)     (((v) << 0) & 0x3ff)

/**
 * Register: HW_CLKCTRL_SAIF
 * Address: 0xc0
 * SCT: no
*/
#define HW_CLKCTRL_SAIF                 (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0xc0))
#define BP_CLKCTRL_SAIF_CLKGATE         31
#define BM_CLKCTRL_SAIF_CLKGATE         0x80000000
#define BF_CLKCTRL_SAIF_CLKGATE(v)      (((v) << 31) & 0x80000000)
#define BP_CLKCTRL_SAIF_BUSY            29
#define BM_CLKCTRL_SAIF_BUSY            0x20000000
#define BF_CLKCTRL_SAIF_BUSY(v)         (((v) << 29) & 0x20000000)
#define BP_CLKCTRL_SAIF_DIV_FRAC_EN     16
#define BM_CLKCTRL_SAIF_DIV_FRAC_EN     0x10000
#define BF_CLKCTRL_SAIF_DIV_FRAC_EN(v)  (((v) << 16) & 0x10000)
#define BP_CLKCTRL_SAIF_DIV             0
#define BM_CLKCTRL_SAIF_DIV             0xffff
#define BF_CLKCTRL_SAIF_DIV(v)          (((v) << 0) & 0xffff)

/**
 * Register: HW_CLKCTRL_FRAC
 * Address: 0xd0
 * SCT: yes
*/
#define HW_CLKCTRL_FRAC                 (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0xd0 + 0x0))
#define HW_CLKCTRL_FRAC_SET             (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0xd0 + 0x4))
#define HW_CLKCTRL_FRAC_CLR             (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0xd0 + 0x8))
#define HW_CLKCTRL_FRAC_TOG             (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0xd0 + 0xc))
#define BP_CLKCTRL_FRAC_CLKGATEIO       31
#define BM_CLKCTRL_FRAC_CLKGATEIO       0x80000000
#define BF_CLKCTRL_FRAC_CLKGATEIO(v)    (((v) << 31) & 0x80000000)
#define BP_CLKCTRL_FRAC_IO_STABLE       30
#define BM_CLKCTRL_FRAC_IO_STABLE       0x40000000
#define BF_CLKCTRL_FRAC_IO_STABLE(v)    (((v) << 30) & 0x40000000)
#define BP_CLKCTRL_FRAC_IOFRAC          24
#define BM_CLKCTRL_FRAC_IOFRAC          0x3f000000
#define BF_CLKCTRL_FRAC_IOFRAC(v)       (((v) << 24) & 0x3f000000)
#define BP_CLKCTRL_FRAC_CLKGATEPIX      23
#define BM_CLKCTRL_FRAC_CLKGATEPIX      0x800000
#define BF_CLKCTRL_FRAC_CLKGATEPIX(v)   (((v) << 23) & 0x800000)
#define BP_CLKCTRL_FRAC_PIX_STABLE      22
#define BM_CLKCTRL_FRAC_PIX_STABLE      0x400000
#define BF_CLKCTRL_FRAC_PIX_STABLE(v)   (((v) << 22) & 0x400000)
#define BP_CLKCTRL_FRAC_PIXFRAC         16
#define BM_CLKCTRL_FRAC_PIXFRAC         0x3f0000
#define BF_CLKCTRL_FRAC_PIXFRAC(v)      (((v) << 16) & 0x3f0000)
#define BP_CLKCTRL_FRAC_CLKGATEEMI      15
#define BM_CLKCTRL_FRAC_CLKGATEEMI      0x8000
#define BF_CLKCTRL_FRAC_CLKGATEEMI(v)   (((v) << 15) & 0x8000)
#define BP_CLKCTRL_FRAC_EMI_STABLE      14
#define BM_CLKCTRL_FRAC_EMI_STABLE      0x4000
#define BF_CLKCTRL_FRAC_EMI_STABLE(v)   (((v) << 14) & 0x4000)
#define BP_CLKCTRL_FRAC_EMIFRAC         8
#define BM_CLKCTRL_FRAC_EMIFRAC         0x3f00
#define BF_CLKCTRL_FRAC_EMIFRAC(v)      (((v) << 8) & 0x3f00)
#define BP_CLKCTRL_FRAC_CLKGATECPU      7
#define BM_CLKCTRL_FRAC_CLKGATECPU      0x80
#define BF_CLKCTRL_FRAC_CLKGATECPU(v)   (((v) << 7) & 0x80)
#define BP_CLKCTRL_FRAC_CPU_STABLE      6
#define BM_CLKCTRL_FRAC_CPU_STABLE      0x40
#define BF_CLKCTRL_FRAC_CPU_STABLE(v)   (((v) << 6) & 0x40)
#define BP_CLKCTRL_FRAC_CPUFRAC         0
#define BM_CLKCTRL_FRAC_CPUFRAC         0x3f
#define BF_CLKCTRL_FRAC_CPUFRAC(v)      (((v) << 0) & 0x3f)

/**
 * Register: HW_CLKCTRL_CLKSEQ
 * Address: 0xe0
 * SCT: yes
*/
#define HW_CLKCTRL_CLKSEQ                   (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0xe0 + 0x0))
#define HW_CLKCTRL_CLKSEQ_SET               (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0xe0 + 0x4))
#define HW_CLKCTRL_CLKSEQ_CLR               (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0xe0 + 0x8))
#define HW_CLKCTRL_CLKSEQ_TOG               (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0xe0 + 0xc))
#define BP_CLKCTRL_CLKSEQ_BYPASS_CPU        7
#define BM_CLKCTRL_CLKSEQ_BYPASS_CPU        0x80
#define BF_CLKCTRL_CLKSEQ_BYPASS_CPU(v)     (((v) << 7) & 0x80)
#define BP_CLKCTRL_CLKSEQ_BYPASS_EMI        6
#define BM_CLKCTRL_CLKSEQ_BYPASS_EMI        0x40
#define BF_CLKCTRL_CLKSEQ_BYPASS_EMI(v)     (((v) << 6) & 0x40)
#define BP_CLKCTRL_CLKSEQ_BYPASS_SSP        5
#define BM_CLKCTRL_CLKSEQ_BYPASS_SSP        0x20
#define BF_CLKCTRL_CLKSEQ_BYPASS_SSP(v)     (((v) << 5) & 0x20)
#define BP_CLKCTRL_CLKSEQ_BYPASS_GPMI       4
#define BM_CLKCTRL_CLKSEQ_BYPASS_GPMI       0x10
#define BF_CLKCTRL_CLKSEQ_BYPASS_GPMI(v)    (((v) << 4) & 0x10)
#define BP_CLKCTRL_CLKSEQ_BYPASS_IR         3
#define BM_CLKCTRL_CLKSEQ_BYPASS_IR         0x8
#define BF_CLKCTRL_CLKSEQ_BYPASS_IR(v)      (((v) << 3) & 0x8)
#define BP_CLKCTRL_CLKSEQ_BYPASS_PIX        1
#define BM_CLKCTRL_CLKSEQ_BYPASS_PIX        0x2
#define BF_CLKCTRL_CLKSEQ_BYPASS_PIX(v)     (((v) << 1) & 0x2)
#define BP_CLKCTRL_CLKSEQ_BYPASS_SAIF       0
#define BM_CLKCTRL_CLKSEQ_BYPASS_SAIF       0x1
#define BF_CLKCTRL_CLKSEQ_BYPASS_SAIF(v)    (((v) << 0) & 0x1)

/**
 * Register: HW_CLKCTRL_RESET
 * Address: 0xf0
 * SCT: no
*/
#define HW_CLKCTRL_RESET            (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0xf0))
#define BP_CLKCTRL_RESET_CHIP       1
#define BM_CLKCTRL_RESET_CHIP       0x2
#define BF_CLKCTRL_RESET_CHIP(v)    (((v) << 1) & 0x2)
#define BP_CLKCTRL_RESET_DIG        0
#define BM_CLKCTRL_RESET_DIG        0x1
#define BF_CLKCTRL_RESET_DIG(v)     (((v) << 0) & 0x1)

/**
 * Register: HW_CLKCTRL_VERSION
 * Address: 0x100
 * SCT: no
*/
#define HW_CLKCTRL_VERSION          (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x100))
#define BP_CLKCTRL_VERSION_MAJOR    24
#define BM_CLKCTRL_VERSION_MAJOR    0xff000000
#define BF_CLKCTRL_VERSION_MAJOR(v) (((v) << 24) & 0xff000000)
#define BP_CLKCTRL_VERSION_MINOR    16
#define BM_CLKCTRL_VERSION_MINOR    0xff0000
#define BF_CLKCTRL_VERSION_MINOR(v) (((v) << 16) & 0xff0000)
#define BP_CLKCTRL_VERSION_STEP     0
#define BM_CLKCTRL_VERSION_STEP     0xffff
#define BF_CLKCTRL_VERSION_STEP(v)  (((v) << 0) & 0xffff)

#endif /* __HEADERGEN__STMP3700__CLKCTRL__H__ */
