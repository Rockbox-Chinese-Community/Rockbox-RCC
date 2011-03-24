// Game_Music_Emu 0.5.5. http://www.slack.net/~ant/

#include "resampler.h"

#include <stdlib.h>
#include <string.h>

/* Copyright (C) 2003-2006 Shay Green. This module is free software; you
can redistribute it and/or modify it under the terms of the GNU Lesser
General Public License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version. This
module is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
details. You should have received a copy of the GNU Lesser General Public
License along with this module; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA */

#include "blargg_source.h"

// TODO: fix this. hack since resampler holds back some output.
unsigned const resampler_extra = 34;

enum { shift = 14 };
int const unit = 1 << shift;

blargg_err_t Resampler_setup( struct Resampler* this_, double oversample, double rolloff, double gain )
{
	(void) rolloff;
	
	this_->gain_ = (int)((1 << gain_bits) * gain);
	this_->step = (int) ( oversample * unit + 0.5);
	this_->rate_ = 1.0 / unit * this_->step;
	return 0;
}

blargg_err_t Resampler_reset( struct Resampler* this_, int pairs )
{
	// expand allocations a bit
	Resampler_resize( this_, pairs );
	this_->resampler_size = this_->oversamples_per_frame + (this_->oversamples_per_frame >> 2);

	this_->buf_size = this_->resampler_size;
	this_->pos = 0;
	this_->write_pos = 0;
	return 0;
}

void Resampler_resize( struct Resampler* this_, int pairs )
{
	int new_sample_buf_size = pairs * 2;
	if ( this_->sample_buf_size != new_sample_buf_size )
	{
		this_->sample_buf_size = new_sample_buf_size;
		this_->oversamples_per_frame = (int) (pairs * this_->rate_) * 2 + 2;
		Resampler_clear( this_ );
	}
}

void mix_mono( struct Resampler* this_, struct Stereo_Buffer* stereo_buf, dsample_t* out_ )
{
	int const bass = BLIP_READER_BASS( stereo_buf->bufs [0] );
	BLIP_READER_BEGIN( sn, stereo_buf->bufs [0] );
	
	int count = this_->sample_buf_size >> 1;
	BLIP_READER_ADJ_( sn, count );
	
	typedef dsample_t stereo_dsample_t [2];
	stereo_dsample_t* BLARGG_RESTRICT out = (stereo_dsample_t*) out_ + count;
	stereo_dsample_t const* BLARGG_RESTRICT in =
			(stereo_dsample_t const*) this_->sample_buf + count;
	int offset = -count;
	int const gain = this_->gain_;
	do
	{
		int s = BLIP_READER_READ_RAW( sn ) >> (blip_sample_bits - 16);
		BLIP_READER_NEXT_IDX_( sn, bass, offset );
		
		int l = (in [offset] [0] * gain >> gain_bits) + s;
		int r = (in [offset] [1] * gain >> gain_bits) + s;
		
		BLIP_CLAMP( l, l );
		out [offset] [0] = (blip_sample_t) l;
		
		BLIP_CLAMP( r, r );
		out [offset] [1] = (blip_sample_t) r;
	}
	while ( ++offset );
	
	BLIP_READER_END( sn, stereo_buf->bufs [0] );
}

