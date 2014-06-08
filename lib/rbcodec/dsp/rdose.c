#include "aatube.h"
#include "config.h"
#include "fixedpoint.h"
#include "fracmul.h"
#include "settings.h"
#include "dsp_proc_entry.h"
#include "dsp_misc.h"


#define b0_dly 72    /* ~800 uS */
#define b2_dly 60    /* ~657 uS */
#define b3_dly 276   /* ~3050 uS */


static bool rDose_enabled = false;
static int strength;

static int32_t tcoef1,tcoef2,tcoef3,tcoef4,tcoef5;
static int32_t b0[2][b0_dly],b2[2][b2_dly],b3[2][b3_dly];

static int32_t temp_buffer[2][3072 * 2];

static bool no_repair=false;

static void dsp_rDose_flush(void)
{
    if (!rDose_enabled)
        return;

    memset(b0[0],0,b0_dly * sizeof(int32_t)); 
    memset(b0[1],0,b0_dly * sizeof(int32_t));
    memset(b2[0],0,b2_dly * sizeof(int32_t));
    memset(b2[1],0,b2_dly * sizeof(int32_t));
    memset(b3[0],0,b3_dly * sizeof(int32_t)); 
    memset(b3[1],0,b3_dly * sizeof(int32_t));  
}




#define UNITY (1L << 24)                    /* unity gain in S7.24 format */


static int32_t comp_curve[66] IBSS_ATTR;    /* S7.24 format */

static void rDose_update_filter(unsigned int fout)
{
    tcoef1 = fp_div(160, fout, 31);
    tcoef2 = fp_div(500, fout, 31);
    tcoef3 = fp_div(1150, fout, 31);
    tcoef4 = fp_div(11025, fout, 31);
    tcoef5 = fp_div(20000, fout, 31);
    memset(b0[0],0,b0_dly * sizeof(int32_t)); 
    memset(b0[1],0,b0_dly * sizeof(int32_t));
    memset(b2[0],0,b2_dly * sizeof(int32_t));
    memset(b2[1],0,b2_dly * sizeof(int32_t));
    memset(b3[0],0,b3_dly * sizeof(int32_t)); 
    memset(b3[1],0,b3_dly * sizeof(int32_t));  
}

static void gain_init(struct dsp_config *dsp,int threshold)
{
    (void)dsp;

    /* configure variables for compressor operation */
    static const int32_t db[] = {
        /* positive db equivalents in S15.16 format */
        0x000000, 0x241FA4, 0x1E1A5E, 0x1A94C8,
        0x181518, 0x1624EA, 0x148F82, 0x1338BD,
        0x120FD2, 0x1109EB, 0x101FA4, 0x0F4BB6,
        0x0E8A3C, 0x0DD840, 0x0D3377, 0x0C9A0E,
        0x0C0A8C, 0x0B83BE, 0x0B04A5, 0x0A8C6C,
        0x0A1A5E, 0x09ADE1, 0x094670, 0x08E398,
        0x0884F6, 0x082A30, 0x07D2FA, 0x077F0F,
        0x072E31, 0x06E02A, 0x0694C8, 0x064BDF,
        0x060546, 0x05C0DA, 0x057E78, 0x053E03,
        0x04FF5F, 0x04C273, 0x048726, 0x044D64,
        0x041518, 0x03DE30, 0x03A89B, 0x037448,
        0x03412A, 0x030F32, 0x02DE52, 0x02AE80,
        0x027FB0, 0x0251D6, 0x0224EA, 0x01F8E2,
        0x01CDB4, 0x01A359, 0x0179C9, 0x0150FC,
        0x0128EB, 0x010190, 0x00DAE4, 0x00B4E1,
        0x008F82, 0x006AC1, 0x004699, 0x002305};

    struct curve_point
    {
        int32_t db;     /* S15.16 format */
        int32_t offset; /* S15.16 format */
    } db_curve[5];

    /** Set up the shape of the compression curve first as decibel values
     *  db_curve[0] = bottom of knee
     *          [1] = threshold
     *          [2] = top of knee
     *          [3] = 0 db input
     *          [4] = ~+12db input (2 bits clipping overhead)
     */

    db_curve[1].db = threshold << 16;
    
        /* bottom of knee is at the threshold for hard knee */
        db_curve[0].db = threshold << 16;
        /* top of knee is at the threshold for hard knee */
        db_curve[2].db = threshold << 16;
        db_curve[2].offset = 0;


    /* Calculate 0db and ~+12db offsets */
    db_curve[4].db = 0xC0A8C; /* db of 2 bits clipping */
    
        /* offset = threshold for hard limit */
        db_curve[3].offset = (threshold << 16);
        db_curve[4].offset = -db_curve[4].db + db_curve[3].offset;


    /** Now set up the comp_curve table with compression offsets in the
     * form of gain factors in S7.24 format
     * comp_curve[0] is 0 (-infinity db) input
     */
    comp_curve[0] = UNITY;
    /** comp_curve[1 to 63] are intermediate compression values 
     * corresponding to the 6 MSB of the input values of a non-clipped
     * signal
     */
    for (int i = 1; i < 64; i++)
    {
        /** db constants are stored as positive numbers;
         * make them negative here
         */
        int32_t this_db = -db[i];

        /* no compression below the knee */
        if (this_db <= db_curve[0].db)
            comp_curve[i] = UNITY;


        /* interpolate along ratio slope above the knee */
        else
            comp_curve[i] = fp_factor(fp_mul(
                fp_div((db_curve[1].db - this_db), db_curve[1].db, 16),
                db_curve[3].offset, 16), 16) << 8;
    }
    /** comp_curve[64] is the compression level of a maximum level,
     * non-clipped signal
     */
    comp_curve[64] = fp_factor(db_curve[3].offset, 16) << 8;

    /** comp_curve[65] is the compression level of a maximum level,
     * clipped signal
     */
    comp_curve[65] = fp_factor(db_curve[4].offset, 16) << 8;
}

