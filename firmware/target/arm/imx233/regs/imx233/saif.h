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
#ifndef __HEADERGEN_IMX233_SAIF_H__
#define __HEADERGEN_IMX233_SAIF_H__

#define HW_SAIF_CTRL(_n1)                       HW(SAIF_CTRL(_n1))
#define HWA_SAIF_CTRL(_n1)                      (((_n1) == 1 ? 0x80042000 : 0x80046000) + 0x0)
#define HWT_SAIF_CTRL(_n1)                      HWIO_32_RW
#define HWN_SAIF_CTRL(_n1)                      SAIF_CTRL
#define HWI_SAIF_CTRL(_n1)                      (_n1)
#define HW_SAIF_CTRL_SET(_n1)                   HW(SAIF_CTRL_SET(_n1))
#define HWA_SAIF_CTRL_SET(_n1)                  (HWA_SAIF_CTRL(_n1) + 0x4)
#define HWT_SAIF_CTRL_SET(_n1)                  HWIO_32_WO
#define HWN_SAIF_CTRL_SET(_n1)                  SAIF_CTRL
#define HWI_SAIF_CTRL_SET(_n1)                  (_n1)
#define HW_SAIF_CTRL_CLR(_n1)                   HW(SAIF_CTRL_CLR(_n1))
#define HWA_SAIF_CTRL_CLR(_n1)                  (HWA_SAIF_CTRL(_n1) + 0x8)
#define HWT_SAIF_CTRL_CLR(_n1)                  HWIO_32_WO
#define HWN_SAIF_CTRL_CLR(_n1)                  SAIF_CTRL
#define HWI_SAIF_CTRL_CLR(_n1)                  (_n1)
#define HW_SAIF_CTRL_TOG(_n1)                   HW(SAIF_CTRL_TOG(_n1))
#define HWA_SAIF_CTRL_TOG(_n1)                  (HWA_SAIF_CTRL(_n1) + 0xc)
#define HWT_SAIF_CTRL_TOG(_n1)                  HWIO_32_WO
#define HWN_SAIF_CTRL_TOG(_n1)                  SAIF_CTRL
#define HWI_SAIF_CTRL_TOG(_n1)                  (_n1)
#define BP_SAIF_CTRL_SFTRST                     31
#define BM_SAIF_CTRL_SFTRST                     0x80000000
#define BF_SAIF_CTRL_SFTRST(v)                  (((v) & 0x1) << 31)
#define BFM_SAIF_CTRL_SFTRST(v)                 BM_SAIF_CTRL_SFTRST
#define BF_SAIF_CTRL_SFTRST_V(e)                BF_SAIF_CTRL_SFTRST(BV_SAIF_CTRL_SFTRST__##e)
#define BFM_SAIF_CTRL_SFTRST_V(v)               BM_SAIF_CTRL_SFTRST
#define BP_SAIF_CTRL_CLKGATE                    30
#define BM_SAIF_CTRL_CLKGATE                    0x40000000
#define BF_SAIF_CTRL_CLKGATE(v)                 (((v) & 0x1) << 30)
#define BFM_SAIF_CTRL_CLKGATE(v)                BM_SAIF_CTRL_CLKGATE
#define BF_SAIF_CTRL_CLKGATE_V(e)               BF_SAIF_CTRL_CLKGATE(BV_SAIF_CTRL_CLKGATE__##e)
#define BFM_SAIF_CTRL_CLKGATE_V(v)              BM_SAIF_CTRL_CLKGATE
#define BP_SAIF_CTRL_BITCLK_MULT_RATE           27
#define BM_SAIF_CTRL_BITCLK_MULT_RATE           0x38000000
#define BF_SAIF_CTRL_BITCLK_MULT_RATE(v)        (((v) & 0x7) << 27)
#define BFM_SAIF_CTRL_BITCLK_MULT_RATE(v)       BM_SAIF_CTRL_BITCLK_MULT_RATE
#define BF_SAIF_CTRL_BITCLK_MULT_RATE_V(e)      BF_SAIF_CTRL_BITCLK_MULT_RATE(BV_SAIF_CTRL_BITCLK_MULT_RATE__##e)
#define BFM_SAIF_CTRL_BITCLK_MULT_RATE_V(v)     BM_SAIF_CTRL_BITCLK_MULT_RATE
#define BP_SAIF_CTRL_BITCLK_BASE_RATE           26
#define BM_SAIF_CTRL_BITCLK_BASE_RATE           0x4000000
#define BF_SAIF_CTRL_BITCLK_BASE_RATE(v)        (((v) & 0x1) << 26)
#define BFM_SAIF_CTRL_BITCLK_BASE_RATE(v)       BM_SAIF_CTRL_BITCLK_BASE_RATE
#define BF_SAIF_CTRL_BITCLK_BASE_RATE_V(e)      BF_SAIF_CTRL_BITCLK_BASE_RATE(BV_SAIF_CTRL_BITCLK_BASE_RATE__##e)
#define BFM_SAIF_CTRL_BITCLK_BASE_RATE_V(v)     BM_SAIF_CTRL_BITCLK_BASE_RATE
#define BP_SAIF_CTRL_FIFO_ERROR_IRQ_EN          25
#define BM_SAIF_CTRL_FIFO_ERROR_IRQ_EN          0x2000000
#define BF_SAIF_CTRL_FIFO_ERROR_IRQ_EN(v)       (((v) & 0x1) << 25)
#define BFM_SAIF_CTRL_FIFO_ERROR_IRQ_EN(v)      BM_SAIF_CTRL_FIFO_ERROR_IRQ_EN
#define BF_SAIF_CTRL_FIFO_ERROR_IRQ_EN_V(e)     BF_SAIF_CTRL_FIFO_ERROR_IRQ_EN(BV_SAIF_CTRL_FIFO_ERROR_IRQ_EN__##e)
#define BFM_SAIF_CTRL_FIFO_ERROR_IRQ_EN_V(v)    BM_SAIF_CTRL_FIFO_ERROR_IRQ_EN
#define BP_SAIF_CTRL_FIFO_SERVICE_IRQ_EN        24
#define BM_SAIF_CTRL_FIFO_SERVICE_IRQ_EN        0x1000000
#define BF_SAIF_CTRL_FIFO_SERVICE_IRQ_EN(v)     (((v) & 0x1) << 24)
#define BFM_SAIF_CTRL_FIFO_SERVICE_IRQ_EN(v)    BM_SAIF_CTRL_FIFO_SERVICE_IRQ_EN
#define BF_SAIF_CTRL_FIFO_SERVICE_IRQ_EN_V(e)   BF_SAIF_CTRL_FIFO_SERVICE_IRQ_EN(BV_SAIF_CTRL_FIFO_SERVICE_IRQ_EN__##e)
#define BFM_SAIF_CTRL_FIFO_SERVICE_IRQ_EN_V(v)  BM_SAIF_CTRL_FIFO_SERVICE_IRQ_EN
#define BP_SAIF_CTRL_RSRVD2                     21
#define BM_SAIF_CTRL_RSRVD2                     0xe00000
#define BF_SAIF_CTRL_RSRVD2(v)                  (((v) & 0x7) << 21)
#define BFM_SAIF_CTRL_RSRVD2(v)                 BM_SAIF_CTRL_RSRVD2
#define BF_SAIF_CTRL_RSRVD2_V(e)                BF_SAIF_CTRL_RSRVD2(BV_SAIF_CTRL_RSRVD2__##e)
#define BFM_SAIF_CTRL_RSRVD2_V(v)               BM_SAIF_CTRL_RSRVD2
#define BP_SAIF_CTRL_DMAWAIT_COUNT              16
#define BM_SAIF_CTRL_DMAWAIT_COUNT              0x1f0000
#define BF_SAIF_CTRL_DMAWAIT_COUNT(v)           (((v) & 0x1f) << 16)
#define BFM_SAIF_CTRL_DMAWAIT_COUNT(v)          BM_SAIF_CTRL_DMAWAIT_COUNT
#define BF_SAIF_CTRL_DMAWAIT_COUNT_V(e)         BF_SAIF_CTRL_DMAWAIT_COUNT(BV_SAIF_CTRL_DMAWAIT_COUNT__##e)
#define BFM_SAIF_CTRL_DMAWAIT_COUNT_V(v)        BM_SAIF_CTRL_DMAWAIT_COUNT
#define BP_SAIF_CTRL_CHANNEL_NUM_SELECT         14
#define BM_SAIF_CTRL_CHANNEL_NUM_SELECT         0xc000
#define BF_SAIF_CTRL_CHANNEL_NUM_SELECT(v)      (((v) & 0x3) << 14)
#define BFM_SAIF_CTRL_CHANNEL_NUM_SELECT(v)     BM_SAIF_CTRL_CHANNEL_NUM_SELECT
#define BF_SAIF_CTRL_CHANNEL_NUM_SELECT_V(e)    BF_SAIF_CTRL_CHANNEL_NUM_SELECT(BV_SAIF_CTRL_CHANNEL_NUM_SELECT__##e)
#define BFM_SAIF_CTRL_CHANNEL_NUM_SELECT_V(v)   BM_SAIF_CTRL_CHANNEL_NUM_SELECT
#define BP_SAIF_CTRL_RSRVD1                     13
#define BM_SAIF_CTRL_RSRVD1                     0x2000
#define BF_SAIF_CTRL_RSRVD1(v)                  (((v) & 0x1) << 13)
#define BFM_SAIF_CTRL_RSRVD1(v)                 BM_SAIF_CTRL_RSRVD1
#define BF_SAIF_CTRL_RSRVD1_V(e)                BF_SAIF_CTRL_RSRVD1(BV_SAIF_CTRL_RSRVD1__##e)
#define BFM_SAIF_CTRL_RSRVD1_V(v)               BM_SAIF_CTRL_RSRVD1
#define BP_SAIF_CTRL_BIT_ORDER                  12
#define BM_SAIF_CTRL_BIT_ORDER                  0x1000
#define BF_SAIF_CTRL_BIT_ORDER(v)               (((v) & 0x1) << 12)
#define BFM_SAIF_CTRL_BIT_ORDER(v)              BM_SAIF_CTRL_BIT_ORDER
#define BF_SAIF_CTRL_BIT_ORDER_V(e)             BF_SAIF_CTRL_BIT_ORDER(BV_SAIF_CTRL_BIT_ORDER__##e)
#define BFM_SAIF_CTRL_BIT_ORDER_V(v)            BM_SAIF_CTRL_BIT_ORDER
#define BP_SAIF_CTRL_DELAY                      11
#define BM_SAIF_CTRL_DELAY                      0x800
#define BF_SAIF_CTRL_DELAY(v)                   (((v) & 0x1) << 11)
#define BFM_SAIF_CTRL_DELAY(v)                  BM_SAIF_CTRL_DELAY
#define BF_SAIF_CTRL_DELAY_V(e)                 BF_SAIF_CTRL_DELAY(BV_SAIF_CTRL_DELAY__##e)
#define BFM_SAIF_CTRL_DELAY_V(v)                BM_SAIF_CTRL_DELAY
#define BP_SAIF_CTRL_JUSTIFY                    10
#define BM_SAIF_CTRL_JUSTIFY                    0x400
#define BF_SAIF_CTRL_JUSTIFY(v)                 (((v) & 0x1) << 10)
#define BFM_SAIF_CTRL_JUSTIFY(v)                BM_SAIF_CTRL_JUSTIFY
#define BF_SAIF_CTRL_JUSTIFY_V(e)               BF_SAIF_CTRL_JUSTIFY(BV_SAIF_CTRL_JUSTIFY__##e)
#define BFM_SAIF_CTRL_JUSTIFY_V(v)              BM_SAIF_CTRL_JUSTIFY
#define BP_SAIF_CTRL_LRCLK_POLARITY             9
#define BM_SAIF_CTRL_LRCLK_POLARITY             0x200
#define BF_SAIF_CTRL_LRCLK_POLARITY(v)          (((v) & 0x1) << 9)
#define BFM_SAIF_CTRL_LRCLK_POLARITY(v)         BM_SAIF_CTRL_LRCLK_POLARITY
#define BF_SAIF_CTRL_LRCLK_POLARITY_V(e)        BF_SAIF_CTRL_LRCLK_POLARITY(BV_SAIF_CTRL_LRCLK_POLARITY__##e)
#define BFM_SAIF_CTRL_LRCLK_POLARITY_V(v)       BM_SAIF_CTRL_LRCLK_POLARITY
#define BP_SAIF_CTRL_BITCLK_EDGE                8
#define BM_SAIF_CTRL_BITCLK_EDGE                0x100
#define BF_SAIF_CTRL_BITCLK_EDGE(v)             (((v) & 0x1) << 8)
#define BFM_SAIF_CTRL_BITCLK_EDGE(v)            BM_SAIF_CTRL_BITCLK_EDGE
#define BF_SAIF_CTRL_BITCLK_EDGE_V(e)           BF_SAIF_CTRL_BITCLK_EDGE(BV_SAIF_CTRL_BITCLK_EDGE__##e)
#define BFM_SAIF_CTRL_BITCLK_EDGE_V(v)          BM_SAIF_CTRL_BITCLK_EDGE
#define BP_SAIF_CTRL_WORD_LENGTH                4
#define BM_SAIF_CTRL_WORD_LENGTH                0xf0
#define BF_SAIF_CTRL_WORD_LENGTH(v)             (((v) & 0xf) << 4)
#define BFM_SAIF_CTRL_WORD_LENGTH(v)            BM_SAIF_CTRL_WORD_LENGTH
#define BF_SAIF_CTRL_WORD_LENGTH_V(e)           BF_SAIF_CTRL_WORD_LENGTH(BV_SAIF_CTRL_WORD_LENGTH__##e)
#define BFM_SAIF_CTRL_WORD_LENGTH_V(v)          BM_SAIF_CTRL_WORD_LENGTH
#define BP_SAIF_CTRL_BITCLK_48XFS_ENABLE        3
#define BM_SAIF_CTRL_BITCLK_48XFS_ENABLE        0x8
#define BF_SAIF_CTRL_BITCLK_48XFS_ENABLE(v)     (((v) & 0x1) << 3)
#define BFM_SAIF_CTRL_BITCLK_48XFS_ENABLE(v)    BM_SAIF_CTRL_BITCLK_48XFS_ENABLE
#define BF_SAIF_CTRL_BITCLK_48XFS_ENABLE_V(e)   BF_SAIF_CTRL_BITCLK_48XFS_ENABLE(BV_SAIF_CTRL_BITCLK_48XFS_ENABLE__##e)
#define BFM_SAIF_CTRL_BITCLK_48XFS_ENABLE_V(v)  BM_SAIF_CTRL_BITCLK_48XFS_ENABLE
#define BP_SAIF_CTRL_SLAVE_MODE                 2
#define BM_SAIF_CTRL_SLAVE_MODE                 0x4
#define BF_SAIF_CTRL_SLAVE_MODE(v)              (((v) & 0x1) << 2)
#define BFM_SAIF_CTRL_SLAVE_MODE(v)             BM_SAIF_CTRL_SLAVE_MODE
#define BF_SAIF_CTRL_SLAVE_MODE_V(e)            BF_SAIF_CTRL_SLAVE_MODE(BV_SAIF_CTRL_SLAVE_MODE__##e)
#define BFM_SAIF_CTRL_SLAVE_MODE_V(v)           BM_SAIF_CTRL_SLAVE_MODE
#define BP_SAIF_CTRL_READ_MODE                  1
#define BM_SAIF_CTRL_READ_MODE                  0x2
#define BF_SAIF_CTRL_READ_MODE(v)               (((v) & 0x1) << 1)
#define BFM_SAIF_CTRL_READ_MODE(v)              BM_SAIF_CTRL_READ_MODE
#define BF_SAIF_CTRL_READ_MODE_V(e)             BF_SAIF_CTRL_READ_MODE(BV_SAIF_CTRL_READ_MODE__##e)
#define BFM_SAIF_CTRL_READ_MODE_V(v)            BM_SAIF_CTRL_READ_MODE
#define BP_SAIF_CTRL_RUN                        0
#define BM_SAIF_CTRL_RUN                        0x1
#define BF_SAIF_CTRL_RUN(v)                     (((v) & 0x1) << 0)
#define BFM_SAIF_CTRL_RUN(v)                    BM_SAIF_CTRL_RUN
#define BF_SAIF_CTRL_RUN_V(e)                   BF_SAIF_CTRL_RUN(BV_SAIF_CTRL_RUN__##e)
#define BFM_SAIF_CTRL_RUN_V(v)                  BM_SAIF_CTRL_RUN

