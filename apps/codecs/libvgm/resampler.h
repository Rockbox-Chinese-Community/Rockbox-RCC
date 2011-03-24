// Combination of Downsampler and Blip_Buffer mixing. Used by Sega FM emulators.

// Game_Music_Emu 0.5.5
#ifndef RESAMPLER_H
#define RESAMPLER_H

#include "blargg_config.h"
#include "multi_buffer.h"

typedef short dsample_t;

enum { stereo = 2 };
enum { max_buf_size = 3960 };
enum { max_resampler_size = 5942 };
enum { write_offset = 8 * stereo };
enum { gain_bits = 14 };

struct Resampler {		
	int (*callback)( void*, blip_time_t, int, dsample_t* );
	void* callback_data;
	
	dsample_t sample_buf [max_buf_size];
	int sample_buf_size;
	int oversamples_per_frame;
	int buf_pos;
	int resampler_size;
	int gain_;
	
	// Internal resampler
	sample_t buf [max_resampler_size];
	int buf_size;
	
	int write_pos;
	double rate_;
	
	int pos;
	int step;
};

static inline void Resampler_init( struct Resampler* this_ )
{
	this_->pos = 0;
	this_->write_pos = 0;
	this_->rate_     = 0;
}

blargg_err_t Resampler_reset( struct Resampler* this_, int max_pairs );
void Resampler_resize( struct Resampler* this_, int pairs_per_frame );
	
void Resampler_play( struct Resampler* this_, long count, dsample_t* out, struct Stereo_Buffer* ); ICODE_ATTR

static inline void Resampler_set_callback(struct Resampler* this_, int (*func)( void*, blip_time_t, int, dsample_t* ), void* user_data )
{
	this_->callback = func;
	this_->callback_data = user_data;
}

blargg_err_t Resampler_setup( struct Resampler* this_, double oversample, double rolloff, double gain );

static inline void Resampler_clear( struct Resampler* this_ )
{
	this_->buf_pos = this_->sample_buf_size;

	this_->pos = 0;
	this_->write_pos = 0;
}

#endif
