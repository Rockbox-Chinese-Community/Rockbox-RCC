#include "surround.h"
#include "config.h"
#include "fixedpoint.h"
#include "fracmul.h"
#include "settings.h"
#include "dsp_proc_entry.h"

#define DOLBY_SURROUND_MAX 3072
static bool surround_enabled = false;
static int surround_balance = 0;
static bool surround_alter_method = false;
/*1 sample ~ 11ns */
#define dly_1ms  91
#define dly_2ms  182
#define dly_5ms  454
#define dly_8ms  727
#define dly_10ms 909
#define max_delay dly_10ms

/*only need to buffer right side for haas effect*/
static int32_t b0[max_delay],b2[max_delay], bb[max_delay], hh[max_delay];
static int32_t temp_buffer[2][DOLBY_SURROUND_MAX * 2];
static int32_t mid_buffer[DOLBY_SURROUND_MAX * 2];

/*voice from 300hz - 3400hz ?*/
static int32_t tcoef1,tcoef2,bcoef,hcoef;

static int dly_size = max_delay;
static int cutoff_l = 320;
static int cutoff_h = 3400;

static void dsp_surround_flush(void)
{
    if (!surround_enabled)
        return; 
    memset(b0,0,max_delay * sizeof(int32_t)); 
    memset(b2,0,max_delay * sizeof(int32_t));
    memset(bb,0,max_delay * sizeof(int32_t)); 
    memset(hh,0,max_delay * sizeof(int32_t));
}

static void surround_update_filter(unsigned int fout)
{
    tcoef1 = fp_div(cutoff_l, fout, 31);
    tcoef2 = fp_div(cutoff_h, fout, 31);
    bcoef  = fp_div(cutoff_l / 2, fout, 31);
    hcoef  = fp_div(cutoff_h * 2, fout, 31);
}

void dsp_surround_set_balance(int var)
{
    surround_balance = var;
}

void dsp_surround_alter_method(bool var)
{
    dsp_surround_flush();
    surround_alter_method = var;
}

void dsp_surround_set_cutoff(int frq_l, int frq_h)
{
    cutoff_l = frq_l;/*fx2*/
    cutoff_h = frq_h;/*fx1*/

    struct dsp_config *dsp = dsp_get_config(CODEC_IDX_AUDIO);
    unsigned int fout = dsp_get_output_frequency(dsp);
    surround_update_filter(fout);
}

static void surround_set_stepsize(int var)
{
    dsp_surround_flush();
    if (var > 0)
    {
    if (var == 1) dly_size =  dly_1ms;
    if (var == 2) dly_size =  dly_2ms;
    if (var == 3) dly_size =  dly_5ms; 
    if (var == 4) dly_size =  dly_8ms; 
    if (var == 5) dly_size =  dly_10ms;
    }
}

void dsp_surround_enable(int var)
{
    bool prev_surround_enabled = surround_enabled;
    surround_set_stepsize(var);
    surround_enabled = (var > 0)?true:false;
    if (prev_surround_enabled == surround_enabled)
        return; /* No change */
    struct dsp_config *dsp = dsp_get_config(CODEC_IDX_AUDIO);
    dsp_proc_enable(dsp, DSP_PROC_SURROUND, surround_enabled);
}

static void dolby_surround_process(struct dsp_proc_entry *this,
                               struct dsp_buffer **buf_p)
{

    struct dsp_buffer *buf = *buf_p;
    int count = buf->remcount;

    int dly_shift3 = dly_size/8;
    int dly_shift2 = dly_size/4;
    int dly_shift1 = dly_size/2;
    int dly = dly_size;
    int i;
    int32_t diff;

    if (count < dly_size)
    {
        dsp_surround_flush(); 
        return;  
    }

    for (i = 0; i < count; i++)
    {
        mid_buffer[i] = buf->p32[0][i] /2 + buf->p32[1][i] /2;
        diff = buf->p32[0][i] - buf->p32[1][i];

        if (!surround_alter_method)
        { 
            temp_buffer[0][i]= buf->p32[0][i];  /*copy whole left channal*/
            /*only need middle band for right channel*/
            temp_buffer[1][i]= FRACMUL(buf->p32[1][i], tcoef1) - FRACMUL(buf->p32[1][i], tcoef2);  
        }
        else
        {
            temp_buffer[0][i] = diff / 2;
            temp_buffer[1][i] = FRACMUL(-diff,tcoef1)/2 - FRACMUL(-diff, tcoef2)/2; 
        }       
    }

