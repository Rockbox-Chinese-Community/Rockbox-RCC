// Blip_Buffer 0.4.1. http://www.slack.net/~ant/

#include "blip_buffer.h"

#include <assert.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

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

#ifdef BLARGG_ENABLE_OPTIMIZER
	#include BLARGG_ENABLE_OPTIMIZER
#endif

int const silent_buf_size = 1; // size used for Silent_Blip_Buffer

void Blip_init( struct Blip_Buffer* this_ )
{
	this_->factor_       = LONG_MAX;
	this_->offset_       = 0;
	this_->buffer_size_  = 0;
	this_->sample_rate_  = 0;
	this_->reader_accum_ = 0;
	this_->bass_shift_   = 0;
	this_->clock_rate_   = 0;
	this_->bass_freq_    = 16;
	this_->length_       = 0;
	
	// assumptions code makes about implementation-defined features
	#ifndef NDEBUG
		// right shift of negative value preserves sign
		buf_t_ i = -0x7FFFFFFE;
		assert( (i >> 1) == -0x3FFFFFFF );
		
		// casting to short truncates to 16 bits and sign-extends
		i = 0x18000;
		assert( (short) i == -0x8000 );
	#endif
}

void Blip_stop( struct Blip_Buffer* this_ )
{
	if ( this_->buffer_size_ != silent_buf_size )
		free( this_->buffer_ );
}

void Blip_clear( struct Blip_Buffer* this_, int entire_buffer )
{
	this_->offset_      = 0;
	this_->reader_accum_ = 0;
	this_->modified_    = 0;
	if ( this_->buffer_ )
	{
		long count = (entire_buffer ? this_->buffer_size_ : Blip_samples_avail( this_ ));
		memset( this_->buffer_, 0, (count + blip_buffer_extra_) * sizeof (buf_t_) );
	}
}

blargg_err_t Blip_set_sample_rate( struct Blip_Buffer* this_, long new_rate, int msec )
{
	if ( this_->buffer_size_ == silent_buf_size )
	{
		assert( 0 );
		return "Internal (tried to resize Silent_Blip_Buffer)";
	}
	
	// start with maximum length that resampled time can represent
	long new_size = (ULONG_MAX >> BLIP_BUFFER_ACCURACY) - blip_buffer_extra_ - 64;
	if ( msec != blip_max_length )
	{
		long s = (new_rate * (msec + 1) + 999) / 1000;
		if ( s < new_size )
			new_size = s;
		else
			assert( 0 ); // fails if requested buffer length exceeds limit
	}
	
	if ( new_size > blip_buffer_max )
		return "Out of memory";
	
	this_->buffer_size_ = new_size;
	assert( this_->buffer_size_ != silent_buf_size );
	
	// update things based on the sample rate
	this_->sample_rate_ = new_rate;
	this_->length_ = new_size * 1000 / new_rate - 1;
	if ( msec )
		assert( this_->length_ == msec ); // ensure length is same as that passed in
	if ( this_->clock_rate_ )
		Blip_set_clock_rate( this_, this_->clock_rate_ );
	Blip_bass_freq( this_, this_->bass_freq_ );
	
	Blip_clear( this_, 1 );
	
	return 0; // success
}

/* Not sure if this_ affects sound quality */
#if defined(ROCKBOX)
  #define floor
#endif

blip_resampled_time_t Blip_clock_rate_factor( struct Blip_Buffer* this_, long rate )
{
	double ratio = (double) this_->sample_rate_ / rate;
	blip_long factor = (blip_long) floor( ratio * (1L << BLIP_BUFFER_ACCURACY) + 0.5 );
	assert( factor > 0 || !this_->sample_rate_ ); // fails if clock/output ratio is too large
	return (blip_resampled_time_t) factor;
}

void Blip_bass_freq( struct Blip_Buffer* this_, int freq )
{
	this_->bass_freq_ = freq;
	int shift = 31;
	if ( freq > 0 )
	{
		shift = 13;
		long f = (freq << 16) / this_->sample_rate_;
		while ( (f >>= 1) && --shift ) { }
	}
	this_->bass_shift_ = shift;
}

void Blip_end_frame( struct Blip_Buffer* this_, blip_time_t t )
{
	this_->offset_ += t * this_->factor_;
	assert( Blip_samples_avail( this_ ) <= (long) this_->buffer_size_ ); // time outside buffer length
}

