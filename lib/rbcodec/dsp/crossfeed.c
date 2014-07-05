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
 * Copyright (C) 2010 Bertrik Sikken
 * Copyright (C) 2012 Michael Sevakis
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
#include "rbcodecconfig.h"
#include "fixedpoint.h"
#include "fracmul.h"
#include "replaygain.h"
#include "dsp_misc.h"
#include "dsp_proc_entry.h"
#include "dsp_filter.h"
#include "crossfeed.h"
#include <string.h>

/* Implemented here or in target assembly code */
void crossfeed_process(struct dsp_proc_entry *this,
                       struct dsp_buffer **buf_p);
void crossfeed_meier_process(struct dsp_proc_entry *this,
                             struct dsp_buffer **buf_p);


/**
 * Applies crossfeed to the stereo signal.
 *
 * Crossfeed is a process where listening over speakers is simulated. This
 * is good for old hard panned stereo records, which might be quite fatiguing
 * to listen to on headphones with no crossfeed.
 */

#define DELAY_LEN(fs)   ((300*(fs) / 1000000)*2) /* ~300 uS */

/* Crossfeed */
static struct crossfeed_state
{
    union
    {
        struct /* Data for meier crossfeed */
        {
            int32_t reserved;   /* 00h: Reserved: overlaps gain */
            int32_t vcl;        /* 04h: Left filter output */
            int32_t vcr;        /* 08h: Right filter output */
            int32_t vdiff;      /* 0ch: L-R difference signal */
            int32_t coef1;      /* 10h: Left/right filter coef */
            int32_t coef2;      /* 14h: Crossfeed filter coef */
        };
        struct /* Data for custom crossfeed */
        {
            int32_t gain;       /* 00h: Direct path gain */
            int32_t coefs[3];   /* 04h: Filter coefficients: b0, b1, a1 */
            int32_t history[4]; /* 10h: Format is x[n - 1], y[n - 1] (L + R) */
            int32_t *index;     /* 20h: Current pointer into the delay line */
            int32_t *index_max; /* 24h: Current max pointer of delay line */
                                /* 28h: Delay line buffer (L + R interleaved) */
            int32_t delay[DELAY_LEN(DSP_OUT_MAX_HZ)]; /* Target-dependent size */
        };
    };
} crossfeed_state IBSS_ATTR;

static int crossfeed_type = CROSSFEED_TYPE_NONE;
/* Cached custom settings */
static long crossfeed_lf_gain;
static long crossfeed_hf_gain;
static long crossfeed_cutoff;

/* Discard the sample histories */
static void crossfeed_flush(struct dsp_proc_entry *this)
{
    struct crossfeed_state *state = (void *)this->data;

    if (crossfeed_type != CROSSFEED_TYPE_CUSTOM)
    {
        state->vcl = state->vcr = state->vdiff = 0;
    }
    else
    {
        memset(state->history, 0, sizeof (state->history));
        memset(state->delay, 0, sizeof (state->delay));
        state->index = state->delay;
    }
}

static void crossfeed_meier_update_filter(struct crossfeed_state *state,
                                          unsigned int fout)
{
    /* 1 / (F.Rforward.C) */
    state->coef1 = fp_div(2128, fout, 31);
    /* 1 / (F.Rcross.C) */
    state->coef2 = fp_div(1000, fout, 31);
}

static void crossfeed_custom_update_filter(struct crossfeed_state *state,
                                           unsigned int fout)
{
    long lf_gain = crossfeed_lf_gain;
    long hf_gain = crossfeed_hf_gain;
    long cutoff  = crossfeed_cutoff;
    int32_t *c = state->coefs;

    long scaler = get_replaygain_int(lf_gain * 10) << 7;

    cutoff = fp_div(cutoff, fout, 32);
    hf_gain -= lf_gain;
    /* Divide cutoff by sqrt(10^(hf_gain/20)) to place cutoff at the -3 dB
     * point instead of shelf midpoint. This is for compatibility with the old
     * crossfeed shelf filter and should be removed if crossfeed settings are
     * ever made incompatible for any other good reason.
     */
    cutoff = fp_div(cutoff, get_replaygain_int(hf_gain*5), 24);

    filter_shelf_coefs(cutoff, hf_gain, false, c);
    /* Scale coefs by LF gain and shift them to s0.31 format. We have no gains
     * over 1 and can do this safely
     */
    c[0] = FRACMUL_SHL(c[0], scaler, 4);
    c[1] = FRACMUL_SHL(c[1], scaler, 4);
    c[2] <<= 4;
}


