/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2005 by Andy Young
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
#include <string.h>

#include "config.h"
#include "logf.h"
#include "system.h"
#include "kernel.h"
#include "audio.h"
#include "debug.h"
#include "udacodec.h"

#include "audiohw.h"

/*  The UDA1380 requires a clock signal at a multiple of the sample rate
    (256Fs, 384Fs, 512Fs or 768Fs, where Fs = sample rate).
    Some targets are able to supply this clock directly to the SYSCLK input.
    The H100 and H300 coldfire targets are limited in the selection of
    frequencies for this clock signal so they use a PLL inside the UDA1380
    (called the WSPLL) to regenerate it from the LRCK signal off the IIS bus.
*/
#if defined(IRIVER_H100_SERIES) || defined(IRIVER_H300_SERIES)
#define USE_WSPLL
#endif

/* convert tenth of dB volume (-840..0) to master volume register value */
static int vol_tenthdb2hw(int db)
{
    if (db < -720)                  /* 1.5 dB steps */
        return (2940 - db) / 15;
    else if (db < -660)             /* 0.75 dB steps */
        return (1110 - db) * 2 / 15;
    else if (db < -520)             /* 0.5 dB steps */
        return (520 - db) / 5;
    else                            /* 0.25 dB steps */
        return -db * 2 / 5;
}

/* convert tenth of dB volume (-780..0) to mixer volume register value */
static int mixer_tenthdb2hw(int db)
{
    if (db < -660)                 /* 1.5 dB steps */
        return (2640 - db) / 15;
    else if (db < -600)            /* 0.75 dB steps */
        return (990 - db) * 2 / 15;
    else if (db < -460)            /* 0.5 dB steps */
        return (460 - db) / 5; 
    else                           /* 0.25 dB steps */
        return -db * 2 / 5;
}

/* ------------------------------------------------- */
/* Local functions and variables */
/* ------------------------------------------------- */

static int uda1380_write_reg(unsigned char reg, unsigned short value);
static unsigned short uda1380_regs[0x30];
static short recgain_mic;
static short recgain_line;

/* Definition of a playback configuration to start with */

#define NUM_DEFAULT_REGS 13
static const unsigned short uda1380_defaults[2*NUM_DEFAULT_REGS] =
{
   REG_0,          EN_DAC | EN_INT | EN_DEC |
#ifdef USE_WSPLL
                   ADC_CLK | DAC_CLK | WSPLL_25_50 |
#endif
                   SYSCLK_256FS,
   REG_I2S,        I2S_IFMT_IIS,
   REG_PWR,        PON_PLL | PON_BIAS,
                   /* PON_HP & PON_DAC is enabled later */
   REG_AMIX,       AMIX_RIGHT(0x3f) | AMIX_LEFT(0x3f),
                   /* 00=max, 3f=mute */
   REG_MASTER_VOL, MASTER_VOL_LEFT(0x20) | MASTER_VOL_RIGHT(0x20),
                   /* 00=max, ff=mute */
   REG_MIX_VOL,    MIX_VOL_CH_1(0) | MIX_VOL_CH_2(0xff),
                   /* 00=max, ff=mute */
   REG_EQ,         EQ_MODE_MAX,
                   /* Bass and treble = 0 dB */
   REG_MUTE,       MUTE_MASTER | MUTE_CH2,
                   /* Mute everything to start with */
   REG_MIX_CTL,    MIX_CTL_MIX | OVERSAMPLE_MODE(3),
                   /* Enable mixer and 4x oversampling */
   REG_DEC_VOL,    0,
   REG_PGA,        MUTE_ADC,
   REG_ADC,        SKIP_DCFIL,
   REG_AGC,        0
};


/* Returns 0 if register was written or -1 if write failed */
static int uda1380_write_reg(unsigned char reg, unsigned short value)
{
    if (udacodec_write(reg, value) < 0)
    {
        DEBUGF("uda1380 error reg=0x%x", reg);
        return -1;
    } 

    uda1380_regs[reg] = value;

    return 0;
}

