#pragma once

#ifndef __YM2413_H__
#define __YM2413_H__

#ifdef __cplusplus
extern "C" {
#endif

/* select output bits size of output : 8 or 16 */
#define SAMPLE_BITS 16

/* compiler dependence */
#ifndef __OSDCOMM_H__
#define __OSDCOMM_H__
typedef unsigned char	UINT8;   /* unsigned  8bit */
typedef unsigned short	UINT16;  /* unsigned 16bit */
typedef unsigned int	UINT32;  /* unsigned 32bit */
typedef signed char		INT8;    /* signed  8bit   */
typedef signed short	INT16;   /* signed 16bit   */
typedef signed int		INT32;   /* signed 32bit   */

typedef INT32           stream_sample_t;
#endif

typedef stream_sample_t SAMP;
/*
#if (SAMPLE_BITS==16)
typedef INT16 SAMP;
#endif
#if (SAMPLE_BITS==8)
typedef INT8 SAMP;
#endif
*/

typedef struct{
	UINT32	ar;			/* attack rate: AR<<2           */
	UINT32	dr;			/* decay rate:  DR<<2           */
	UINT32	rr;			/* release rate:RR<<2           */
	UINT8	KSR;		/* key scale rate               */
	UINT8	ksl;		/* keyscale level               */
	UINT8	ksr;		/* key scale rate: kcode>>KSR   */
	UINT8	mul;		/* multiple: mul_tab[ML]        */

	/* Phase Generator */
	UINT32	phase;		/* frequency counter            */
	UINT32	freq;		/* frequency counter step       */
	UINT8   fb_shift;	/* feedback shift value         */
	INT32   op1_out[2];	/* slot1 output for feedback    */

	/* Envelope Generator */
	UINT8	eg_type;	/* percussive/nonpercussive mode*/
	UINT8	state;		/* phase type                   */
	UINT32	TL;			/* total level: TL << 2         */
	INT32	TLL;		/* adjusted now TL              */
	INT32	volume;		/* envelope counter             */
	UINT32	sl;			/* sustain level: sl_tab[SL]    */

	UINT8	eg_sh_dp;	/* (dump state)                 */
	UINT8	eg_sel_dp;	/* (dump state)                 */
	UINT8	eg_sh_ar;	/* (attack state)               */
	UINT8	eg_sel_ar;	/* (attack state)               */
	UINT8	eg_sh_dr;	/* (decay state)                */
	UINT8	eg_sel_dr;	/* (decay state)                */
	UINT8	eg_sh_rr;	/* (release state for non-perc.)*/
	UINT8	eg_sel_rr;	/* (release state for non-perc.)*/
	UINT8	eg_sh_rs;	/* (release state for perc.mode)*/
	UINT8	eg_sel_rs;	/* (release state for perc.mode)*/

	UINT32	key;		/* 0 = KEY OFF, >0 = KEY ON     */

	/* LFO */
	UINT32	AMmask;		/* LFO Amplitude Modulation enable mask */
	UINT8	vib;		/* LFO Phase Modulation enable flag (active high)*/

	/* waveform select */
	unsigned int wavetable;
} OPLL_SLOT;

typedef struct{
	OPLL_SLOT SLOT[2];
	/* phase generator state */
	UINT32  block_fnum;	/* block+fnum                   */
	UINT32  fc;			/* Freq. freqement base         */
	UINT32  ksl_base;	/* KeyScaleLevel Base step      */
	UINT8   kcode;		/* key code (for key scaling)   */
	UINT8   sus;		/* sus on/off (release speed in percussive mode)*/
} OPLL_CH;

/* chip state */
typedef struct {
	OPLL_CH	P_CH[9];				/* OPLL chips have 9 channels*/
	UINT8	instvol_r[9];			/* instrument/volume (or volume/volume in percussive mode)*/

	UINT32	eg_cnt;					/* global envelope generator counter    */
	UINT32	eg_timer;				/* global envelope generator counter works at frequency = chipclock/72 */
	UINT32	eg_timer_add;			/* step of eg_timer                     */
	UINT32	eg_timer_overflow;		/* envelope generator timer overlfows every 1 sample (on real chip) */

	UINT8	rhythm;					/* Rhythm mode                  */

	/* LFO */
	UINT32	lfo_am_cnt;
	UINT32	lfo_am_inc;
	UINT32	lfo_pm_cnt;
	UINT32	lfo_pm_inc;

	UINT32	noise_rng;				/* 23 bit noise shift register  */
	UINT32	noise_p;				/* current noise 'phase'        */
	UINT32	noise_f;				/* current noise period         */


/* instrument settings */
/*
    0-user instrument
    1-15 - fixed instruments
    16 -bass drum settings
    17,18 - other percussion instruments
*/
	UINT8 inst_tab[19][8];

	UINT32	fn_tab[1024];			/* fnumber->increment counter   */

	UINT8 address;					/* address register             */
	UINT8 status;					/* status flag                  */

	int clock;						/* master clock  (Hz)           */
	int rate;						/* sampling rate (Hz)           */
	double freqbase;				/* frequency base               */

	/* work table */
	OPLL_SLOT *SLOT7_1,*SLOT7_2,*SLOT8_1,*SLOT8_2;

	signed int output[2];

	UINT32	LFO_AM;
	INT32	LFO_PM;

	int chip_type;
	UINT32 mask;
} YM2413;

void ym2413_init(YM2413* chip, int clock, int rate, int type);
void ym2413_reset_chip(YM2413 *chip);
void ym2413_write(YM2413 *chip, int a, int v); ICODE_ATTR
unsigned char ym2413_read(YM2413 *chip, int a); ICODE_ATTR
void ym2413_update_one(YM2413 *chip, SAMP **buffers, int length); ICODE_ATTR

void ym2413_advance_lfo(YM2413 *chip); ICODE_ATTR   /* call this_ once */
SAMP ym2413_calcch(YM2413 *chip, int ch); ICODE_ATTR/* then call this_ for each channel */
void ym2413_advance(YM2413 *chip); ICODE_ATTR       /* then call this_ */

void * ym2413_get_inst0(YM2413 *chip); ICODE_ATTR

void ym2413_set_mask(YM2413 *chip, UINT32 mask); ICODE_ATTR

#ifdef __cplusplus
}
#endif

#endif /*__YM2413_H__*/
