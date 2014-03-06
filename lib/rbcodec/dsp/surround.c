#include "surround.h"
#include "config.h"
#include "fixedpoint.h"
#include "fracmul.h"
#include "settings.h"
#include "dsp_proc_entry.h"

#define DOLBY_SURROUND_MAX 3072
/*STEPSIZE sets the strength of surround effect. */
/*number between 1 to 4: 1 is strongest, 4 is the weakest. */ 
#define STEPSIZE 2 
static int32_t dolbyBuffer[DOLBY_SURROUND_MAX * 2 * ( (STEPSIZE>1)?STEPSIZE:2 ) ]; 
static int32_t dolbyBufferPointer IBSS_ATTR;
static bool surround_enabled = false;

static void dsp_surround_flush(void)
{
    if (!surround_enabled)
        return; 
    memset(dolbyBuffer, 0, sizeof(dolbyBuffer));
    dolbyBufferPointer = 0;
}

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
    		
            if (X < 0) 
            {   /*buffer overflowed*/
                X = 0;
                memset(dolbyBuffer, 0, sizeof(dolbyBuffer));
                dolbyBufferPointer = 0;
                return;
            }
            if (X > count) X = count;
            while (X--)
            {
                backBuffer[0] = *inputSamplesB;
                *inputSamplesB *= 0;
    	        inputSamplesB ++;
                backBuffer += 1;
                dolbyBufferPointer += 1;
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
                backBuffer[0] = *inputSamplesA;
                backBuffer[1] = *inputSamplesB;
                inputSamplesA ++;
                inputSamplesB ++;
                backBuffer += STEPSIZE;
                dolbyBufferPointer += STEPSIZE;
            }
            inputSamplesA = buf->p32[0];
            inputSamplesB = buf->p32[1];	
		
            backBuffer = dolbyBuffer;
            X = count;
		
            while (X--)
            {
                *inputSamplesB = backBuffer[1];
                inputSamplesB ++;
                backBuffer += STEPSIZE;
                dolbyBufferPointer -= STEPSIZE;
            }
            memcpy(dolbyBuffer, backBuffer, dolbyBufferPointer * 4);
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
            break;
        this->process = dolby_surround_process;
        dsp_proc_activate(dsp, DSP_PROC_SURROUND, true);
        break;
    case DSP_FLUSH:
        dsp_surround_flush();
        break;
   case DSP_PROC_CLOSE:
        break;
    }

    return 1;
    (void)dsp;
}

/* Database entry */
DSP_PROC_DB_ENTRY(
    SURROUND,
    surround_configure);