/**
 * Sets left and right master volume  (0(max) to 252(muted))
 */
void audiohw_set_volume(int vol_l, int vol_r)
{
    vol_l = vol_tenthdb2hw(vol_l);
    vol_r = vol_tenthdb2hw(vol_r);
    uda1380_write_reg(REG_MASTER_VOL,
                      MASTER_VOL_LEFT(vol_l) | MASTER_VOL_RIGHT(vol_r));
}

/**
 * Sets the bass value (0-12)
 */
void audiohw_set_bass(int value)
{
    uda1380_write_reg(REG_EQ, (uda1380_regs[REG_EQ] & ~BASS_MASK)
                              | BASSL(value) | BASSR(value));
}

/**
 * Sets the treble value (0-3)
 */
void audiohw_set_treble(int value)
{
    uda1380_write_reg(REG_EQ, (uda1380_regs[REG_EQ] & ~TREBLE_MASK)
                              | TREBLEL(value) | TREBLER(value));
}

static void audiohw_mute(bool mute)
{
    unsigned int value = uda1380_regs[REG_MUTE];

    if (mute)
        value = value | MUTE_MASTER;
    else
        value = value & ~MUTE_MASTER;

    uda1380_write_reg(REG_MUTE, value);
}

/* Returns 0 if successful or -1 if some register failed */
static int audiohw_set_regs(void)
{
    int i;
    memset(uda1380_regs, 0, sizeof(uda1380_regs));

    /* Initialize all registers */
    for (i=0; i<NUM_DEFAULT_REGS; i++)
    {
        unsigned char reg = uda1380_defaults[i*2+0];
        unsigned short value = uda1380_defaults[i*2+1];

        if (uda1380_write_reg(reg, value) == -1)
            return -1;
    }

    return 0;
}

/**
 * Sets frequency settings for DAC and ADC relative to MCLK
 *
 * Selection for frequency ranges:
 *  Fs:        range:       with:
 *  11025: 0 = 6.25 to 12.5 MCLK/2 SCLK, LRCK: Audio Clk / 16
 *  22050: 1 = 12.5 to 25   MCLK/2 SCLK, LRCK: Audio Clk / 8
 *  44100: 2 = 25   to 50   MCLK   SCLK, LRCK: Audio Clk / 4 (default)
 *  88200: 3 = 50   to 100  MCLK   SCLK, LRCK: Audio Clk / 2
 */
void audiohw_set_frequency(int fsel)
{
    static const unsigned short values_reg[HW_NUM_FREQ][2] =
    {
        [HW_FREQ_11] =                    /* Fs:   */
        {
            0,
            WSPLL_625_125 | SYSCLK_512FS
        },
        [HW_FREQ_22] =
        {
            0,
            WSPLL_125_25 | SYSCLK_256FS
        },
        [HW_FREQ_44] =
        {
            MIX_CTL_SEL_NS,
            WSPLL_25_50 | SYSCLK_256FS
        },
        [HW_FREQ_88] =
        {
            MIX_CTL_SEL_NS,
            WSPLL_50_100 | SYSCLK_256FS
        },
    };

    const unsigned short *ent;

    if ((unsigned)fsel >= HW_NUM_FREQ)
        fsel = HW_FREQ_DEFAULT;

    ent = values_reg[fsel];

    /* Set WSPLL input frequency range or SYSCLK divider */
    uda1380_regs[REG_0] &= ~0xf;
    uda1380_write_reg(REG_0, uda1380_regs[REG_0] | ent[1]);

    /* Choose 3rd order or 5th order noise shaper */
    uda1380_regs[REG_MIX_CTL] &= ~MIX_CTL_SEL_NS;
    uda1380_write_reg(REG_MIX_CTL, uda1380_regs[REG_MIX_CTL] | ent[0]);
}

