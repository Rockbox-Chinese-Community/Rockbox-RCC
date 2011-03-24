// Blip_Buffer 0.4.1. http://www.slack.net/~ant/

#include "multi_buffer.h"

/* Copyright (C) 2003-2006 Shay Green. This module is free software; you
can redistribute it and/or modify it under the terms of the GNU Lesser
General Public License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version. This
module is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
details. You should have received a copy of the GNU Lesser General Public
License along with this_ module; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA */

#include "blargg_source.h"

#ifdef BLARGG_ENABLE_OPTIMIZER
	#include BLARGG_ENABLE_OPTIMIZER
#endif

// Stereo_Buffer
 
void Buffer_init( struct Stereo_Buffer* this_ )
{
	Blip_init( &this_->bufs [0] );
	Blip_init( &this_->bufs [1] );
	Blip_init( &this_->bufs [2] );
			
	this_->chan.center = &this_->bufs [0];
	this_->chan.left = &this_->bufs [1];
	this_->chan.right = &this_->bufs [2];
	
	this_->length_ = 0;
	this_->sample_rate_ = 0;
	this_->channels_changed_count_ = 1;
	this_->samples_per_frame_ = 2;
}

blargg_err_t Buffer_set_sample_rate( struct Stereo_Buffer* this_, long rate, int msec )
{
	int i;
	for ( i = 0; i < buf_count; i++ )
		RETURN_ERR( Blip_set_sample_rate( &this_->bufs[i], rate, msec ) );
		
	this_->sample_rate_ = Blip_sample_rate( &this_->bufs [0] );
	this_->length_ = Blip_length( &this_->bufs [0] );
	return 0;
}

void Buffer_clock_rate( struct Stereo_Buffer* this_, long rate )
{
	int i;
	for ( i = 0; i < buf_count; i++ )
		Blip_set_clock_rate( &this_->bufs [i], rate );
}

void Buffer_bass_freq( struct Stereo_Buffer* this_, int bass )
{
	unsigned i;
	for ( i = 0; i < buf_count; i++ )
		Blip_bass_freq( &this_->bufs [i], bass );
}

struct channel_t Buffer_channel( struct Stereo_Buffer* this_ )
{
	return this_->chan;
}

void Buffer_clear( struct Stereo_Buffer* this_ )
{
	this_->stereo_added = 0;
	this_->was_stereo   = false;
	int i;
	for ( i = 0; i < buf_count; i++ )
		Blip_clear( &this_->bufs [i], 1 );
}

void Buffer_end_frame( struct Stereo_Buffer* this_, blip_time_t clock_count )
{
	this_->stereo_added = 0;
	unsigned i;
	for ( i = 0; i < buf_count; i++ )
	{
		this_->stereo_added |= Blip_clear_modified( &this_->bufs [i] ) << i;
		Blip_end_frame( &this_->bufs [i], clock_count );
	}
}

long Buffer_read_samples( struct Stereo_Buffer* this_, blip_sample_t* out, long count )
{
	require( !(count & 1) ); // count must be even
	count = (unsigned) count / 2;
	
	long avail = Blip_samples_avail( &this_->bufs [0] );
	if ( count > avail )
		count = avail;
	if ( count )
	{
		int bufs_used = this_->stereo_added | this_->was_stereo;
		//dprintf( "%X\n", bufs_used );
		if ( bufs_used <= 1 )
		{
			Buffer_mix_mono( this_, out, count );
			Blip_remove_samples( &this_->bufs [0], count );
			Blip_remove_silence( &this_->bufs [1], count );
			Blip_remove_silence( &this_->bufs [2], count );
		}
		else if ( bufs_used & 1 )
		{
			Buffer_mix_stereo( this_, out, count );
			Blip_remove_samples( &this_->bufs [0], count );
			Blip_remove_samples( &this_->bufs [1], count );
			Blip_remove_samples( &this_->bufs [2], count );
		}
		else
		{
			Buffer_mix_stereo_no_center( this_, out, count );
			Blip_remove_silence( &this_->bufs [0], count );
			Blip_remove_samples( &this_->bufs [1], count );
			Blip_remove_samples( &this_->bufs [2], count );
		}
		
		// to do: this_ might miss opportunities for optimization
		if ( !Blip_samples_avail( &this_->bufs [0] ) )
		{
			this_->was_stereo   = this_->stereo_added;
			this_->stereo_added = 0;
		}
	}
	
	return count * 2;
}