/** DSP interface **/

/* Set the type of crossfeed to use */
void dsp_set_crossfeed_type(int type)
{
    if (type == crossfeed_type)
        return; /* No change */

    crossfeed_type = type;

    struct dsp_config *dsp = dsp_get_config(CODEC_IDX_AUDIO);
    dsp_proc_enable(dsp, DSP_PROC_CROSSFEED, type != CROSSFEED_TYPE_NONE);
}

/* Set the gain of the dry mix */
void dsp_set_crossfeed_direct_gain(int gain)
{
    uint32_t gain32 = get_replaygain_int(gain * 10);
    crossfeed_state.gain =
        gain32 >= (0x80000000ul >> 7) ? 0x7ffffffful: (gain32 << 7);
}

/* Both gains should be below 0 dB */
void dsp_set_crossfeed_cross_params(long lf_gain, long hf_gain, long cutoff)
{
    crossfeed_lf_gain = lf_gain;
    crossfeed_hf_gain = hf_gain;
    crossfeed_cutoff  = cutoff;

    if (crossfeed_type != CROSSFEED_TYPE_CUSTOM)
        return;

    struct dsp_config *dsp = dsp_get_config(CODEC_IDX_AUDIO);
    crossfeed_custom_update_filter(&crossfeed_state,
                                   dsp_get_output_frequency(dsp));
}

#if !defined(CPU_COLDFIRE) && !defined(CPU_ARM)
/* Apply the crossfade to the buffer in place */
void crossfeed_process(struct dsp_proc_entry *this, struct dsp_buffer **buf_p)
{
    struct crossfeed_state *state = (void *)this->data;
    struct dsp_buffer *buf = *buf_p;
   
    int32_t *hist_l = &state->history[0];
    int32_t *hist_r = &state->history[2];
    int32_t *delay = state->delay;
    int32_t *coefs = &state->coefs[0];
    int32_t gain = state->gain;
    int32_t *di = state->index;
    int32_t *di_max = state->index_max;

    int count = buf->remcount;

    for (int i = 0; i < count; i++)
    {
        int32_t left = buf->p32[0][i];
        int32_t right = buf->p32[1][i];

        /* Filter delayed sample from left speaker */
        int32_t acc = FRACMUL(*di, coefs[0]);
        acc += FRACMUL(hist_l[0], coefs[1]);
        acc += FRACMUL(hist_l[1], coefs[2]);
        /* Save filter history for left speaker */
        hist_l[1] = acc;
        hist_l[0] = *di;
        *di++ = left;
        /* Filter delayed sample from right speaker */
        acc = FRACMUL(*di, coefs[0]);
        acc += FRACMUL(hist_r[0], coefs[1]);
        acc += FRACMUL(hist_r[1], coefs[2]);
        /* Save filter history for right speaker */
        hist_r[1] = acc;
        hist_r[0] = *di;
        *di++ = right;
        /* Now add the attenuated direct sound and write to outputs */
        buf->p32[0][i] = FRACMUL(left, gain) + hist_r[1];
        buf->p32[1][i] = FRACMUL(right, gain) + hist_l[1];

        /* Wrap delay line index if bigger than delay line size */
        if (di >= di_max)
            di = delay;
    }

    /* Write back local copies of data we've modified */
    state->index = di;
}
#endif /* CPU */

#if !defined(CPU_COLDFIRE) && !defined(CPU_ARM)
/**
 * Implementation of the "simple" passive crossfeed circuit by Jan Meier.
 * See also: http://www.meier-audio.homepage.t-online.de/passivefilter.htm
 */

void crossfeed_meier_process(struct dsp_proc_entry *this,
                             struct dsp_buffer **buf_p)
{
    struct dsp_buffer *buf = *buf_p;

    /* Get filter state */
    struct crossfeed_state *state = (struct crossfeed_state *)this->data;
    int32_t vcl = state->vcl;
    int32_t vcr = state->vcr;
    int32_t vdiff = state->vdiff;
    int32_t coef1 = state->coef1;
    int32_t coef2 = state->coef2;