void mix_stereo( struct Resampler* this_, struct Stereo_Buffer* stereo_buf, dsample_t* out_ )
{
	int const bass = BLIP_READER_BASS( stereo_buf->bufs [0] );
	BLIP_READER_BEGIN( snc, stereo_buf->bufs [0] );
	BLIP_READER_BEGIN( snl, stereo_buf->bufs [1] );
	BLIP_READER_BEGIN( snr, stereo_buf->bufs [2] );
	
	int count = this_->sample_buf_size >> 1;
	BLIP_READER_ADJ_( snc, count );
	BLIP_READER_ADJ_( snl, count );
	BLIP_READER_ADJ_( snr, count );
	
	typedef dsample_t stereo_dsample_t [2];
	stereo_dsample_t* BLARGG_RESTRICT out = (stereo_dsample_t*) out_ + count;
	stereo_dsample_t const* BLARGG_RESTRICT in =
			(stereo_dsample_t const*) this_->sample_buf + count;
	int offset = -count;
	int const gain = this_->gain_;
	do
	{
		int sc = BLIP_READER_READ_RAW( snc ) >> (blip_sample_bits - 16);
		int sl = BLIP_READER_READ_RAW( snl ) >> (blip_sample_bits - 16);
		int sr = BLIP_READER_READ_RAW( snr ) >> (blip_sample_bits - 16);
		BLIP_READER_NEXT_IDX_( snc, bass, offset );
		BLIP_READER_NEXT_IDX_( snl, bass, offset );
		BLIP_READER_NEXT_IDX_( snr, bass, offset );
		
		int l = (in [offset] [0] * gain >> gain_bits) + sl + sc;
		int r = (in [offset] [1] * gain >> gain_bits) + sr + sc;
		
		BLIP_CLAMP( l, l );
		out [offset] [0] = (blip_sample_t) l;
		
		BLIP_CLAMP( r, r );
		out [offset] [1] = (blip_sample_t) r;
	}
	while ( ++offset );
	
	BLIP_READER_END( snc, stereo_buf->bufs [0] );
	BLIP_READER_END( snl, stereo_buf->bufs [1] );
	BLIP_READER_END( snr, stereo_buf->bufs [2] );
}

void mix_stereo_no_center( struct Resampler* this_, struct Stereo_Buffer* stereo_buf, dsample_t* out_ )
{
	int const bass = BLIP_READER_BASS( stereo_buf->bufs [0] );
	BLIP_READER_BEGIN( snl, stereo_buf->bufs [1] );
	BLIP_READER_BEGIN( snr, stereo_buf->bufs [2] );
	
	int count = this_->sample_buf_size >> 1;
	BLIP_READER_ADJ_( snl, count );
	BLIP_READER_ADJ_( snr, count );
	
	typedef dsample_t stereo_dsample_t [2];
	stereo_dsample_t* BLARGG_RESTRICT out = (stereo_dsample_t*) out_ + count;
	stereo_dsample_t const* BLARGG_RESTRICT in =
			(stereo_dsample_t const*) this_->sample_buf + count;
	int offset = -count;
	int const gain = this_->gain_;
	do
	{
		int sl = BLIP_READER_READ_RAW( snl ) >> (blip_sample_bits - 16);
		int sr = BLIP_READER_READ_RAW( snr ) >> (blip_sample_bits - 16);
		BLIP_READER_NEXT_IDX_( snl, bass, offset );
		BLIP_READER_NEXT_IDX_( snr, bass, offset );
		
		int l = (in [offset] [0] * gain >> gain_bits) + sl;
		int r = (in [offset] [1] * gain >> gain_bits) + sr;
		
		BLIP_CLAMP( l, l );
		out [offset] [0] = (blip_sample_t) l;
		
		BLIP_CLAMP( r, r );
		out [offset] [1] = (blip_sample_t) r;
	}
	while ( ++offset );
	
	BLIP_READER_END( snl, stereo_buf->bufs [1] );
	BLIP_READER_END( snr, stereo_buf->bufs [2] );
}

sample_t const* resample_( struct Resampler* this_, sample_t** out_,
		sample_t const* out_end, sample_t const in [], int in_size )
{
	in_size -= write_offset;
	if ( in_size > 0 )
	{
		sample_t* BLIP_RESTRICT out = *out_;
		sample_t const* const in_end = in + in_size;
		
		int const step = this_->step;
		int       pos  = this_->pos;
		
		// TODO: IIR filter, then linear resample
		// TODO: detect skipped sample, allowing merging of IIR and resample?
		
		do
		{
			#define INTERP( i, out )\
				out = (in [0 + i] * (unit - pos) + ((in [2 + i] + in [4 + i] + in [6 + i]) << shift) +\
				in [8 + i] * pos) >> (shift + 2);
			
			int out_0;
			INTERP( 0,                  out_0 )
			INTERP( 1, out [0] = out_0; out [1] )
			out += stereo;
			
			pos += step;
			in += ((unsigned) pos >> shift) * stereo;
			pos &= unit - 1;
		}
		while ( in < in_end && out < out_end );
		
		this_->pos = pos;
		*out_ = out;
	}
	return in;
}