/* Initialize UDA1380 codec with default register values (uda1380_defaults) */
void audiohw_init(void)
{
    recgain_mic = 0;
    recgain_line = 0;

    udacodec_reset();

    if (audiohw_set_regs() == -1)
    {
        /* this shoud never (!) happen. */
        logf("uda1380: audiohw_init failed");
    }
}

void audiohw_postinit(void)
{
    /* Sleep a while so the power can stabilize (especially a long
       delay is needed for the line out connector). */
    sleep(HZ);

    /* Power on FSDAC and HP amp. */
    uda1380_write_reg(REG_PWR, uda1380_regs[REG_PWR] | PON_DAC | PON_HP);

    /* UDA1380: Unmute the master channel
       (DAC should be at zero point now). */
    audiohw_mute(false);
}

void audiohw_set_prescaler(int val)
{
    val = mixer_tenthdb2hw(-val);
    uda1380_write_reg(REG_MIX_VOL,
                      MIX_VOL_CH_1(val) | MIX_VOL_CH_2(val));
}

/* Nice shutdown of UDA1380 codec */
void audiohw_close(void)
{
    /* First enable mute and sleep a while */
    uda1380_write_reg(REG_MUTE, MUTE_MASTER);
    sleep(HZ/8);

    /* Then power off the rest of the chip */
    uda1380_write_reg(REG_PWR, 0);
    uda1380_write_reg(REG_0, 0);    /* Disable codec    */
}

/**
 * Calling this function enables the UDA1380 to send
 * sound samples over the I2S bus, which is connected
 * to the processor's IIS1 interface. 
 *
 * source_mic: true=record from microphone, false=record from line-in (or radio)
 */
void audiohw_enable_recording(bool source_mic)
{
#ifdef USE_WSPLL
    uda1380_regs[REG_0] &= ~(ADC_CLK | DAC_CLK);
#endif
    uda1380_write_reg(REG_0, uda1380_regs[REG_0] | EN_ADC);

    if (source_mic)
    {
        /* VGA_GAIN: 0=0 dB, F=30dB */
        /* Output of left ADC is fed into right bitstream */
        uda1380_regs[REG_PWR] &= ~(PON_PGAR | PON_ADCR);
        uda1380_write_reg(REG_PWR, uda1380_regs[REG_PWR] | PON_LNA | PON_ADCL);
        uda1380_regs[REG_ADC] &= ~SKIP_DCFIL;
        uda1380_write_reg(REG_ADC, (uda1380_regs[REG_ADC] & VGA_GAIN_MASK)
                                   | SEL_LNA | SEL_MIC | EN_DCFIL);
        uda1380_write_reg(REG_PGA, 0);
    }
    else
    {
        /* PGA_GAIN: 0=0 dB, F=24dB */
        uda1380_regs[REG_PWR] &= ~PON_LNA;
        uda1380_write_reg(REG_PWR, uda1380_regs[REG_PWR] | PON_PGAL | PON_ADCL
                                   | PON_PGAR | PON_ADCR);
        uda1380_write_reg(REG_ADC, EN_DCFIL);
        uda1380_write_reg(REG_PGA, uda1380_regs[REG_PGA] & PGA_GAIN_MASK);
    }

    sleep(HZ/8);

    uda1380_write_reg(REG_I2S,     uda1380_regs[REG_I2S] | I2S_MODE_MASTER);
    uda1380_write_reg(REG_MIX_CTL, MIX_MODE(1)); 
}

/** 
 * Stop sending samples on the I2S bus
 */
void audiohw_disable_recording(void)
{
    uda1380_write_reg(REG_PGA, MUTE_ADC);
    sleep(HZ/8);
    
    uda1380_write_reg(REG_I2S, I2S_IFMT_IIS);

    uda1380_regs[REG_PWR] &= ~(PON_LNA | PON_ADCL | PON_ADCR |
                               PON_PGAL | PON_PGAR);
    uda1380_write_reg(REG_PWR, uda1380_regs[REG_PWR]);

    uda1380_regs[REG_0] &= ~EN_ADC;
#ifdef USE_WSPLL
    uda1380_write_reg(REG_0,   uda1380_regs[REG_0] | ADC_CLK | DAC_CLK);
#endif

    uda1380_write_reg(REG_ADC, SKIP_DCFIL);
}