    int count = buf->remcount;

    for (int i = 0; i < count; i++)
    {
        /* Calculate new output */
        int32_t lout = buf->p32[0][i] + vcl;
        int32_t rout = buf->p32[1][i] + vcr;
        buf->p32[0][i] = lout;
        buf->p32[1][i] = rout;

        /* Update filter state */
        int32_t common = FRACMUL(vdiff, coef2);
        vcl -= FRACMUL(vcl, coef1) + common;
        vcr -= FRACMUL(vcr, coef1) - common;

        vdiff = lout - rout;
    }

    /* Store filter state */
    state->vcl = vcl;
    state->vcr = vcr;
    state->vdiff = vdiff;
}
#endif /* CPU */

static int32_t tcoef1,tcoef2,tcoef3;

static const float kernel_44k[] = {
	1, -0.020730974, -0.019022247, -0.018487588, -0.017449792, -0.024426898, -0.038292188,
	-0.071077453, -0.11469807, -0.1874774, -0.20722037, -0.00073042441
};
typedef struct crossfeed_s {
        /* 1st band */
	int32_t mid[25];
	int32_t side[25];
        /* 2nd band */
        int32_t mid2[25];
	int32_t side2[25];
        /* 3nd band */
        int32_t mid3[25];
	int32_t side3[25];
	const float *filter;
	unsigned char delay;
        unsigned char delay_h;
        unsigned char delay_sh;
	unsigned char len;
	unsigned char pos;
        unsigned char pos2; 
        unsigned char pos3;
	unsigned char bypass;
} crossfeed_t;

static crossfeed_t *filter;
static crossfeed_t filter_ins;

void crossfeed_LnxPrgr3_init(crossfeed_t *filter) {
        memset(filter, 0, sizeof(crossfeed_t));
        filter->filter = kernel_44k;
        filter->delay = 7;         
        filter->delay_h = 10;      
        filter->delay_sh = 12;     
        filter->pos2 = 1;
        filter->pos3 = 2;
        filter->len = sizeof(kernel_44k)/sizeof(float);

        struct dsp_config *dsp = dsp_get_config(CODEC_IDX_AUDIO);
        unsigned int fout = dsp_get_output_frequency(dsp);
        tcoef1 = fp_div(680, fout, 31);
        tcoef2 = fp_div(1280, fout, 31);
        tcoef3 = fp_div(6800, fout, 31); 
}

static inline void crossfeed_process_sample(crossfeed_t *filter, int32_t left, int32_t right,
                                            int32_t *oleft, int32_t *oright) {

	int32_t mid = (left + right) / 2;
	int32_t side = (left - right) / 2;
	int32_t oside = 0;
        int32_t oside_h = 0; 
        int32_t oside_sh = 0;
	filter->mid[(filter->pos + filter->delay) % filter->len] = mid;
	filter->side[filter->pos] = side;

        filter->mid2[(filter->pos2 + filter->delay_h) % filter->len] = mid;
	filter->side2[filter->pos2] = side;

        filter->mid3[(filter->pos3 + filter->delay_sh) % filter->len] = mid;
	filter->side3[filter->pos3] = side;

	if(!filter->bypass) {
		for(unsigned int i=0;i<filter->len;++i) {
			oside    += filter->side[(filter->pos + filter->len - i) % filter->len] * filter->filter[i];
                        oside_h  += filter->side2[(filter->pos2 + filter->len - i) % filter->len] * filter->filter[i];
                        oside_sh += filter->side3[(filter->pos3 + filter->len - i) % filter->len] * filter->filter[i];
		}
	} else {
		oside    = filter->side[(filter->pos + filter->len - filter->delay) % filter->len];
                oside_h  = filter->side2[(filter->pos2 + filter->len - filter->delay_h) % filter->len];
                oside_sh = filter->side3[(filter->pos3 + filter->len - filter->delay_sh) % filter->len];
	}
/*
        *oleft = filter->mid[filter->pos] + oside;
        *oright = filter->mid[filter->pos] - oside;
*/

        *oleft = *oleft -FRACMUL(oleft[0],tcoef1) 
                 + FRACMUL(filter->mid[filter->pos] + oside,tcoef1) 
                 - FRACMUL(filter->mid[filter->pos] + oside,tcoef2) 
                 + FRACMUL(filter->mid2[filter->pos2] + oside_h,tcoef2) 
                 - FRACMUL(filter->mid2[filter->pos2] + oside_h,tcoef3)
                 + FRACMUL(filter->mid3[filter->pos3] + oside_sh,tcoef3); 

        *oright = *oright -FRACMUL(oright[0],tcoef1) 
                 + FRACMUL(filter->mid[filter->pos] - oside,tcoef1)
                 - FRACMUL(filter->mid[filter->pos] - oside,tcoef2)     
                 + FRACMUL(filter->mid2[filter->pos2] - oside_h,tcoef2) 
                 - FRACMUL(filter->mid2[filter->pos2] - oside_h,tcoef3) 
                 + FRACMUL(filter->mid3[filter->pos3] - oside_sh,tcoef3); 
 
	filter->pos = (filter->pos + 1) % filter->len;
        filter->pos2 = (filter->pos2 + 1) % filter->len;
        filter->pos3 = (filter->pos3 + 1) % filter->len;
}