inline int resample_wrapper( struct Resampler* this_, sample_t out [], int* out_size,
		sample_t const in [], int in_size )
{
	assert( Resampler_rate( this_ ) );
	
	sample_t* out_ = out;
	int result = resample_( this_, &out_, out + *out_size, in, in_size ) - in;
	assert( out_ <= out + *out_size );
	assert( result <= in_size );
	
	*out_size = out_ - out;
	return result;
}

int skip_input( struct Resampler* this_, int count )
{
	this_->write_pos -= count;
	if ( this_->write_pos < 0 ) // occurs when downsampling
	{
		count += this_->write_pos;
		this_->write_pos = 0;
	}
	memmove( this_->buf, &this_->buf [count], this_->write_pos * sizeof this_->buf [0] );
	return count;
}

void play_frame_( struct Resampler* this_, struct Stereo_Buffer* stereo_buf, dsample_t* out )
{
	long pair_count = this_->sample_buf_size >> 1;
	blip_time_t blip_time = Blip_count_clocks( &stereo_buf->bufs [0], pair_count );
	int sample_count = this_->oversamples_per_frame - this_->write_pos + resampler_extra;
	
	int new_count = this_->callback( this_->callback_data, blip_time, sample_count, &this_->buf [this_->write_pos] );
	assert( new_count < resampler_size );
	
	Buffer_end_frame( stereo_buf, blip_time );
	/* Blip_end_frame( &stereo_buf->bufs [0], blip_time ); */
	assert( Blip_samples_avail( &stereo_buf->bufs [0] ) == pair_count * 2 );
	
	this_->write_pos += new_count;
	assert( (unsigned) this_->write_pos <= this_->buf_size );
	
	new_count = this_->sample_buf_size;
	if ( new_count )
		skip_input( this_, resample_wrapper( this_, this_->sample_buf, &new_count, this_->buf, this_->write_pos ) );
	assert( new_count == (long) this_->sample_buf_size );
	
	int bufs_used = stereo_buf->stereo_added | stereo_buf->was_stereo;
	if ( bufs_used <= 1 ) {
		mix_mono( this_, stereo_buf, out );
		Blip_remove_samples( &stereo_buf->bufs [0], pair_count );
		Blip_remove_silence( &stereo_buf->bufs [1], pair_count );
		Blip_remove_silence( &stereo_buf->bufs [2], pair_count );
	}
	else if ( bufs_used & 1 ) {
		mix_stereo( this_, stereo_buf, out );
		Blip_remove_samples( &stereo_buf->bufs [0], pair_count );
		Blip_remove_samples( &stereo_buf->bufs [1], pair_count );
		Blip_remove_samples( &stereo_buf->bufs [2], pair_count );
	}
	else {
		mix_stereo_no_center( this_, stereo_buf, out );
		Blip_remove_silence( &stereo_buf->bufs [0], pair_count );
		Blip_remove_samples( &stereo_buf->bufs [1], pair_count );
		Blip_remove_samples( &stereo_buf->bufs [2], pair_count );
	}
	
	// to do: this might miss opportunities for optimization
	if ( !Blip_samples_avail( &stereo_buf->bufs [0] ) )
	{
		stereo_buf->was_stereo   = stereo_buf->stereo_added;
		stereo_buf->stereo_added = 0;
	}
	
	/* mix_mono( this_, stereo_buf, out );
	Blip_remove_samples( &stereo_buf->bufs [0], pair_count ); */
}

void Resampler_play( struct Resampler* this_, long count, dsample_t* out, struct Stereo_Buffer* stereo_buf )
{
	// empty extra buffer
	long remain = this_->sample_buf_size - this_->buf_pos;
	if ( remain )
	{
		if ( remain > count )
			remain = count;
		count -= remain;
		memcpy( out, &this_->sample_buf [this_->buf_pos], remain * sizeof *out );
		out += remain;
		this_->buf_pos += remain;
	}
	
	// entire frames
	while ( count >= (long) this_->sample_buf_size )
	{
		play_frame_( this_, stereo_buf, out );
		out += this_->sample_buf_size;
		count -= this_->sample_buf_size;
	}
	
	// extra
	if ( count )
	{
		play_frame_( this_, stereo_buf, this_->sample_buf );
		this_->buf_pos = count;
		memcpy( out, this_->sample_buf, count * sizeof *out );
		out += count;
	}
}
