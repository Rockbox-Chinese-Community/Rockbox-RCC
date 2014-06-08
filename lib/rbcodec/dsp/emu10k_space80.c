#include "emu10k_space80.h"
#include "emu10k_core.h"

static struct space80_data_e
{
    int32_t dl_0[728];
    int32_t dl_1[700];
    int32_t dl_2[680];
    int32_t dl_3[654];
    int32_t dl_4[610];
    int32_t dl_5[587];
    int32_t dl_6[13];
    int32_t dl_7[12];
} space80_data_e;

static struct space80_data
{
    int32_t c_80_decay;
    int32_t c_80_freq;
    int32_t c_80_gain;
    int32_t c_80_mix;

    int32_t *idx_80_w0;
    int32_t *idx_80_r0;
    int32_t *idx_80_w1;
    int32_t *idx_80_r1;
    int32_t *idx_80_w2;
    int32_t *idx_80_r2;
    int32_t *idx_80_w3;
    int32_t *idx_80_r3;
    int32_t *idx_80_w4;
    int32_t *idx_80_r4;
    int32_t *idx_80_w5;
    int32_t *idx_80_r5;
    int32_t *idx_80_w6;
    int32_t *idx_80_r6;
    int32_t *idx_80_w7;
    int32_t *idx_80_r7;

    struct dsp_config *dsp;
#ifdef CPU_MIPS
} space80_data = 
#else
} space80_data IDATA_ATTR = 
#endif
{
    .c_80_decay = 0x73333333,
    .c_80_freq = 0x1fffffff,
    .c_80_gain = 0x3fffffff,
    .c_80_mix = 0x33333333,
    .idx_80_w0 = space80_data_e.dl_0,
    .idx_80_r0 = space80_data_e.dl_0+1,
    .idx_80_w1 = space80_data_e.dl_1,
    .idx_80_r1 = space80_data_e.dl_1+1,
    .idx_80_w2 = space80_data_e.dl_2,
    .idx_80_r2 = space80_data_e.dl_2+1,
    .idx_80_w3 = space80_data_e.dl_3,
    .idx_80_r3 = space80_data_e.dl_3+1,
    .idx_80_w4 = space80_data_e.dl_4,
    .idx_80_r4 = space80_data_e.dl_4+1,
    .idx_80_w5 = space80_data_e.dl_5,
    .idx_80_r5 = space80_data_e.dl_5+1,
    .idx_80_w6 = space80_data_e.dl_6,
    .idx_80_r6 = space80_data_e.dl_6+1,
    .idx_80_w7 = space80_data_e.dl_7,
    .idx_80_r7 = space80_data_e.dl_7+1,
};

static bool space80_enable = false;

static void space80_flush(void)
{
    if (!space80_enable)
        return;

    memset(space80_data_e.dl_0, 0, sizeof(space80_data_e.dl_0));
    memset(space80_data_e.dl_1, 0, sizeof(space80_data_e.dl_1));
    memset(space80_data_e.dl_2, 0, sizeof(space80_data_e.dl_2));
    memset(space80_data_e.dl_3, 0, sizeof(space80_data_e.dl_3));
    memset(space80_data_e.dl_4, 0, sizeof(space80_data_e.dl_4));
    memset(space80_data_e.dl_5, 0, sizeof(space80_data_e.dl_5));
    memset(space80_data_e.dl_6, 0, sizeof(space80_data_e.dl_6));
    memset(space80_data_e.dl_7, 0, sizeof(space80_data_e.dl_7));
}

void dsp_set_space80_enable(bool enable)
{
    
    space80_enable = enable;

    struct dsp_config *dsp = dsp_get_config(CODEC_IDX_AUDIO);
    dsp_proc_enable(dsp, DSP_PROC_SPACE80, enable);

    if(space80_enable && !dsp_proc_active(dsp,DSP_PROC_SPACE80))
        dsp_proc_activate(dsp, DSP_PROC_SPACE80, true);
    if(!space80_enable && dsp_proc_active(dsp,DSP_PROC_SPACE80))
        dsp_proc_activate(dsp, DSP_PROC_SPACE80, false);
}