/**
 * Set recording gain and volume
 * 
 * type:                params:        ranges:
 * AUDIO_GAIN_MIC:      left           -128 .. 108 -> -64 .. 54 dB gain
 * AUDIO_GAIN_LINEIN    left & right   -128 ..  96 -> -64 .. 48 dB gain
 *
 * Note: - For all types the value 0 gives 0 dB gain.
 *       - order of setting both values determines if the small glitch will
           be a peak or a dip. The small glitch is caused by the time between
           setting the two gains
 */
void audiohw_set_recvol(int left, int right, int type)
{
    int left_ag, right_ag;

    switch (type)
    {
        case AUDIO_GAIN_MIC:
            left_ag = MIN(MAX(0, left / 4), 15);
            left -= left_ag * 4;

            if(left < recgain_mic)
            {
                uda1380_write_reg(REG_DEC_VOL, DEC_VOLL(left)
                                                   | DEC_VOLR(left));
                uda1380_write_reg(REG_ADC, (uda1380_regs[REG_ADC] 
                                               & ~VGA_GAIN_MASK) 
                                            | VGA_GAIN(left_ag));
            }
            else
            {
                uda1380_write_reg(REG_ADC, (uda1380_regs[REG_ADC] 
                                               & ~VGA_GAIN_MASK) 
                                            | VGA_GAIN(left_ag));
                uda1380_write_reg(REG_DEC_VOL, DEC_VOLL(left) 
                                                   | DEC_VOLR(left));
            }
            recgain_mic = left;
            logf("Mic: %dA/%dD", left_ag, left);
            break;
        
        case AUDIO_GAIN_LINEIN:
            left_ag = MIN(MAX(0, left / 6), 8);
            left -= left_ag * 6;
            right_ag = MIN(MAX(0, right / 6), 8);
            right -= right_ag * 6;

            if(left < recgain_line)
            {
                /* for this order we can combine both registers,
                    making the glitch even smaller */
                unsigned short value_dec;
                unsigned short value_pga;
                value_dec = DEC_VOLL(left) | DEC_VOLR(right);
                value_pga = (uda1380_regs[REG_PGA] & ~PGA_GAIN_MASK)
                                | PGA_GAINL(left_ag) | PGA_GAINR(right_ag);

                if (udacodec_write2(REG_DEC_VOL, value_dec, value_pga) < 0)
                {
                    DEBUGF("uda1380 error reg=combi rec gain");
                }
                else
                {
                    uda1380_regs[REG_DEC_VOL] = value_dec;
                    uda1380_regs[REG_PGA] = value_pga;
                }
            }
            else
            {
                uda1380_write_reg(REG_PGA, (uda1380_regs[REG_PGA] 
                                               & ~PGA_GAIN_MASK)
                                            | PGA_GAINL(left_ag)
                                            | PGA_GAINR(right_ag));
                uda1380_write_reg(REG_DEC_VOL, DEC_VOLL(left)
                                                   | DEC_VOLR(right));
            }

            recgain_line = left;
            logf("Line L: %dA/%dD", left_ag, left);
            logf("Line R: %dA/%dD", right_ag, right);
            break;
    }
}


/** 
 * Enable or disable recording monitor (so one can listen to the recording)
 * 
 */
void audiohw_set_monitor(bool enable)
{
    if (enable)    /* enable channel 2 */
        uda1380_write_reg(REG_MUTE, uda1380_regs[REG_MUTE] & ~MUTE_CH2);
    else           /* mute channel 2 */
        uda1380_write_reg(REG_MUTE, uda1380_regs[REG_MUTE] | MUTE_CH2);
}