static inline int32_t get_gain(struct sample_format *format,
                                           int32_t sample)
{
    const int frac_bits_offset = format->frac_bits - 15;

    /* sample must be positive */
    if (sample < 0)
        sample = -(sample + 1);

    /* shift sample into 15 frac bit range */
    if (frac_bits_offset > 0)
        sample >>= frac_bits_offset;
    if (frac_bits_offset < 0)
        sample <<= -frac_bits_offset;

    /* normal case: sample isn't clipped */
    if (sample < (1 << 15))
    {
        /* index is 6 MSB, rem is 9 LSB */
        int index = sample >> 9;
        int32_t rem = (sample & 0x1FF) << 22;

        /** interpolate from the compression curve:
         * higher gain - ((rem / (1 << 31)) * (higher gain - lower gain))
         */
        return comp_curve[index] - (FRACMUL(rem,
            (comp_curve[index] - comp_curve[index + 1])));
    }
    /* sample is somewhat clipped, up to 2 bits of overhead */
    if (sample < (1 << 17))
    {
        /** straight interpolation:
         *  higher gain - ((clipped portion of sample * 4/3
         *  / (1 << 31)) * (higher gain - lower gain))
         */
        return comp_curve[64] - (FRACMUL(((sample - (1 << 15)) / 3) << 16,
            (comp_curve[64] - comp_curve[65])));
    }
    /* sample is too clipped, return invalid value */
    return -1;
}

void dsp_rdose_enable(int var)
{
    struct dsp_config *dsp = dsp_get_config(CODEC_IDX_AUDIO);
    static int level[4] = {0,0,-3,-3};
    bool prev_rDose_enabled = rDose_enabled;
    
    strength = var;
    rDose_enabled=(var > 0)?  true:false;

    no_repair = (strength > 1)?false:true;
    
    gain_init(dsp,level[strength]);
 
    if (prev_rDose_enabled == rDose_enabled)
        return; /* No change */
    
    dsp_proc_enable(dsp, DSP_PROC_RDOSE, rDose_enabled);
}

static void rDose_process(struct dsp_proc_entry *this,
                               struct dsp_buffer **buf_p)
{

    (void)this;
    int32_t x,y;
    int32_t i;
    struct dsp_buffer *buf = *buf_p;
    int count = buf->remcount;

    
    if (count < b3_dly )
    {
        dsp_rDose_flush(); 
        return;  
    }


    //add 160hz - 500hz no delay
     for (i = 0; i < count; i++)
     {
         temp_buffer[0][i]= FRACMUL(buf->p32[0][i], tcoef1) - FRACMUL(buf->p32[0][i], tcoef2);
         temp_buffer[1][i]= FRACMUL(buf->p32[1][i], tcoef1) - FRACMUL(buf->p32[1][i], tcoef2);
         
     }
  
    //filter below 160 and with delays
     for (i = 0; i < b0_dly; i++)
     {
          temp_buffer[0][i] +=b0[0][i];
          temp_buffer[1][i] +=b0[1][i];
     }
     for (i = 0; i < count-b0_dly; i++)
     {
         temp_buffer[0][i+b0_dly] += buf->p32[0][i] - FRACMUL(buf->p32[0][i], tcoef1);
         temp_buffer[1][i+b0_dly] += buf->p32[1][i] - FRACMUL(buf->p32[1][i], tcoef1);
     }
     for (i = 0; i < b0_dly; i++)
     {
          b0[0][i] = buf->p32[0][count-b0_dly+i] - FRACMUL(buf->p32[0][count-b0_dly+i], tcoef1);
          b0[1][i] = buf->p32[1][count-b0_dly+i] - FRACMUL(buf->p32[1][count-b0_dly+i], tcoef1);
     }
     
