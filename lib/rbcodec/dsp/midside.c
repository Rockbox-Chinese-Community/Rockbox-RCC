#include "aatube.h"
#include "config.h"
#include "fixedpoint.h"
#include "fracmul.h"
#include "settings.h"
#include "dsp_proc_entry.h"

static int32_t mid  IBSS_ATTR;
static int32_t side IBSS_ATTR;
static bool midside_enabled = false;
static int mid_ratio = 100, side_ratio = 100;
unsigned int fout;

static void dsp_midside_flush(void)
{
    if (!midside_enabled)
        return;
    mid=0;
    side=0;
}

void dsp_midside_enable(int var)
{
    bool prev_midside_enabled = midside_enabled;
    midside_enabled=(var > 0)?  true:false;

    if (prev_midside_enabled == midside_enabled)
        return; /* No change */
    struct dsp_config *dsp = dsp_get_config(CODEC_IDX_AUDIO);
    dsp_proc_enable(dsp, DSP_PROC_MIDSIDE, midside_enabled);
}

void dsp_midside_mid_level(int var)
{
    mid_ratio = var;
}

void dsp_midside_side_level(int var)
{
    side_ratio = var;
}


static void mid_side_process(struct dsp_proc_entry *this,
                               struct dsp_buffer **buf_p)
{
    int32_t i;
    struct dsp_buffer *buf = *buf_p;
    int count = buf->remcount;

    for (i = 0; i < count; i++)
    {     
         mid  = buf->p32[0][i]/2 + buf->p32[1][i]/2;
         side = (buf->p32[0][i] - buf->p32[1][i])/2; 
         
         buf->p32[0][i] =  mid/100 * mid_ratio + side/100 * side_ratio;
         buf->p32[1][i] =  mid/100 * mid_ratio - side/100 * side_ratio;
           
    }
    (void)this;
}

/* DSP message hook */
static intptr_t midside_configure(struct dsp_proc_entry *this,
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
        this->process = mid_side_process;
        dsp_proc_activate(dsp, DSP_PROC_MIDSIDE, true);
        break;
    case DSP_FLUSH:
        dsp_midside_flush();
        break;
    case DSP_SET_OUT_FREQUENCY:
        if (!midside_enabled)
            this->process = NULL;
        else
           this->process = mid_side_process; 
        break;
    case DSP_PROC_CLOSE:
        break;
    }

    return 1;
    (void)dsp;
}

/* Database entry */
DSP_PROC_DB_ENTRY(
    MIDSIDE,
    midside_configure);
