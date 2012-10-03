#include "aatube.h"
#include "config.h"
#include "fixedpoint.h"
#include "fracmul.h"
#include "settings.h"
#include "dsp_proc_entry.h"

static int32_t aatube_filter_state[2] IBSS_ATTR;

static bool aatube_enabled = false;

static void dsp_aatube_flush(void)
{
    if (!aatube_enabled)
        return;
    aatube_filter_state[0] = 0;
    aatube_filter_state[1] = 0;
}

void dsp_aatube_enable(bool enable)
{
    aatube_enabled = enable;
    struct dsp_config *dsp = dsp_get_config(CODEC_IDX_AUDIO);
    dsp_proc_enable(dsp, DSP_PROC_AATUBE, enable);
}

static void antialias_tube_process(struct dsp_proc_entry *this,
                               struct dsp_buffer **buf_p)
{
    int32_t *pSmpCH0, *pSmpCH1;
    int32_t i;
    struct dsp_buffer *buf = *buf_p;
    int count = buf->remcount;
    const int num_chan = buf->format.num_channels;
    {
        pSmpCH0 = buf->p32[0];
        pSmpCH1 = buf->p32[1];
        for (int ch = 0; ch < num_chan; ch++)
        {
            for (i = 0; i < count; i++)
            {
                aatube_filter_state[ch] = (aatube_filter_state[ch] + pSmpCH0[i]) >> 1;
                pSmpCH0[i] = aatube_filter_state[ch];
            }
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
    switch (setting)
    {
    case DSP_PROC_INIT:
        if (value != 0)
            break; /* Already enabled */
        this->process[0] = antialias_tube_process;
        dsp_proc_activate(dsp, DSP_PROC_AATUBE, true);
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
