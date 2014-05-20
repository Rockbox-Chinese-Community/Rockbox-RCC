#include "aatube.h"
#include "config.h"
#include "fixedpoint.h"
#include "fracmul.h"
#include "settings.h"
#include "dsp_proc_entry.h"

static int32_t aatube_filter_state[2] IBSS_ATTR;

static bool aatube_enabled = false;
static int strength;
static void dsp_aatube_flush(void)
{
    if (!aatube_enabled)
        return;
    aatube_filter_state[0] = 0;
    aatube_filter_state[1] = 0;
}

void dsp_aatube_enable(int var)
{
    bool prev_aatube_enabled = aatube_enabled;
    strength = var;
    aatube_enabled=(var > 0)?  true:false;	
    if (prev_aatube_enabled == aatube_enabled)
        return; /* No change */
    struct dsp_config *dsp = dsp_get_config(CODEC_IDX_AUDIO);
    dsp_proc_enable(dsp, DSP_PROC_AATUBE, aatube_enabled);
    if(aatube_enabled && !dsp_proc_active(dsp,DSP_PROC_AATUBE))
        dsp_proc_activate(dsp, DSP_PROC_AATUBE, true);
    if(!aatube_enabled && dsp_proc_active(dsp,DSP_PROC_AATUBE))
        dsp_proc_activate(dsp, DSP_PROC_AATUBE, false);
}

static void antialias_tube_process(struct dsp_proc_entry *this,
                               struct dsp_buffer **buf_p)
{
    int32_t *pSmpCH0, *pSmpCH1;
    int32_t i;
    struct dsp_buffer *buf = *buf_p;
    int count = buf->remcount;
    float ratio1,ratio2;
    pSmpCH0 = buf->p32[0];
    pSmpCH1 = buf->p32[1];

    /*strength 100% means mix 10% of origional woth 90% buffered sample*/
    ratio2 =  (100- strength*0.9) / 100;
    ratio1 =  (strength*0.9 ) / 100;

        for (i = 0; i < count; i++)
        {
            //aatube_filter_state[0] = (aatube_filter_state[0]+ *pSmpCH0 ) >> 1;
            aatube_filter_state[0] = (aatube_filter_state[0] * ratio1 + *pSmpCH0 * ratio2 );
            *pSmpCH0 = aatube_filter_state[0];
            pSmpCH0++;

            //aatube_filter_state[1] = (aatube_filter_state[1]+ *pSmpCH1 ) >> 1;
            aatube_filter_state[1] = (aatube_filter_state[1] * ratio1 + *pSmpCH1 * ratio2 );
            *pSmpCH1 = aatube_filter_state[1];
            pSmpCH1++;
        }

    (void)this;
}

/* DSP message hook */
static intptr_t aatube_configure(struct dsp_proc_entry *this,
                                     struct dsp_config *dsp,
                                     unsigned int setting,
                                     intptr_t value)
{
    switch (setting)
    {
    case DSP_PROC_INIT:
        if (value != 0)
            break; /* Already enabled */
        this->process = antialias_tube_process;
        //dsp_proc_activate(dsp, DSP_PROC_AATUBE, true);
        break;
    case DSP_FLUSH:
        dsp_aatube_flush();
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