    /* apply 1/8 delay to frequency below fx2 */
    for (i = 0; i < dly_shift3; i++) 
    {
        temp_buffer[1][i] +=b0[i];
    }
    for (i = 0; i < count-dly_shift3; i++)
    {
        temp_buffer[1][i+dly_shift3] += buf->p32[1][i] - FRACMUL(buf->p32[1][i], tcoef1);
    }
    for (i = 0; i < dly_shift3; i++)
    {
        b0[i] = buf->p32[1][count-dly_shift3+i] - FRACMUL(buf->p32[1][count-dly_shift3+i], tcoef1);
    }
    /* cut frequency below half fx2*/
    for (i = 0; i < count; i++)
        temp_buffer[1][i] = FRACMUL(temp_buffer[1][i], bcoef);
    
    /*apply 1/4 delay to below half fx2 */
    /* using different delay will create the illusion of the lower frequency sound direction*/
    for (i = 0; i < dly_shift2; i++) 
    {
        temp_buffer[1][i] +=bb[i];
    }
    for (i = 0; i < count-dly_shift2; i++)
    {
        temp_buffer[1][i+dly_shift2] += buf->p32[1][i] - FRACMUL(buf->p32[1][i], bcoef);
    }
    for (i = 0; i < dly_shift2; i++)
    {
        bb[i] = buf->p32[1][count-dly_shift2+i] - FRACMUL(buf->p32[1][count-dly_shift2+i], bcoef);
    }

    /*apply full delay to higher band*/
    for (i = 0; i < dly; i++)
    { 
        temp_buffer[1][i] +=b2[i]; 
    }
    for (i = 0; i < count-dly; i++)
    {  
        temp_buffer[1][i+dly]+= FRACMUL(buf->p32[1][i], tcoef2);
    } 
    for (i = 0; i < dly; i++)
    {  
        b2[i] = FRACMUL(buf->p32[1][count-dly+i], tcoef2);
    }
    /*do the same direction trick,*/
    for (i = 0; i < count; i++)
        temp_buffer[1][i] -= FRACMUL(temp_buffer[1][i], hcoef);
    /*apply the same trick using 1/2 delay to frequency twice the fx1*/
    for (i = 0; i < dly_shift1; i++)
    { 
        temp_buffer[1][i] +=hh[i]; 
    }
    for (i = 0; i < count-dly_shift1; i++)
    {  
        temp_buffer[1][i+dly_shift1]+= FRACMUL(buf->p32[1][i], hcoef);
    }
    for (i = 0; i < dly_shift1; i++)
    {  
        hh[i] = FRACMUL(buf->p32[1][count-dly_shift1+i], hcoef);
    }


    for (i = 0; i < count; i++) /*balance*/
    { 
        if (surround_balance > 0  && !surround_alter_method)
        {
            temp_buffer[0][i] -= temp_buffer[0][i]/200  * surround_balance;
            temp_buffer[1][i] += temp_buffer[1][i]/200  * surround_balance;
        }
        else if (surround_balance > 0)
        {
            temp_buffer[0][i] += temp_buffer[0][i]/200  * surround_balance;
            temp_buffer[1][i] -= temp_buffer[1][i]/200  * surround_balance;
        }
    }

//  
    if  (surround_alter_method)
    {
        for (i = 0; i < count; i++)
        {
            temp_buffer[0][i] += mid_buffer[i];
            temp_buffer[1][i] += mid_buffer[i];    
        }
    }  
//
    memcpy(buf->p32[0],temp_buffer[0],count * sizeof(int32_t));
    memcpy(buf->p32[1],temp_buffer[1],count * sizeof(int32_t));
   
    
    (void)this;
}

/* DSP message hook */
static intptr_t surround_configure(struct dsp_proc_entry *this,
                                     struct dsp_config *dsp,
                                     unsigned int setting,
                                     intptr_t value)
{
    unsigned int fout = dsp_get_output_frequency(dsp);
    switch (setting)
    {
    case DSP_PROC_INIT:
        if (value != 0)
            break;
        this->process = dolby_surround_process;
        surround_update_filter(fout);
        dsp_proc_activate(dsp, DSP_PROC_SURROUND, true);
        break;
    case DSP_FLUSH:
        dsp_surround_flush();
        break;
   case DSP_SET_OUT_FREQUENCY:
        if (!surround_enabled)
            this->process = NULL;
        else
           this->process = dolby_surround_process;
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