void crossfeed_LnxPrgr3_process(struct dsp_proc_entry *this,
                             struct dsp_buffer **buf_p)
{
    struct dsp_buffer *buf = *buf_p;
    int count = buf->remcount; 
    if (count < filter->len)
    {
        crossfeed_LnxPrgr3_init(filter);
        return;
    } 
    for(int i=0;i<(count/2);++i)
    {
      crossfeed_process_sample(filter, buf->p32[0][i*2], buf->p32[1][i*2+1], &buf->p32[0][i*2],&buf->p32[1][i*2+1]);
    }
    (void)this;
}

void crossfeed_LnxPrgr3_inplace_noninterleaved_process(struct dsp_proc_entry *this,
                             struct dsp_buffer **buf_p)
{
    struct dsp_buffer *buf = *buf_p;
    int count = buf->remcount; 
    if (count < filter->len)
    {
        crossfeed_LnxPrgr3_init(filter);
        return;
    } 

    for(int i=0;i<count;++i) 
    {
      crossfeed_process_sample(filter, buf->p32[0][i], buf->p32[1][i], &buf->p32[0][i],&buf->p32[1][i]);
    }
    (void)this;
}

static void crossfeed_Lnx_update_filter(int32_t freq_m,int32_t freq_l,
                                         struct crossfeed_state *state,unsigned int fout)
{
    tcoef1 = fp_div(freq_m, fout, 31);
    tcoef2 = fp_div(freq_l, fout, 31);

    state->vcl = state->vcr = state->vdiff = 0;
}



void crossfeed_Lnx_process(struct dsp_proc_entry *this,
                             struct dsp_buffer **buf_p)
{
    struct dsp_buffer *buf = *buf_p;

    /* Get filter state */
    struct crossfeed_state *state = (struct crossfeed_state *)this->data;
    int32_t vcl = state->vcl;
    int32_t vcr = state->vcr;
    int32_t vdiff = state->vdiff;

    int32_t coef1 =tcoef1;
    int32_t coef2 =tcoef2;
    
    int count = buf->remcount;

    int32_t tcl =0;
    int32_t tcr =0; 
    for (int i = 0; i < count; i++)
    {
        /* Calculate new output */
        int32_t lout = buf->p32[0][i] + vcl -tcl+tcr;
        int32_t rout = buf->p32[1][i] + vcr -tcr+tcl;
        
        tcl = FRACMUL(buf->p32[0][i],coef2);
        tcr = FRACMUL(buf->p32[1][i],coef2);
       
        buf->p32[0][i] = lout;
        buf->p32[1][i] = rout;

        /* Update filter state */
        int32_t common = FRACMUL(vdiff, coef2);
           
        
        vcl -= FRACMUL(vcl, coef1) + common;
        vcr -= FRACMUL(vcr, coef1) - common;
        
         
        vdiff = lout - rout;
    }

    /* Store filter state */
    state->vcl = vcl;
    state->vcr = vcr;
    state->vdiff = vdiff;
}


