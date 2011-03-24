// YM2413 FM sound chip emulator interface

// Game_Music_Emu 0.6-pre
#ifndef YM2413_EMU_H
#define YM2413_EMU_H

#include "blargg_common.h"

#include "ym2413.h"

enum { out_chan_count = 2 }; // stereo
enum { channel_count = 14 };
enum { disabled_time = -1 };

struct Ym2413_Emu  {
	YM2413 opll;
		
	// Impl
	int last_time;
	short* out;
};

void Ym2413_init( struct Ym2413_Emu* this_ );

static inline bool Ym2413_supported( void ) { return true; }
	
// Sets output sample rate and chip clock rates, in Hz. Returns non-zero
// if error.
int Ym2413_set_rate( struct Ym2413_Emu* this_, double sample_rate, double clock_rate );
	
// Resets to power-up state
void Ym2413_reset( struct Ym2413_Emu* this_ );
	
// Mutes voice n if bit n (1 << n) of mask is set
void Ym2413_mute_voices( struct Ym2413_Emu* this_, int mask );
	
// Writes data to addr
void Ym2413_write( struct Ym2413_Emu* this_, int addr, int data ); ICODE_ATTR
	
// Runs and writes pair_count*2 samples to output
void Ym2413_run( struct Ym2413_Emu* this_, int pair_count, sample_t* out ); ICODE_ATTR

static inline void Ym2413_enable( struct Ym2413_Emu* this_, bool b ) { this_->last_time = b ? 0 : disabled_time; }
static inline bool Ym2413_enabled( struct Ym2413_Emu* this_ ) { return this_->last_time != disabled_time; }
static inline void Ym2413_begin_frame( struct Ym2413_Emu* this_, short* buf ) { this_->out = buf; this_->last_time = 0; }
	
static inline int Ym2413_run_until( struct Ym2413_Emu* this_, int time )
{
	int count = time - this_->last_time;
	if ( count > 0 )
	{
		if ( this_->last_time < 0 )
			return false;
		this_->last_time = time;
		short* p = this_->out;
		this_->out += count * out_chan_count;
		Ym2413_run( this_, count, p );
	}
	return true;
}

#endif
