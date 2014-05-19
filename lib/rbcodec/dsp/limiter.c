#include "rbcodecconfig.h"
#include "fixedpoint.h"
#include "fracmul.h"
#include <string.h>

/* Define LOGF_ENABLE to enable logf output in this file
 * #define LOGF_ENABLE
 */
#include "logf.h"
#include "dsp_proc_entry.h"
#include "limiter.h"
#include "dsp_misc.h"

#define UNITY (1L << 24)                    /* unity gain in S7.24 format */


static int32_t comp_curve[66] IBSS_ATTR;    /* S7.24 format */



/** COMPRESSOR UPDATE
 *  Called via the menu system to configure the compressor process
 */
static bool limiter_update(struct dsp_config *dsp)
{
    (void)dsp;
    /* make settings values useful */
    int  threshold   = 0;

    bool active  = true;

  

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



    return active;
}

/** GET COMPRESSION GAIN
 *  Returns the required gain factor in S7.24 format in order to compress the
 *  sample in accordance with the compression curve.  Always 1 or less.
 */

static inline int32_t get_limiter_gain(struct sample_format *format,
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

/** DSP interface **/

/** 
 *  somehow this setting will alter the sound signiture.
 *  so NEVER call this! Always enable the limiter.
 */

void dsp_set_limiter(int val)
{
    /* enable/disable the compressor depending upon settings */
    struct dsp_config *dsp = dsp_get_config(CODEC_IDX_AUDIO);
    bool enable = (val>0)?true:false;

    if (enable){
        limiter_update(dsp);
        dsp_proc_activate(dsp, DSP_PROC_LIMITER, true);
    }    
    dsp_proc_enable(dsp, DSP_PROC_LIMITER, enable);
    
}

/** COMPRESSOR PROCESS
 *  Changes the gain of the samples according to the compressor curve
 */
static void limiter_process(struct dsp_proc_entry *this,
                               struct dsp_buffer **buf_p)
{
    struct dsp_buffer *buf = *buf_p;
    int count = buf->remcount;
    int32_t *in_buf[2] = { buf->p32[0], buf->p32[1] };
    const int num_chan = buf->format.num_channels;

    struct dsp_config *dsp = dsp_get_config(CODEC_IDX_AUDIO);
    unsigned int fout = dsp_get_output_frequency(dsp);
    int32_t coef = fp_div(2180, fout, 31); 
    int32_t coef2 = fp_div(1600, fout, 31); 
    while (count-- > 0)
    {
        int32_t sample_gain = UNITY;
        int32_t x = 0,y = 0;
        x=*in_buf[0];
        y=*in_buf[1];
        
        x=FRACMUL(x, coef)-FRACMUL(x, coef2);
        y=FRACMUL(y, coef)-FRACMUL(y, coef2);
         
        x = (x>y)?x:y;
        
        sample_gain =get_limiter_gain(&buf->format, x);
         
        if(sample_gain == -1) /*skip those beyong repair*/
        { 
           sample_gain =UNITY; 
        } 

        /*apply the gain to the sample */
        if (sample_gain != UNITY)
        {
            for (int ch = 0; ch < num_chan; ch++)
            {
               *in_buf[ch]  = FRACMUL_SHL(sample_gain, *in_buf[ch], 7);
            }
        }
        in_buf[0]++;
        in_buf[1]++;
    }

    (void)this;
}

/* DSP message hook */
static intptr_t limiter_configure(struct dsp_proc_entry *this,
                                     struct dsp_config *dsp,
                                     unsigned int setting,
                                     intptr_t value)
{
    

    switch (setting)
    {
    case DSP_PROC_INIT:
        if (value != 0)
            break; /* Already enabled */
        
        this->process = limiter_process;
        /* Won't have been getting frequency updates */
        limiter_update(dsp);
        /* Fall-through */
    case DSP_RESET:
    case DSP_FLUSH:

        break;

    case DSP_SET_OUT_FREQUENCY:
        limiter_update(dsp);
        break;
    }

    return 0;
}

/* Database entry */
DSP_PROC_DB_ENTRY(
    LIMITER,
    limiter_configure);
