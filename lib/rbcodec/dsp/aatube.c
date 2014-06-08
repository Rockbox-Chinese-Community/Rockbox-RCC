#include "aatube.h"
#include "config.h"
#include "fixedpoint.h"
#include "fracmul.h"
#include "settings.h"
#include "dsp_proc_entry.h"

static int32_t aatube_filter_state[6] IBSS_ATTR;
static bool aatube_enabled = false;
static int strength;
static int32_t tcoef1,bcoef,lcoef,ucoef;
unsigned int fout;

#define ULTRA 22050
#define BASEBAND 11025
#define LOWPASS 1150
#define LOWCUT 320
static void dsp_aatube_flush(void)
{
    if (!aatube_enabled)
        return;
    aatube_filter_state[0] = 0;
    aatube_filter_state[1] = 0;
    aatube_filter_state[2] = 0;
    aatube_filter_state[3] = 0;
    aatube_filter_state[4] = 0;
    aatube_filter_state[5] = 0;
}

static void strength_to_Cutoff(int var)
{
    int x,y;
    if (var == 0)
        return;
    else
    {   
        var = (var+1) * 25; 
        x = (BASEBAND-LOWPASS) * (100-var) / 100; 
        tcoef1 = fp_div(x + LOWPASS, fout, 31);
        bcoef = fp_div(BASEBAND, fout, 31);
        y= LOWCUT * (100-var) / 100;
        lcoef = fp_div(y,fout,31);
        ucoef = fp_div(ULTRA,fout,31); 
        dsp_aatube_flush(); 
    }
}


void dsp_aatube_enable(int var)
{
    bool prev_aatube_enabled = aatube_enabled;
    strength = var;
    aatube_enabled=(var > 0)?  true:false;

    strength_to_Cutoff(strength);
    if (prev_aatube_enabled == aatube_enabled)
        return; /* No change */
    struct dsp_config *dsp = dsp_get_config(CODEC_IDX_AUDIO);
    dsp_proc_enable(dsp, DSP_PROC_AATUBE, aatube_enabled);
}

static void antialias_tube_process(struct dsp_proc_entry *this,
                               struct dsp_buffer **buf_p)
{
    int32_t i;
    struct dsp_buffer *buf = *buf_p;
    int count = buf->remcount;

    for (i = 0; i < count; i++)
    {      
         if (aatube_filter_state[0] > 0 && aatube_filter_state[1] > 0)
            buf->p32[0][i] =  (FRACMUL(buf->p32[0][i], bcoef) -FRACMUL(buf->p32[0][i], ucoef))/2 + aatube_filter_state[4]/2 /*above BASEBAND*/
                              + ( FRACMUL(buf->p32[0][i], tcoef1) -  FRACMUL(buf->p32[0][i], bcoef)) + aatube_filter_state[0]/4  
                              + FRACMUL(buf->p32[0][i], lcoef) - FRACMUL(buf->p32[0][i], tcoef1)
                              + (buf->p32[0][i] - FRACMUL(buf->p32[0][i], lcoef)) + aatube_filter_state[2]/4; 
         else{
             aatube_filter_state[4] = FRACMUL(buf->p32[0][i], bcoef) -FRACMUL(buf->p32[0][i], ucoef); 
             aatube_filter_state[0] = FRACMUL(buf->p32[0][i], tcoef1) -  FRACMUL(buf->p32[0][i], bcoef);
             aatube_filter_state[2] = buf->p32[0][i] - FRACMUL(buf->p32[0][i], lcoef);
         }  
         if (aatube_filter_state[0] > 0 && aatube_filter_state[1] > 0)
            buf->p32[1][i] =  (FRACMUL(buf->p32[1][i], bcoef) -FRACMUL(buf->p32[1][i], ucoef))/2 + aatube_filter_state[5]/2 /*above BASEBAND*/
                              + ( FRACMUL(buf->p32[1][i], tcoef1) -  FRACMUL(buf->p32[1][i], bcoef)) + aatube_filter_state[1]/4
                              + FRACMUL(buf->p32[1][i], lcoef) - FRACMUL(buf->p32[1][i], tcoef1)
                              + (buf->p32[1][i] - FRACMUL(buf->p32[1][i], lcoef)) + aatube_filter_state[3]/4;    
         else{
             aatube_filter_state[5] = FRACMUL(buf->p32[1][i], bcoef) -FRACMUL(buf->p32[1][i], ucoef);
             aatube_filter_state[1] = FRACMUL(buf->p32[1][i], tcoef1) -  FRACMUL(buf->p32[1][i], bcoef);
             aatube_filter_state[3] = buf->p32[1][i] - FRACMUL(buf->p32[1][i], lcoef);
         }
           
    }
    (void)this;
}

/* DSP message hook */
static intptr_t aatube_configure(struct dsp_proc_entry *this,
                                     struct dsp_config *dsp,
                                     unsigned int setting,
                                     intptr_t value)
{
    fout = dsp_get_output_frequency(dsp);
    switch (setting)
    {
    case DSP_PROC_INIT:
        if (value != 0)
            break; /* Already enabled */
        this->process = antialias_tube_process;
        dsp_proc_activate(dsp, DSP_PROC_AATUBE, true);
        break;
    case DSP_FLUSH:
        dsp_aatube_flush();
        break;
    case DSP_SET_OUT_FREQUENCY:
        if (!aatube_enabled)
            this->process = NULL;
        else
           this->process = antialias_tube_process; 
        break;
    case DSP_PROC_CLOSE:
        break;
    }

    return 1;
    (void)dsp;
}

/* Database entry */
DSP_PROC_DB_ENTRY(
    AATUBE,
    aatube_configure);
