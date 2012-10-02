#include "surround_aatube.h"
#include "config.h"
#include "fixedpoint.h"
#include "fracmul.h"
#include "settings.h"
#include "dsp_proc_entry.h"
/* dolby surround */
#define DOLBY_SURROUND_MAX 2048
static int32_t dolbyBuffer[DOLBY_SURROUND_MAX * 2];
static int32_t dolbyBufferPointer IBSS_ATTR;

static bool surround_enabled = false;
static bool aatube_enabled = false;

/* antialias-tube */
static int32_t aatube_filter_state[2] IBSS_ATTR;
 
/* Dolby surround */
void dsp_surround_enable(bool enable)
{
    surround_enabled = enable;
    struct dsp_config *dsp = dsp_get_config(CODEC_IDX_AUDIO);
    dsp_proc_enable(dsp, DSP_PROC_SURROUND, enable);
}

static void dolby_surround_process(struct dsp_proc_entry *this,
                               struct dsp_buffer **buf_p)
{
    struct dsp_buffer *buf = *buf_p;
    int count = buf->remcount;
    int32_t *inputSamplesA, *inputSamplesB;
    int32_t *backBuffer;
    int32_t X;
    if ((buf->format.num_channels == 2) && surround_enabled)
    {		
        if (dolbyBufferPointer < DOLBY_SURROUND_MAX)
        {
            inputSamplesA = buf->p32[0];
            inputSamplesB = buf->p32[1];	
		
            backBuffer = dolbyBuffer + dolbyBufferPointer;
            X = DOLBY_SURROUND_MAX - dolbyBufferPointer;
    		
            if (X < 0) X = 0;
            if (X > count) X = count;
            while (X--)
            {
                backBuffer[1] = *inputSamplesB;

                *inputSamplesB *= 0;
    	        inputSamplesB ++;
                backBuffer += 2;
                dolbyBufferPointer += 2;
            }
        }
        else
        {
            inputSamplesA = buf->p32[0];
            inputSamplesB = buf->p32[1];	
		
            backBuffer = dolbyBuffer + dolbyBufferPointer;
            X = count;
    		
            while (X--)
            {
                backBuffer[1] = *inputSamplesB;
	    		
                inputSamplesB ++;
                backBuffer += 2;
                dolbyBufferPointer += 2;
            }
		
            inputSamplesA = buf->p32[0];
            inputSamplesB = buf->p32[1];	
		
            backBuffer = dolbyBuffer;
            X = count;
		
            while (X--)
            {
                *inputSamplesB = backBuffer[1];
			
                inputSamplesB ++;
                backBuffer += 2;
                dolbyBufferPointer -= 2;
            }
            memcpy(dolbyBuffer, backBuffer, dolbyBufferPointer * 4);
        }
    }
(void)this;
}

/* antialias tube */
void dsp_aatube_enable(bool enable)
{
    aatube_enabled = enable;
    struct dsp_config *dsp = dsp_get_config(CODEC_IDX_AUDIO);
    dsp_proc_enable(dsp, DSP_PROC_AATUBE, enable);
    dsp_proc_activate(dsp, DSP_PROC_AATUBE, true);
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
static intptr_t surround_configure(struct dsp_proc_entry *this,
                                     struct dsp_config *dsp,
                                     unsigned int setting,
                                     intptr_t value)
{
    switch (setting)
    {
    case DSP_PROC_INIT:
        if (value != 0)
            break; /* Already enabled */
        this->process[0] = dolby_surround_process;
        dsp_proc_activate(dsp, DSP_PROC_SURROUND, true);
        break;
    case DSP_RESET:
    case DSP_FLUSH:
        break;
    case DSP_PROC_CLOSE:
        dolbyBufferPointer = 0;
    }

    return 1;
    (void)dsp;
}

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
        break;
    case DSP_PROC_CLOSE:
        aatube_filter_state[0] = 0;
	aatube_filter_state[1] = 0;
        break;
    }

    return 1;
    (void)dsp;
}

/* Database entry */
DSP_PROC_DB_ENTRY(
    SURROUND,
    surround_configure);
DSP_PROC_DB_ENTRY(
    AATUBE,
    aatube_configure);

