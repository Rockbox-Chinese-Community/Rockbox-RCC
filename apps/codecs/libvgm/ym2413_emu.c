// Game_Music_Emu 0.5.5. http://www.slack.net/~ant/

#include "ym2413_emu.h"

void Ym2413_init( struct Ym2413_Emu* this_ )
{
	this_->last_time = disabled_time; this_->out = 0; 
}

int Ym2413_set_rate( struct Ym2413_Emu* this_, double sample_rate, double clock_rate )
{	
	ym2413_init( &this_->opll, (int) clock_rate, (int) sample_rate, 0 );	
	Ym2413_reset( this_ );
	return 0;
}

void Ym2413_reset( struct Ym2413_Emu* this_ )
{
	ym2413_reset_chip( &this_->opll );
	ym2413_set_mask( &this_->opll, 0 );
}

void Ym2413_write( struct Ym2413_Emu* this_, int addr, int data )
{
	ym2413_write( &this_->opll, 0, addr );
	ym2413_write( &this_->opll, 1, data );
}

void Ym2413_mute_voices( struct Ym2413_Emu* this_, int mask )
{
	ym2413_set_mask( &this_->opll, mask );
}

void Ym2413_run( struct Ym2413_Emu* this_, int pair_count, sample_t* out )
{
	SAMP bufMO[ 1024 ];
	SAMP bufRO[ 1024 ];
	SAMP * buffers[2] = { bufMO, bufRO };

	while (pair_count > 0)
	{
		int todo = pair_count;
		if (todo > 1024) todo = 1024;
		ym2413_update_one( &this_->opll, buffers, todo );

		int i;
		for (i = 0; i < todo; i++)
		{
			int output = bufMO [i];
			output += bufRO [i];
			if ( (short)output != output ) output = 0x7FFF ^ ( output >> 31 );
			out [0] = output;
			out [1] = output;
			out += 2;
		}

		pair_count -= todo;
	}
}