#define HW_SAIF_STAT(_n1)                       HW(SAIF_STAT(_n1))
#define HWA_SAIF_STAT(_n1)                      (((_n1) == 1 ? 0x80042000 : 0x80046000) + 0x10)
#define HWT_SAIF_STAT(_n1)                      HWIO_32_RW
#define HWN_SAIF_STAT(_n1)                      SAIF_STAT
#define HWI_SAIF_STAT(_n1)                      (_n1)
#define HW_SAIF_STAT_SET(_n1)                   HW(SAIF_STAT_SET(_n1))
#define HWA_SAIF_STAT_SET(_n1)                  (HWA_SAIF_STAT(_n1) + 0x4)
#define HWT_SAIF_STAT_SET(_n1)                  HWIO_32_WO
#define HWN_SAIF_STAT_SET(_n1)                  SAIF_STAT
#define HWI_SAIF_STAT_SET(_n1)                  (_n1)
#define HW_SAIF_STAT_CLR(_n1)                   HW(SAIF_STAT_CLR(_n1))
#define HWA_SAIF_STAT_CLR(_n1)                  (HWA_SAIF_STAT(_n1) + 0x8)
#define HWT_SAIF_STAT_CLR(_n1)                  HWIO_32_WO
#define HWN_SAIF_STAT_CLR(_n1)                  SAIF_STAT
#define HWI_SAIF_STAT_CLR(_n1)                  (_n1)
#define HW_SAIF_STAT_TOG(_n1)                   HW(SAIF_STAT_TOG(_n1))
#define HWA_SAIF_STAT_TOG(_n1)                  (HWA_SAIF_STAT(_n1) + 0xc)
#define HWT_SAIF_STAT_TOG(_n1)                  HWIO_32_WO
#define HWN_SAIF_STAT_TOG(_n1)                  SAIF_STAT
#define HWI_SAIF_STAT_TOG(_n1)                  (_n1)
#define BP_SAIF_STAT_PRESENT                    31
#define BM_SAIF_STAT_PRESENT                    0x80000000
#define BF_SAIF_STAT_PRESENT(v)                 (((v) & 0x1) << 31)
#define BFM_SAIF_STAT_PRESENT(v)                BM_SAIF_STAT_PRESENT
#define BF_SAIF_STAT_PRESENT_V(e)               BF_SAIF_STAT_PRESENT(BV_SAIF_STAT_PRESENT__##e)
#define BFM_SAIF_STAT_PRESENT_V(v)              BM_SAIF_STAT_PRESENT
#define BP_SAIF_STAT_RSRVD2                     17
#define BM_SAIF_STAT_RSRVD2                     0x7ffe0000
#define BF_SAIF_STAT_RSRVD2(v)                  (((v) & 0x3fff) << 17)
#define BFM_SAIF_STAT_RSRVD2(v)                 BM_SAIF_STAT_RSRVD2
#define BF_SAIF_STAT_RSRVD2_V(e)                BF_SAIF_STAT_RSRVD2(BV_SAIF_STAT_RSRVD2__##e)
#define BFM_SAIF_STAT_RSRVD2_V(v)               BM_SAIF_STAT_RSRVD2
#define BP_SAIF_STAT_DMA_PREQ                   16
#define BM_SAIF_STAT_DMA_PREQ                   0x10000
#define BF_SAIF_STAT_DMA_PREQ(v)                (((v) & 0x1) << 16)
#define BFM_SAIF_STAT_DMA_PREQ(v)               BM_SAIF_STAT_DMA_PREQ
#define BF_SAIF_STAT_DMA_PREQ_V(e)              BF_SAIF_STAT_DMA_PREQ(BV_SAIF_STAT_DMA_PREQ__##e)
#define BFM_SAIF_STAT_DMA_PREQ_V(v)             BM_SAIF_STAT_DMA_PREQ
#define BP_SAIF_STAT_RSRVD1                     7
#define BM_SAIF_STAT_RSRVD1                     0xff80
#define BF_SAIF_STAT_RSRVD1(v)                  (((v) & 0x1ff) << 7)
#define BFM_SAIF_STAT_RSRVD1(v)                 BM_SAIF_STAT_RSRVD1
#define BF_SAIF_STAT_RSRVD1_V(e)                BF_SAIF_STAT_RSRVD1(BV_SAIF_STAT_RSRVD1__##e)
#define BFM_SAIF_STAT_RSRVD1_V(v)               BM_SAIF_STAT_RSRVD1
#define BP_SAIF_STAT_FIFO_UNDERFLOW_IRQ         6
#define BM_SAIF_STAT_FIFO_UNDERFLOW_IRQ         0x40
#define BF_SAIF_STAT_FIFO_UNDERFLOW_IRQ(v)      (((v) & 0x1) << 6)
#define BFM_SAIF_STAT_FIFO_UNDERFLOW_IRQ(v)     BM_SAIF_STAT_FIFO_UNDERFLOW_IRQ
#define BF_SAIF_STAT_FIFO_UNDERFLOW_IRQ_V(e)    BF_SAIF_STAT_FIFO_UNDERFLOW_IRQ(BV_SAIF_STAT_FIFO_UNDERFLOW_IRQ__##e)
#define BFM_SAIF_STAT_FIFO_UNDERFLOW_IRQ_V(v)   BM_SAIF_STAT_FIFO_UNDERFLOW_IRQ
#define BP_SAIF_STAT_FIFO_OVERFLOW_IRQ          5
#define BM_SAIF_STAT_FIFO_OVERFLOW_IRQ          0x20
#define BF_SAIF_STAT_FIFO_OVERFLOW_IRQ(v)       (((v) & 0x1) << 5)
#define BFM_SAIF_STAT_FIFO_OVERFLOW_IRQ(v)      BM_SAIF_STAT_FIFO_OVERFLOW_IRQ
#define BF_SAIF_STAT_FIFO_OVERFLOW_IRQ_V(e)     BF_SAIF_STAT_FIFO_OVERFLOW_IRQ(BV_SAIF_STAT_FIFO_OVERFLOW_IRQ__##e)
#define BFM_SAIF_STAT_FIFO_OVERFLOW_IRQ_V(v)    BM_SAIF_STAT_FIFO_OVERFLOW_IRQ
#define BP_SAIF_STAT_FIFO_SERVICE_IRQ           4
#define BM_SAIF_STAT_FIFO_SERVICE_IRQ           0x10
#define BF_SAIF_STAT_FIFO_SERVICE_IRQ(v)        (((v) & 0x1) << 4)
#define BFM_SAIF_STAT_FIFO_SERVICE_IRQ(v)       BM_SAIF_STAT_FIFO_SERVICE_IRQ
#define BF_SAIF_STAT_FIFO_SERVICE_IRQ_V(e)      BF_SAIF_STAT_FIFO_SERVICE_IRQ(BV_SAIF_STAT_FIFO_SERVICE_IRQ__##e)
#define BFM_SAIF_STAT_FIFO_SERVICE_IRQ_V(v)     BM_SAIF_STAT_FIFO_SERVICE_IRQ
#define BP_SAIF_STAT_RSRVD0                     1
#define BM_SAIF_STAT_RSRVD0                     0xe
#define BF_SAIF_STAT_RSRVD0(v)                  (((v) & 0x7) << 1)
#define BFM_SAIF_STAT_RSRVD0(v)                 BM_SAIF_STAT_RSRVD0
#define BF_SAIF_STAT_RSRVD0_V(e)                BF_SAIF_STAT_RSRVD0(BV_SAIF_STAT_RSRVD0__##e)
#define BFM_SAIF_STAT_RSRVD0_V(v)               BM_SAIF_STAT_RSRVD0
#define BP_SAIF_STAT_BUSY                       0
#define BM_SAIF_STAT_BUSY                       0x1
#define BF_SAIF_STAT_BUSY(v)                    (((v) & 0x1) << 0)
#define BFM_SAIF_STAT_BUSY(v)                   BM_SAIF_STAT_BUSY
#define BF_SAIF_STAT_BUSY_V(e)                  BF_SAIF_STAT_BUSY(BV_SAIF_STAT_BUSY__##e)
#define BFM_SAIF_STAT_BUSY_V(v)                 BM_SAIF_STAT_BUSY