     //add 500-1150hz with delays
     for (i = 0; i < b2_dly; i++)
     {
          temp_buffer[0][i] +=b2[0][i];
          temp_buffer[1][i] +=b2[1][i];
     }
     for (i = 0; i < count-b2_dly; i++)
     {
         temp_buffer[0][i+b2_dly]+= FRACMUL(buf->p32[0][i], tcoef2) - FRACMUL(buf->p32[0][i], tcoef3);
         temp_buffer[1][i+b2_dly]+= FRACMUL(buf->p32[1][i], tcoef2) - FRACMUL(buf->p32[1][i], tcoef3);
     } 
     for (i = 0; i < b2_dly; i++)
     {
          b2[0][i] = FRACMUL(buf->p32[0][count-b2_dly+i], tcoef2) - FRACMUL(buf->p32[0][count-b2_dly+i], tcoef3);
          b2[1][i] = FRACMUL(buf->p32[1][count-b2_dly+i], tcoef2) - FRACMUL(buf->p32[1][count-b2_dly+i], tcoef3);
     }

     //add above 1150 with delays
     for (i = 0; i < b3_dly; i++)
     {
         temp_buffer[0][i] +=b3[0][i];
         temp_buffer[1][i] +=b3[1][i];
     }
     for (i = 0; i < count-b3_dly; i++)
     {
         temp_buffer[0][i+b3_dly]+= FRACMUL(buf->p32[0][i], tcoef3);
         temp_buffer[1][i+b3_dly]+= FRACMUL(buf->p32[1][i], tcoef3);
     } 
     for (i = 0; i < b3_dly; i++)
     {
         b3[0][i] = FRACMUL(buf->p32[0][count-b3_dly+i], tcoef3);
         b3[1][i] = FRACMUL(buf->p32[1][count-b3_dly+i], tcoef3);
     }
     
     //check gain from 1150hz-11025hz and apply this gain to frequency above 20khz
     int32_t sample_gain = UNITY;
     if (no_repair == false)
     {
        for (i = 0; i < count; i++)
        {
            x = FRACMUL(temp_buffer[0][i], tcoef3) - FRACMUL(temp_buffer[0][i], tcoef4);
            y = FRACMUL(temp_buffer[1][i], tcoef3) - FRACMUL(temp_buffer[1][i], tcoef4);
 
            x = (x>y)?x:y;
 
            sample_gain = get_gain(&buf->format, x);
            if (sample_gain != -1)
            {
                x = FRACMUL(temp_buffer[0][i], tcoef5);
                y = FRACMUL(temp_buffer[1][i], tcoef5);
                if (strength == 3)
                {
                   if (FRACMUL_SHL(sample_gain, x, 7) + x != -1 &&
                       FRACMUL_SHL(sample_gain, y, 7) + y != -1)
                   temp_buffer[0][i]+=FRACMUL_SHL(sample_gain, x, 7);
                   temp_buffer[1][i]+=FRACMUL_SHL(sample_gain, y, 7);
                }
                else
                {   
                    temp_buffer[0][i]+=FRACMUL_SHL(sample_gain, x, 7);
                    temp_buffer[1][i]+=FRACMUL_SHL(sample_gain, y, 7);
                }
            }
         } 
     }
     /*end of repair*/
   
     memcpy(buf->p32[0],temp_buffer[0],count * sizeof(int32_t));
     memcpy(buf->p32[1],temp_buffer[1],count * sizeof(int32_t));
    
}

/* DSP message hook */
static intptr_t rDose_configure(struct dsp_proc_entry *this,
                                     struct dsp_config *dsp,
                                     unsigned int setting,
                                     intptr_t value)
{
    unsigned int fout = dsp_get_output_frequency(dsp);
    switch (setting)
    {
    case DSP_PROC_INIT:
        if (value != 0)
            break; /* Already enabled */
        this->process = rDose_process;
        rDose_update_filter(fout);
        gain_init(dsp,0);
        dsp_proc_activate(dsp, DSP_PROC_RDOSE, true);
        break;
    case DSP_FLUSH:
        dsp_rDose_flush();
        break;
    case DSP_SET_OUT_FREQUENCY:
        if (!rDose_enabled)
           this->process =NULL;
        else
           this->process = rDose_process; 
        break; 
    case DSP_PROC_CLOSE:
        break;
    }

    return 1;
    (void)dsp;
}

/* Database entry */
DSP_PROC_DB_ENTRY(
    RDOSE,
    rDose_configure);
