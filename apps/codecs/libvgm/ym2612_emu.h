// YM2612 FM sound chip emulator

// Game_Music_Emu 0.6-pre
#ifndef YM2612_EMU_H
#define YM2612_EMU_H

#include <limits.h>

#include "blargg_common.h"

#define BUILD_YM2612  1
#define FM_BUSY_FLAG_SUPPORT 0
#define YM2612UpdateReq( chip ) ((void) 0)

/* compiler dependence */
#ifndef __OSDCOMM_H__
#define __OSDCOMM_H__
typedef unsigned char   UINT8;
typedef unsigned short  UINT16;

#if ULONG_MAX == 0xFFFFFFFF
	typedef long            INT32;
	typedef unsigned long   UINT32;
#elif UINT_MAX == 0xFFFFFFFF
	typedef int             INT32;
	typedef unsigned int    UINT32;
#else
	#error "No suitable 32-bit type available"
#endif
#endif

/* struct describing a single operator (SLOT) */
typedef struct
{
	INT32   *DT;        /* detune          :dt_tab[DT] */
	UINT8   KSR;        /* key scale rate  :3-KSR */
	UINT32  ar;         /* attack rate  */
	UINT32  d1r;        /* decay rate   */
	UINT32  d2r;        /* sustain rate */
	UINT32  rr;         /* release rate */
	UINT8   ksr;        /* key scale rate  :kcode>>(3-KSR) */
	UINT32  mul;        /* multiple        :ML_TABLE[ML] */

	/* Phase Generator */
	UINT32  phase;      /* phase counter */
	INT32	Incr;		/* phase step */

	/* Envelope Generator */
	UINT8   state;      /* phase type */
	UINT32  tl;         /* total level: TL << 3 */
	INT32   volume;     /* envelope counter */
	UINT32  sl;         /* sustain level:sl_table[SL] */
	UINT32  vol_out;    /* current output from EG circuit (without AM from LFO) */

	UINT8   eg_sh_ar;   /*  (attack state) */
	UINT8   eg_sel_ar;  /*  (attack state) */
	UINT8   eg_sh_d1r;  /*  (decay state) */
	UINT8   eg_sel_d1r; /*  (decay state) */
	UINT8   eg_sh_d2r;  /*  (sustain state) */
	UINT8   eg_sel_d2r; /*  (sustain state) */
	UINT8   eg_sh_rr;   /*  (release state) */
	UINT8   eg_sel_rr;  /*  (release state) */

	UINT8   ssg;        /* SSG-EG waveform */
	UINT8   ssgn;       /* SSG-EG negated output */

	UINT8   key;        /* 0=last key was KEY OFF, 1=KEY ON */

	/* LFO */
	UINT32  AMmask;     /* AM enable flag */

} FM_SLOT;

typedef struct
{
	FM_SLOT SLOT[4];    /* four SLOTs (operators) */

	UINT8   ALGO;       /* algorithm */
	UINT8   FB;         /* feedback shift */
	INT32   op1_out[2]; /* op1 output for feedback */

	INT32   *connect1;  /* SLOT1 output pointer */
	INT32   *connect3;  /* SLOT3 output pointer */
	INT32   *connect2;  /* SLOT2 output pointer */
	INT32   *connect4;  /* SLOT4 output pointer */

	INT32   *mem_connect;/* where to put the delayed sample (MEM) */
	INT32   mem_value;  /* delayed sample (MEM) value */

	INT32   pms;        /* channel PMS */
	UINT8   ams;        /* channel AMS */

	UINT32  fc;         /* fnum,blk:adjusted to sample rate */
	UINT8   kcode;      /* key code:                        */
	UINT32  block_fnum; /* current blk/fnum value for this slot (can be different betweeen slots of one channel in 3slot mode) */
} FM_CH;


typedef struct
{
	void *  param;      /* this chip parameter  */
	float   clock;      /* master clock  (Hz)   */
	int     rate;       /* sampling rate (Hz)   */
	double  freqbase;   /* frequency base       */
#ifdef FM_BUSY_FLAG_SUPPORT
	double  BusyExpire; /* ExpireTime of Busy clear */
#endif
	UINT16  address;    /* address register     */
	UINT8   irq;        /* interrupt level      */
	UINT8   irqmask;    /* irq mask             */
	UINT8   status;     /* status flag          */
	UINT32  mode;       /* mode  CSM / 3SLOT    */
	UINT8   prescaler_sel;/* prescaler selector */
	UINT8   fn_h;       /* freq latch           */
	INT32   TimerBase;  /* Timer base time      */
	INT32   TA;         /* timer a value        */
	INT32   TAL;        /* timer a base         */
	INT32   TAC;        /* timer a counter      */
	INT32   TB;         /* timer b value        */
	INT32   TBL;        /* timer b base         */
	INT32   TBC;        /* timer b counter      */
	/* Extention Timer and IRQ handler */
	//const struct ssg_callbacks *SSG;
	/* local time tables */
	INT32   dt_tab[8][32];/* DeTune table       */
} FM_ST;