#define HW_SAIF_DATA(_n1)               HW(SAIF_DATA(_n1))
#define HWA_SAIF_DATA(_n1)              (((_n1) == 1 ? 0x80042000 : 0x80046000) + 0x20)
#define HWT_SAIF_DATA(_n1)              HWIO_32_RW
#define HWN_SAIF_DATA(_n1)              SAIF_DATA
#define HWI_SAIF_DATA(_n1)              (_n1)
#define HW_SAIF_DATA_SET(_n1)           HW(SAIF_DATA_SET(_n1))
#define HWA_SAIF_DATA_SET(_n1)          (HWA_SAIF_DATA(_n1) + 0x4)
#define HWT_SAIF_DATA_SET(_n1)          HWIO_32_WO
#define HWN_SAIF_DATA_SET(_n1)          SAIF_DATA
#define HWI_SAIF_DATA_SET(_n1)          (_n1)
#define HW_SAIF_DATA_CLR(_n1)           HW(SAIF_DATA_CLR(_n1))
#define HWA_SAIF_DATA_CLR(_n1)          (HWA_SAIF_DATA(_n1) + 0x8)
#define HWT_SAIF_DATA_CLR(_n1)          HWIO_32_WO
#define HWN_SAIF_DATA_CLR(_n1)          SAIF_DATA
#define HWI_SAIF_DATA_CLR(_n1)          (_n1)
#define HW_SAIF_DATA_TOG(_n1)           HW(SAIF_DATA_TOG(_n1))
#define HWA_SAIF_DATA_TOG(_n1)          (HWA_SAIF_DATA(_n1) + 0xc)
#define HWT_SAIF_DATA_TOG(_n1)          HWIO_32_WO
#define HWN_SAIF_DATA_TOG(_n1)          SAIF_DATA
#define HWI_SAIF_DATA_TOG(_n1)          (_n1)
#define BP_SAIF_DATA_PCM_RIGHT          16
#define BM_SAIF_DATA_PCM_RIGHT          0xffff0000
#define BF_SAIF_DATA_PCM_RIGHT(v)       (((v) & 0xffff) << 16)
#define BFM_SAIF_DATA_PCM_RIGHT(v)      BM_SAIF_DATA_PCM_RIGHT
#define BF_SAIF_DATA_PCM_RIGHT_V(e)     BF_SAIF_DATA_PCM_RIGHT(BV_SAIF_DATA_PCM_RIGHT__##e)
#define BFM_SAIF_DATA_PCM_RIGHT_V(v)    BM_SAIF_DATA_PCM_RIGHT
#define BP_SAIF_DATA_PCM_LEFT           0
#define BM_SAIF_DATA_PCM_LEFT           0xffff
#define BF_SAIF_DATA_PCM_LEFT(v)        (((v) & 0xffff) << 0)
#define BFM_SAIF_DATA_PCM_LEFT(v)       BM_SAIF_DATA_PCM_LEFT
#define BF_SAIF_DATA_PCM_LEFT_V(e)      BF_SAIF_DATA_PCM_LEFT(BV_SAIF_DATA_PCM_LEFT__##e)
#define BFM_SAIF_DATA_PCM_LEFT_V(v)     BM_SAIF_DATA_PCM_LEFT

