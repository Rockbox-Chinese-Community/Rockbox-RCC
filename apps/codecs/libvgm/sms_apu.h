// Sega Master System SN76489 PSG sound chip emulator

// Sms_Snd_Emu 0.1.2
#ifndef SMS_APU_H
#define SMS_APU_H

#include "blargg_common.h"
#include "blip_buffer.h"

// 0: Square 1, 1: Square 2, 2: Square 3, 3: Noise
enum { osc_count = 4 }; // 0 <= chan < osc_count
	
struct Osc
{
	struct Blip_Buffer* outputs [4]; // NULL, right, left, center
	struct Blip_Buffer* output;
	int          last_amp;
	
	int         volume;
	int         period;
	int         delay;
	unsigned    phase;
};

struct Sms_Apu {
	struct Osc     oscs [osc_count];
	int     ggstereo;
	int     latch;
	
	blip_time_t last_time;
	int         min_tone_period;
	unsigned    noise_feedback;
	unsigned    looped_feedback;
	struct Blip_Synth synth;
};

// Basics

void Apu_init( struct Sms_Apu* this );

// Sets buffer(s) to generate sound into, or 0 to mute. If only center is not 0,
// output is mono.
void Apu_set_output( struct Sms_Apu* this, int i, struct Blip_Buffer* center, struct Blip_Buffer* left, struct Blip_Buffer* right);

// Emulates to time t, then writes data to Game Gear left/right assignment byte
void Apu_write_ggstereo( struct Sms_Apu* this, blip_time_t t, int data ); ICODE_ATTR
	
// Emulates to time t, then writes data
void Apu_write_data( struct Sms_Apu* this, blip_time_t t, int data ); ICODE_ATTR
	
// Emulates to time t, then subtracts t from the current time.
// OK if previous write call had time slightly after t.
void Apu_end_frame( struct Sms_Apu* this, blip_time_t t ); ICODE_ATTR

// More features

// Resets sound chip and sets noise feedback bits and width
void Apu_reset( struct Sms_Apu* this, unsigned noise_feedback, int noise_width );
		
// Sets overall volume, where 1.0 is normal
void Apu_volume( struct Sms_Apu* this, double vol );

#endif