/* Update the processing function according to crossfeed type */
static void update_process_fn(struct dsp_proc_entry *this,
                              struct dsp_config *dsp)
{
    struct crossfeed_state *state = (struct crossfeed_state *)this->data;
    dsp_proc_fn_type fn;


    unsigned int fout = dsp_get_output_frequency(dsp);

    if (crossfeed_type == CROSSFEED_TYPE_MEIER)
    {
        crossfeed_meier_update_filter(state, fout);
        fn = crossfeed_meier_process;
    }
    else if(crossfeed_type == CROSSFEED_TYPE_CUSTOM)
    {
        state->index_max = state->delay + DELAY_LEN(fout);
        crossfeed_custom_update_filter(state, fout);
        fn = crossfeed_process;
    }
    else if (crossfeed_type == CROSSFEED_TYPE_LNX)
    {
        crossfeed_Lnx_update_filter(1280,640,state,fout);
        fn = crossfeed_Lnx_process;
    }
    else if (crossfeed_type == CROSSFEED_TYPE_LNX2)
    {
        crossfeed_Lnx_update_filter(640,320,state,fout);
        fn = crossfeed_Lnx_process;
    }
    else if (crossfeed_type == CROSSFEED_TYPE_LNX3)
    {
        crossfeed_Lnx_update_filter(320,160,state,fout);
        fn = crossfeed_Lnx_process;
    }
    else if (crossfeed_type == CROSSFEED_TYPE_LNX4)
    {
        crossfeed_Lnx_update_filter(160,80,state,fout);
        fn = crossfeed_Lnx_process;
    }
    else if (crossfeed_type == CROSSFEED_TYPE_LNX5)
    {
        crossfeed_Lnx_update_filter(80,40,state,fout);
        fn = crossfeed_Lnx_process;
        
    }
    else if (crossfeed_type == CROSSFEED_TYPE_LNX6)
    {
        filter = &filter_ins;
        crossfeed_LnxPrgr3_init(filter);
        fn = crossfeed_LnxPrgr3_process;
    }
    else if (crossfeed_type == CROSSFEED_TYPE_LNX7)
    {
        filter = &filter_ins;
        crossfeed_LnxPrgr3_init(filter);
        fn = crossfeed_LnxPrgr3_inplace_noninterleaved_process;
    }
    else
    {
        fn = NULL;
    }

    if (this->process != fn)
    {
        this->process = fn;    /* Set proper function */
        if (dsp_proc_active(dsp, DSP_PROC_CROSSFEED))
            crossfeed_flush(this);
    }
}

/* Crossfeed boot/format change function */
static intptr_t crossfeed_new_format(struct dsp_proc_entry *this,
                                     struct dsp_config *dsp,
                                     struct sample_format *format)
{
    DSP_PRINT_FORMAT(DSP_PROC_CROSSFEED, format);

    bool was_active = dsp_proc_active(dsp, DSP_PROC_CROSSFEED);
    bool active = format->num_channels >= 2;
    dsp_proc_activate(dsp, DSP_PROC_CROSSFEED, active);

    if (active)
    {
        if (!was_active)
            crossfeed_flush(this); /* Going online */

        return PROC_NEW_FORMAT_OK;
    }

    /* Can't do this. Sleep until next change */
    DEBUGF("  DSP_PROC_CROSSFEED- deactivated\n");
    return PROC_NEW_FORMAT_DEACTIVATED;
}

/* DSP message hook */
static intptr_t crossfeed_configure(struct dsp_proc_entry *this,
                                    struct dsp_config *dsp,
                                    unsigned int setting,
                                    intptr_t value)
{
    intptr_t retval = 0;

    switch (setting)
    {
    case DSP_PROC_INIT:
        if (value == 0)
            this->data = (intptr_t)&crossfeed_state;

    case DSP_SET_OUT_FREQUENCY:
        update_process_fn(this, dsp);
        break;

    case DSP_FLUSH:
        crossfeed_flush(this);
        break;

    case DSP_PROC_NEW_FORMAT:
        retval = crossfeed_new_format(this, dsp,
                                      (struct sample_format *)value);
        break;
    }

    return retval;
}

/* Database entry */
DSP_PROC_DB_ENTRY(
    CROSSFEED,
    crossfeed_configure);