#define HW_SAIF_VERSION(_n1)        HW(SAIF_VERSION(_n1))
#define HWA_SAIF_VERSION(_n1)       (((_n1) == 1 ? 0x80042000 : 0x80046000) + 0x30)
#define HWT_SAIF_VERSION(_n1)       HWIO_32_RW
#define HWN_SAIF_VERSION(_n1)       SAIF_VERSION
#define HWI_SAIF_VERSION(_n1)       (_n1)
#define BP_SAIF_VERSION_MAJOR       24
#define BM_SAIF_VERSION_MAJOR       0xff000000
#define BF_SAIF_VERSION_MAJOR(v)    (((v) & 0xff) << 24)
#define BFM_SAIF_VERSION_MAJOR(v)   BM_SAIF_VERSION_MAJOR
#define BF_SAIF_VERSION_MAJOR_V(e)  BF_SAIF_VERSION_MAJOR(BV_SAIF_VERSION_MAJOR__##e)
#define BFM_SAIF_VERSION_MAJOR_V(v) BM_SAIF_VERSION_MAJOR
#define BP_SAIF_VERSION_MINOR       16
#define BM_SAIF_VERSION_MINOR       0xff0000
#define BF_SAIF_VERSION_MINOR(v)    (((v) & 0xff) << 16)
#define BFM_SAIF_VERSION_MINOR(v)   BM_SAIF_VERSION_MINOR
#define BF_SAIF_VERSION_MINOR_V(e)  BF_SAIF_VERSION_MINOR(BV_SAIF_VERSION_MINOR__##e)
#define BFM_SAIF_VERSION_MINOR_V(v) BM_SAIF_VERSION_MINOR
#define BP_SAIF_VERSION_STEP        0
#define BM_SAIF_VERSION_STEP        0xffff
#define BF_SAIF_VERSION_STEP(v)     (((v) & 0xffff) << 0)
#define BFM_SAIF_VERSION_STEP(v)    BM_SAIF_VERSION_STEP
#define BF_SAIF_VERSION_STEP_V(e)   BF_SAIF_VERSION_STEP(BV_SAIF_VERSION_STEP__##e)
#define BFM_SAIF_VERSION_STEP_V(v)  BM_SAIF_VERSION_STEP

#endif /* __HEADERGEN_IMX233_SAIF_H__*/