/***********************************************************/
/* OPN unit                                                */
/***********************************************************/

/* OPN 3slot struct */
typedef struct
{
	UINT32  fc[3];          /* fnum3,blk3: calculated */
	UINT8   fn_h;           /* freq3 latch */
	UINT8   kcode[3];       /* key code */
	UINT32  block_fnum[3];  /* current fnum value for this slot (can be different betweeen slots of one channel in 3slot mode) */
	UINT8   key_csm;
} FM_3SLOT;

/* OPN/A/B common state */
typedef struct
{
	FM_ST   ST;             /* general state */
	FM_3SLOT SL3;           /* 3 slot mode state */
	FM_CH   *P_CH;          /* pointer of CH */
	unsigned char pan_regs [6]; /* last pan register write (high two bits) */
	unsigned char pan_mutes [6]; /* external channel *disable* mask to apply to pan registers */
	unsigned int pan[6*2];  /* fm channels output masks (0xffffffff = enable) */

	UINT32  eg_cnt;         /* global envelope generator counter */
	UINT32  eg_timer;       /* global envelope generator counter works at frequency = chipclock/144/3 */
	UINT32  eg_timer_add;   /* step of eg_timer */
	UINT32  eg_timer_overflow;/* envelope generator timer overlfows every 3 samples (on real chip) */

	/* LFO */
	UINT32  lfo_cnt;
	UINT32  lfo_timer;
	UINT32  lfo_timer_add;
	UINT32  lfo_timer_overflow;
	UINT32  LFO_AM;         /* runtime LFO calculations helper */
	INT32   LFO_PM;         /* runtime LFO calculations helper */
	
	/* there are 2048 FNUMs that can be generated using FNUM/BLK registers
		but LFO works with one more bit of a precision so we really need 4096 elements */

	UINT32  fn_table[4096]; /* fnumber->increment counter */

	UINT32  fn_max;

	INT32   m2,c1,c2;       /* Phase Modulation input for operators 2,3,4 */
	INT32   mem;            /* one sample delay memory */

	INT32   out_fm[8];      /* outputs of working channels */
} FM_OPN;

#ifdef BUILD_YM2612
/*******************************************************************************/
/*		YM2612 local section                                                   */
/*******************************************************************************/
/* here's the virtual YM2612 */
struct Ym2612_Impl
{
#ifdef _STATE_H
	UINT8		REGS[512];			/* registers			*/
#endif
	FM_OPN		OPN;				/* OPN state			*/
	FM_CH		CH[6];				/* channel state		*/

	/* dac output (YM2612) */
	int			dacen;
	INT32		dacout;
};
#endif

enum { ym2612_out_chan_count = 2 }; // stereo
enum { ym2612_channel_count = 6 };
enum { ym2612_disabled_time = -1 };

struct Ym2612_Emu  {
	struct Ym2612_Impl impl;
		
	// Impl
	int last_time;
	short* out;
};

static inline void Ym2612_init( struct Ym2612_Emu* this_ )
{ 
	this_->last_time = ym2612_disabled_time; this_->out = 0;
}
	
// Sets sample rate and chip clock rate, in Hz. Returns non-zero
// if error. If clock_rate=0, uses sample_rate*144
const char* Ym2612_set_rate( struct Ym2612_Emu* this_, double sample_rate, double clock_rate );
	
// Resets to power-up state
void Ym2612_reset( struct Ym2612_Emu* this_ );
	
// Mutes voice n if bit n (1 << n) of mask is set
void Ym2612_mute_voices( struct Ym2612_Emu* this_, int mask );
	
// Writes addr to register 0 then data to register 1
void Ym2612_write0( struct Ym2612_Emu* this_, int addr, int data ); ICODE_ATTR
	
// Writes addr to register 2 then data to register 3
void Ym2612_write1( struct Ym2612_Emu* this_, int addr, int data ); ICODE_ATTR
	
// Runs and adds pair_count*2 samples into current output buffer contents
void Ym2612_run( struct Ym2612_Emu* this_, int pair_count, sample_t* out ); ICODE_ATTR

static inline void Ym2612_enable( struct Ym2612_Emu* this_, bool b ) { this_->last_time = b ? 0 : ym2612_disabled_time; }
static inline bool Ym2612_enabled( struct Ym2612_Emu* this_ ) { return this_->last_time != ym2612_disabled_time; }
static inline void Ym2612_begin_frame( struct Ym2612_Emu* this_, short* buf ) { this_->out = buf; this_->last_time = 0; }
		
static inline int Ym2612_run_until( struct Ym2612_Emu* this_, int time )
{
	int count = time - this_->last_time;
	if ( count > 0 )
	{
		if ( this_->last_time < 0 )
			return false;
		this_->last_time = time;
		short* p = this_->out;
		this_->out += count * ym2612_out_chan_count;
		Ym2612_run( this_, count, p );
	}
	return true;
}
#endif