void Blip_remove_silence( struct Blip_Buffer* this_, long count )
{
	assert( count <= Blip_samples_avail( this_ ) ); // tried to remove more samples than available
	this_->offset_ -= (blip_resampled_time_t) count << BLIP_BUFFER_ACCURACY;
}

long Blip_count_samples( struct Blip_Buffer* this_, blip_time_t t )
{
	unsigned long last_sample  = Blip_resampled_time( this_, t ) >> BLIP_BUFFER_ACCURACY;
	unsigned long first_sample = this_->offset_ >> BLIP_BUFFER_ACCURACY;
	return (long) (last_sample - first_sample);
}

blip_time_t Blip_count_clocks( struct Blip_Buffer* this_, long count )
{
	if ( !this_->factor_ )
	{
		assert( 0 ); // sample rate and clock rates must be set first
		return 0;
	}
	
	if ( count > this_->buffer_size_ )
		count = this_->buffer_size_;
	blip_resampled_time_t time = (blip_resampled_time_t) count << BLIP_BUFFER_ACCURACY;
	return (blip_time_t) ((time - this_->offset_ + this_->factor_ - 1) / this_->factor_);
}

void Blip_remove_samples( struct Blip_Buffer* this_, long count )
{
	if ( count )
	{
		Blip_remove_silence( this_, count );
		
		// copy remaining samples to beginning and clear old samples
		long remain = Blip_samples_avail( this_ ) + blip_buffer_extra_;
		memmove( this_->buffer_, this_->buffer_ + count, remain * sizeof *this_->buffer_ );
		memset( this_->buffer_ + remain, 0, count * sizeof *this_->buffer_ );
	}
}

long Blip_read_samples( struct Blip_Buffer* this_, blip_sample_t* BLIP_RESTRICT out, long max_samples, int stereo )
{
	long count = Blip_samples_avail( this_ );
	if ( count > max_samples )
		count = max_samples;
	
	if ( count )
	{
		int const bass = BLIP_READER_BASS( *this_ );
		BLIP_READER_BEGIN( reader, *this_ );
		
		if ( !stereo )
		{
			blip_long n;
			for ( n = count; n; --n )
			{
				blip_long s = BLIP_READER_READ( reader );
				if ( (blip_sample_t) s != s )
					s = 0x7FFF - (s >> 24);
				*out++ = (blip_sample_t) s;
				BLIP_READER_NEXT( reader, bass );
			}
		}
		else
		{
			blip_long n;
			for ( n = count; n; --n )
			{
				blip_long s = BLIP_READER_READ( reader );
				if ( (blip_sample_t) s != s )
					s = 0x7FFF - (s >> 24);
				*out = (blip_sample_t) s;
				out += 2;
				BLIP_READER_NEXT( reader, bass );
			}
		}
		BLIP_READER_END( reader, *this_ );
		
		Blip_remove_samples( this_, count );
	}
	return count;
}

void Blip_mix_samples( struct Blip_Buffer* this_,  blip_sample_t const* in, long count )
{
	if ( this_->buffer_size_ == silent_buf_size )
	{
		assert( 0 );
		return;
	}
	
	buf_t_* out = this_->buffer_ + (this_->offset_ >> BLIP_BUFFER_ACCURACY) + blip_widest_impulse_ / 2;
	
	int const sample_shift = blip_sample_bits - 16;
	int prev = 0;
	while ( count-- )
	{
		blip_long s = (blip_long) *in++ << sample_shift;
		*out += s - prev;
		prev = s;
		++out;
	}
	*out -= prev;
}

void Blip_set_modified( struct Blip_Buffer* this_ ) 
{ 
	this_->modified_ = 1; 
}

int Blip_clear_modified( struct Blip_Buffer* this_ )
{ 
	int b = this_->modified_;
	this_->modified_ = 0;
	return b; 
}

blip_resampled_time_t Blip_resampled_duration( struct Blip_Buffer* this_, int t )
{
	return t * this_->factor_;
}

blip_resampled_time_t Blip_resampled_time( struct Blip_Buffer* this_, blip_time_t t )
{
	return t * this_->factor_ + this_->offset_;
}


// Blip_Synth

void Synth_init( struct Blip_Synth* this_ )
{
	this_->buf = 0;
	this_->last_amp = 0;
	this_->delta_factor = 0;
}

// Set overall volume of waveform
void Synth_volume( struct Blip_Synth* this_, double v )
{
	this_->delta_factor = (int) (v * (1L << blip_sample_bits) + 0.5);
}