unsigned Buffer_channels_changed_count( struct Stereo_Buffer* this_ )
{
	return this_->channels_changed_count_;
}

void Buffer_channels_changed( struct Stereo_Buffer* this_ )
{
	this_->channels_changed_count_++;
}

void Buffer_mix_stereo( struct Stereo_Buffer* this_, blip_sample_t* out_, blargg_long count )
{
	blip_sample_t* BLIP_RESTRICT out = out_;
	int const bass = BLIP_READER_BASS( this_->bufs [1] );
	BLIP_READER_BEGIN( left, this_->bufs [1] );
	BLIP_READER_BEGIN( right, this_->bufs [2] );
	BLIP_READER_BEGIN( center, this_->bufs [0] );
	
	for ( ; count; --count )
	{
		int c = BLIP_READER_READ( center );
		blargg_long l = c + BLIP_READER_READ( left );
		blargg_long r = c + BLIP_READER_READ( right );
		if ( (int16_t) l != l )
			l = 0x7FFF - (l >> 24);
		
		BLIP_READER_NEXT( center, bass );
		if ( (int16_t) r != r )
			r = 0x7FFF - (r >> 24);
		
		BLIP_READER_NEXT( left, bass );
		BLIP_READER_NEXT( right, bass );
		
		out [0] = l;
		out [1] = r;
		out += 2;
	}
	
	BLIP_READER_END( center, this_->bufs [0] );
	BLIP_READER_END( right, this_->bufs [2] );
	BLIP_READER_END( left, this_->bufs [1] );
}

void Buffer_mix_stereo_no_center( struct Stereo_Buffer* this_, blip_sample_t* out_, blargg_long count )
{
	blip_sample_t* BLIP_RESTRICT out = out_;
	int const bass = BLIP_READER_BASS( this_->bufs [1] );
	BLIP_READER_BEGIN( left, this_->bufs [1] );
	BLIP_READER_BEGIN( right, this_->bufs [2] );
	
	for ( ; count; --count )
	{
		blargg_long l = BLIP_READER_READ( left );
		if ( (int16_t) l != l )
			l = 0x7FFF - (l >> 24);
		
		blargg_long r = BLIP_READER_READ( right );
		if ( (int16_t) r != r )
			r = 0x7FFF - (r >> 24);
		
		BLIP_READER_NEXT( left, bass );
		BLIP_READER_NEXT( right, bass );
		
		out [0] = l;
		out [1] = r;
		out += 2;
	}
	
	BLIP_READER_END( right, this_->bufs [2] );
	BLIP_READER_END( left, this_->bufs [1] );
}

void Buffer_mix_mono( struct Stereo_Buffer* this_, blip_sample_t* out_, blargg_long count )
{
	blip_sample_t* BLIP_RESTRICT out = out_;
	int const bass = BLIP_READER_BASS( this_->bufs [0] );
	BLIP_READER_BEGIN( center, this_->bufs [0] );
	
	for ( ; count; --count )
	{
		blargg_long s = BLIP_READER_READ( center );
		if ( (int16_t) s != s )
			s = 0x7FFF - (s >> 24);
		
		BLIP_READER_NEXT( center, bass );
		out [0] = s;
		out [1] = s;
		out += 2;
	}
	
	BLIP_READER_END( center, this_->bufs [0] );
}