void space80_process(struct dsp_proc_entry *this, struct dsp_buffer **buf_p)
{
    struct dsp_buffer *buf = *buf_p;
    int count = buf->remcount;
    int32_t c_decay = space80_data.c_80_decay;
    int32_t c_freq = space80_data.c_80_freq;
    int32_t c_gain = space80_data.c_80_gain;
    int32_t c_mix = space80_data.c_80_mix;

    int32_t *idx_dl0  = &space80_data_e.dl_0[0] + 728;
    int32_t *idx_dl1  = &space80_data_e.dl_1[0] + 700;
    int32_t *idx_dl2  = &space80_data_e.dl_2[0] + 680;
    int32_t *idx_dl3  = &space80_data_e.dl_3[0] + 654;
    int32_t *idx_dl4  = &space80_data_e.dl_4[0] + 610;
    int32_t *idx_dl5  = &space80_data_e.dl_5[0] + 587;
    int32_t *idx_dl6  = &space80_data_e.dl_6[0] + 13;
    int32_t *idx_dl7  = &space80_data_e.dl_7[0] + 12;

    int32_t *idx_w0  = space80_data.idx_80_w0;
    int32_t *idx_r0  = space80_data.idx_80_r0;
    int32_t *idx_w1  = space80_data.idx_80_w1;
    int32_t *idx_r1  = space80_data.idx_80_r1;
    int32_t *idx_w2  = space80_data.idx_80_w2;
    int32_t *idx_r2  = space80_data.idx_80_r2;
    int32_t *idx_w3  = space80_data.idx_80_w3;
    int32_t *idx_r3  = space80_data.idx_80_r3;
    int32_t *idx_w4  = space80_data.idx_80_w4;
    int32_t *idx_r4  = space80_data.idx_80_r4;
    int32_t *idx_w5  = space80_data.idx_80_w5;
    int32_t *idx_r5  = space80_data.idx_80_r5;
    int32_t *idx_w6  = space80_data.idx_80_w6;
    int32_t *idx_r6  = space80_data.idx_80_r6;
    int32_t *idx_w7  = space80_data.idx_80_w7;
    int32_t *idx_r7  = space80_data.idx_80_r7;

    int32_t t_wet, t_c, t_d[2];

    int i;

    for (i = 0; i < count; i++)
    {
        t_wet = FRACMUL((buf->p32[0][i] >> 4) + (buf->p32[0][i] >> 6) + (buf->p32[1][i] >> 4) + (buf->p32[1][i] >> 6), c_mix);
        buf->p32[0][i] -=  buf->p32[0][i] >> 4;
        buf->p32[1][i] -=  buf->p32[1][i] >> 4;
        buf->p32[0][i] -= FRACMUL(buf->p32[0][i], c_mix);
        buf->p32[1][i] -= FRACMUL(buf->p32[1][i], c_mix);
        *idx_w0 = t_wet + FRACMUL(*idx_r0, c_decay);
        *idx_w1 = t_wet + FRACMUL(*idx_r1, c_decay);
        *idx_w2 = t_wet + FRACMUL(*idx_r2, c_decay);
        *idx_w3 = t_wet + FRACMUL(*idx_r3, c_decay);
        *idx_w4 = t_wet + FRACMUL(*idx_r4, c_decay);
        *idx_w5 = t_wet + FRACMUL(*idx_r5, c_decay);
        t_c = FRACMUL(*idx_r0 + *idx_r1 + *idx_r2 + *idx_r3 + *idx_r4 + *idx_r5, c_gain);
        *idx_w6 = t_c - FRACMUL(*idx_r6, c_freq);
        t_d[0] = *idx_r6 + FRACMUL(*idx_w6, c_freq);
        *idx_w7 = t_d[0] - FRACMUL(*idx_r7, c_freq);
        t_d[1] = *idx_r7 + FRACMUL(*idx_w7, c_freq);
        buf->p32[0][i] += t_d[1];
        buf->p32[1][i] -= t_d[1];

        if(++idx_w0 >= idx_dl0) idx_w0 = idx_dl0 - 728;
        if(++idx_r0 >= idx_dl0) idx_r0 = idx_dl0 - 728;
        if(++idx_w1 >= idx_dl1) idx_w1 = idx_dl1 - 700;
        if(++idx_r1 >= idx_dl1) idx_r1 = idx_dl1 - 700;
        if(++idx_w2 >= idx_dl2) idx_w2 = idx_dl2 - 680;
        if(++idx_r2 >= idx_dl2) idx_r2 = idx_dl2 - 680;
        if(++idx_w3 >= idx_dl3) idx_w3 = idx_dl3 - 654;
        if(++idx_r3 >= idx_dl3) idx_r3 = idx_dl3 - 654;
        if(++idx_w4 >= idx_dl4) idx_w4 = idx_dl4 - 610;
        if(++idx_r4 >= idx_dl4) idx_r4 = idx_dl4 - 610;
        if(++idx_w5 >= idx_dl5) idx_w5 = idx_dl5 - 587;
        if(++idx_r5 >= idx_dl5) idx_r5 = idx_dl5 - 587;
        if(++idx_w6 >= idx_dl6) idx_w6 = idx_dl6 - 13;
        if(++idx_r6 >= idx_dl6) idx_r6 = idx_dl6 - 13;
        if(++idx_w7 >= idx_dl7) idx_w7 = idx_dl7 - 12;
        if(++idx_r7 >= idx_dl7) idx_r7 = idx_dl7 - 12;
    }
    space80_data.idx_80_w0 = idx_w0;
    space80_data.idx_80_r0 = idx_r0;
    space80_data.idx_80_w1 = idx_w1;
    space80_data.idx_80_r1 = idx_r1;
    space80_data.idx_80_w2 = idx_w2;
    space80_data.idx_80_r2 = idx_r2;
    space80_data.idx_80_w3 = idx_w3;
    space80_data.idx_80_r3 = idx_r3;
    space80_data.idx_80_w4 = idx_w4;
    space80_data.idx_80_r4 = idx_r4;
    space80_data.idx_80_w5 = idx_w5;
    space80_data.idx_80_r5 = idx_r5;
    space80_data.idx_80_w6 = idx_w6;
    space80_data.idx_80_r6 = idx_r6;
    space80_data.idx_80_w7 = idx_w7;
    space80_data.idx_80_r7 = idx_r7;

    (void)this;
}

void dsp_set_space80_params(long decay, long freq, long gain, long mix)
{
    space80_data.c_80_decay = emu10k_log15(0x5c28f5c * (decay / 5));
    space80_data.c_80_freq = 0x6666666 * (freq / 5);
    space80_data.c_80_gain = 0x6666666 * (gain / 5);
    space80_data.c_80_mix = 0x6666666 * (mix / 5);
}

static intptr_t space80_configure(struct dsp_proc_entry *this, struct dsp_config *dsp, unsigned int setting, intptr_t value)
{
    switch (setting)
    {
    case DSP_PROC_INIT:
        if (value != 0)
        {
            this->data = (intptr_t)&space80_data;
            ((struct space80_data *)this->data)->dsp = dsp;
        }
        this->process = space80_process;
        //dsp_proc_activate(dsp, DSP_PROC_SPACE80, true);
        break;
    case DSP_FLUSH:
        space80_flush();
        break;
    case DSP_PROC_CLOSE:
        ((struct space80_data *)this->data)->dsp = NULL;
        break;
    }
    return 1;
    (void)dsp;
}

/* Database entry */
DSP_PROC_DB_ENTRY(
    SPACE80,
    space80_configure);
