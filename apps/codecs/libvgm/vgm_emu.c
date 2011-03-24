// Game_Music_Emu 0.5.5. http://www.slack.net/~ant/

#include "vgm_emu.h"

#include "blargg_endian.h"
#include <string.h>
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

#include "blargg_source.h"

const char* const gme_wrong_file_type = "Wrong file type for this emulator";

double const fm_gain = 3.0; // FM emulators are internally quieter to avoid 16-bit overflow
double const rolloff = 0.990;
double const oversample_factor = 1.5;

int const silence_max = 6; // seconds
int const silence_threshold = 0x10;
long const fade_block_size = 512;
int const fade_shift = 8; // fade ends with gain at 1.0 / (1 << fade_shift)

// VGM commands (Spec v1.50)
enum {
	cmd_gg_stereo       = 0x4F,
	cmd_psg             = 0x50,
	cmd_ym2413          = 0x51,
	cmd_ym2612_port0    = 0x52,
	cmd_ym2612_port1    = 0x53,
	cmd_ym2151          = 0x54,
	cmd_delay           = 0x61,
	cmd_delay_735       = 0x62,
	cmd_delay_882       = 0x63,
	cmd_byte_delay      = 0x64,
	cmd_end             = 0x66,
	cmd_data_block      = 0x67,
	cmd_short_delay     = 0x70,
	cmd_pcm_delay       = 0x80,
	cmd_pcm_seek        = 0xE0,
	
	pcm_block_type      = 0x00,
	ym2612_dac_port     = 0x2A,
	ym2612_dac_pan_port = 0xB6
};

void clear_track_vars( struct Vgm_Emu* this_ )
{
	this_->out_time         = 0;
	this_->emu_time         = 0;
	this_->emu_track_ended_ = true;
	this_->track_ended     = true;
	this_->fade_start       = INT_MAX / 2 + 1;
	this_->fade_step        = 1;
	this_->silence_time     = 0;
	this_->silence_count    = 0;
	this_->buf_remain       = 0;
}

int play_frame( struct Vgm_Emu* this_, blip_time_t blip_time, int sample_count, sample_t* buf );
static int play_frame_( void* data, blip_time_t blip_time, int sample_count, short int* buf )
{
	return play_frame( (struct Vgm_Emu*) data, blip_time, sample_count, buf );
} 

void Vgm_init( struct Vgm_Emu* this_ )
{
	this_->sample_rate = 0;
	this_->mute_mask_ = 0;
	this_->tempo = 1.0;
	
	// defaults
	this_->max_initial_silence = 2;
	this_->silence_lookahead = 1; // tracks should already be trimmed
	this_->ignore_silence     = false;

	// Disable oversampling by default
	this_->disable_oversampling = true;
	this_->psg_rate   = 0;
	
	Apu_init( &this_->psg );
	Synth_init( &this_->pcm );
	
	Buffer_init( &this_->buf );
	Buffer_init( &this_->stereo_buf );
	this_->blip_buf = &this_->stereo_buf.bufs [0];

	// Init fm chips	
	Ym2413_init( &this_->ym2413 );
	Ym2612_init( &this_->ym2612 );
	
	// Init resampler
	Resampler_init( &this_->resampler );
	Resampler_set_callback( &this_->resampler, play_frame_, this_ );

	// Set sound gain, a value too high
	// will cause saturation
	Sound_set_gain(this_, 1.4);
	
	// Unload
	this_->voice_count = 0;
	clear_track_vars( this_ );	
}

// Track info

static byte const* skip_gd3_str( byte const* in, byte const* end )
{
	while ( end - in >= 2 )
	{
		in += 2;
		if ( !(in [-2] | in [-1]) )
			break;
	}
	return in;
}

static byte const* get_gd3_str( byte const* in, byte const* end, char* field )
{
	byte const* mid = skip_gd3_str( in, end );
	int i, len = (mid - in) / 2 - 1;
	if ( field && len > 0 )
	{
		len = min( len, (int) gme_max_field );
		field [len] = 0;
		for ( i = 0; i < len; i++ )
			field [i] = (in [i * 2 + 1] ? '?' : in [i * 2]); // TODO: convert to utf-8
	}
	return mid;
}

static byte const* get_gd3_pair( byte const* in, byte const* end, char* field )
{
	return skip_gd3_str( get_gd3_str( in, end, field ), end );
}

static void parse_gd3( byte const* in, byte const* end, struct track_info_t* out )
{
	in = get_gd3_pair( in, end, out->song );
	in = get_gd3_pair( in, end, out->game );
	in = get_gd3_pair( in, end, NULL ); // Skip system
	in = get_gd3_pair( in, end, out->author );
}

int const gd3_header_size = 12;

static long check_gd3_header( byte const* h, long remain )
{
	if ( remain < gd3_header_size ) return 0;
	if ( memcmp( h, "Gd3 ", 4 ) ) return 0;
	if ( get_le32( h + 4 ) >= 0x200 ) return 0;
	
	long gd3_size = get_le32( h + 8 );
	if ( gd3_size > remain - gd3_header_size )
		gd3_size = remain - gd3_header_size;
	return gd3_size;
}

byte const* gd3_data( struct Vgm_Emu* this_, int* size )
{
	if ( size )
		*size = 0;
	
	long gd3_offset = get_le32( header( this_ )->gd3_offset ) - 0x2C;
	if ( gd3_offset < 0 )
		return 0;
	
	byte const* gd3 = this_->file_begin + header_size + gd3_offset;
	long gd3_size = check_gd3_header( gd3, this_->file_end - gd3 );
	if ( !gd3_size )
		return 0;
	
	if ( size )
		*size = gd3_size + gd3_header_size;
	
	return gd3;
}

static void get_vgm_length( struct header_t const* h, struct track_info_t* out )
{
	long length = get_le32( h->track_duration ) * 10 / 441;
	if ( length > 0 )
	{
		long loop = get_le32( h->loop_duration );
		if ( loop > 0 && get_le32( h->loop_offset ) )
		{
			out->loop_length = loop * 10 / 441;
			out->intro_length = length - out->loop_length;
		}
		else
		{
			out->length = length; // 1000 / 44100 (VGM files used 44100 as timebase)
			out->intro_length = length; // make it clear that track is no longer than length
			out->loop_length = 0;
		}
	}
}

blargg_err_t track_info( struct Vgm_Emu* this_, struct track_info_t* out )
{
	memset(out, 0, sizeof out);
	get_vgm_length( header( this_ ), out );
	
	int size;
	byte const* gd3 = gd3_data( this_, &size );
	if ( gd3 )
		parse_gd3( gd3 + gd3_header_size, gd3 + size, out );
	
	return 0;
}

static blargg_err_t check_vgm_header( struct header_t* h )
{
	if ( memcmp( h->tag, "Vgm ", 4 ) )
		return gme_wrong_file_type;
	return 0;
}

void set_voice( struct Vgm_Emu* this_, int i, struct Blip_Buffer* c, struct Blip_Buffer* l, struct Blip_Buffer* r )
{
	if ( i < osc_count ) {
		int j;
		for ( j = osc_count; --j >= 0; )
			Apu_set_output( &this_->psg, j, c, l, r );
	}
}

blargg_err_t setup_fm( struct Vgm_Emu* this_ );
blargg_err_t Vgm_load_mem( struct Vgm_Emu* this_, byte const* new_data, long new_size, bool parse_info )
{
	// Unload
	this_->voice_count = 0;
	clear_track_vars( this_ );
	
	// Clear info
	memset( &this_->info, 0, sizeof this_->info );

	assert( offsetof (struct header_t,unused2 [8]) == header_size );
	
	if ( new_size <= header_size )
		return gme_wrong_file_type;
			
	// Reset data pointers
	this_->file_begin = new_data;
	this_->file_end = new_data + new_size;
	
	struct header_t* h = (struct header_t*) new_data;
	RETURN_ERR( check_vgm_header( h ) );
	check( get_le32( h.version ) <= 0x150 );
	
	// If this was VGZ file gd3 parse info
	if ( parse_info ) {
 		track_info( this_, &this_->info );
		
		// If file was trimmed add an
		// incomplete token to the game tag
		if ( get_le32( h->data_size ) > (unsigned) new_size ) {
			*((char *) this_->file_end) = cmd_end;
			strcat(this_->info.game, "(Trimmed VGZ file)" );
		}
	}
	
	// Get loop
	this_->loop_begin = this_->file_end;
	
	// If file was trimmed don't loop
	if ( get_le32( h->loop_offset ) && get_le32( h->data_size ) <= (unsigned) new_size ) 
		this_->loop_begin = &new_data [get_le32( h->loop_offset ) + offsetof (struct header_t,loop_offset)];

	// PSG rate
	this_->psg_rate = get_le32( h->psg_rate );
	if ( !this_->psg_rate )
		this_->psg_rate = 3579545;
	
	Buffer_clock_rate( &this_->stereo_buf, this_->psg_rate );
	
	// Disable FM
	this_->fm_rate = 0;
	Ym2612_enable( &this_->ym2612, false );
	Ym2413_enable( &this_->ym2413, false );
	
	Sound_set_tempo( this_, 1 );
	
	this_->voice_count = osc_count;
	
	RETURN_ERR( setup_fm( this_ ) );
	
	// do after FM in case output buffer is changed
	// setup buffer
	this_->clock_rate_ = this_->psg_rate;
	Buffer_clock_rate( &this_->buf, this_->psg_rate );
	
	// Setup bass
	this_->buf_changed_count = Buffer_channels_changed_count( &this_->buf );
	
	// Post load
	Sound_set_tempo( this_, this_->tempo );
	Sound_mute_voices( this_, this_->mute_mask_ );
	
	return 0;
}

void update_fm_rates( struct Vgm_Emu* this_, int* ym2413_rate, int* ym2612_rate );
blargg_err_t init_fm( struct Vgm_Emu* this_, double* rate )
{
	int ym2612_rate = get_le32( header( this_ )->ym2612_rate );
	int ym2413_rate = get_le32( header( this_ )->ym2413_rate );
	if ( ym2413_rate && get_le32( header( this_ )->version ) < 0x110 )
		update_fm_rates( this_, &ym2413_rate, &ym2612_rate );
	
	if ( ym2612_rate )
	{
		if ( !*rate )
			*rate = ym2612_rate / 144.0;
		RETURN_ERR( Ym2612_set_rate( &this_->ym2612, *rate, ym2612_rate ) );
		Ym2612_enable( &this_->ym2612, true );
	}
	else if ( ym2413_rate )
	{
		if ( !*rate )
			*rate = ym2413_rate / 72.0;
		int result = Ym2413_set_rate( &this_->ym2413, *rate, ym2413_rate );
		if ( result == 2 )
			return "YM2413 FM sound not supported";
		CHECK_ALLOC( !result );
		Ym2413_enable( &this_->ym2413, true );
	}
	
	this_->fm_rate = *rate;
	
	return 0;
}

blargg_err_t setup_fm( struct Vgm_Emu* this_ )
{
	double fm_rate = 0.0;
	if ( !this_->disable_oversampling )
		this_->fm_rate = this_->sample_rate * oversample_factor;
	RETURN_ERR( init_fm( this_, &fm_rate ) );
	
	if ( uses_fm( this_ ) )
	{
		this_->voice_count = 8;
		RETURN_ERR( Resampler_setup( &this_->resampler, fm_rate / this_->sample_rate, rolloff, fm_gain * this_->gain ) );
		RETURN_ERR( Resampler_reset( &this_->resampler, Buffer_length( &this_->stereo_buf ) * this_->sample_rate / 1000 ) );
		Apu_volume( &this_->psg, 0.135 * fm_gain * this_->gain );
	}
	else
	{
		Apu_volume( &this_->psg, this_->gain );
	}
	
	return 0;
}

// Emulation

blip_time_t run( struct Vgm_Emu* this_, vgm_time_t end_time );
blargg_err_t run_clocks( struct Vgm_Emu* this_, blip_time_t* time_io, int msec )
{
	*time_io = run( this_, msec * this_->vgm_rate / 1000 );
	Apu_end_frame( &this_->psg, *time_io );
	return 0;
}



blargg_err_t play_( struct Vgm_Emu* this_, long count, sample_t* out )
{
	if ( !uses_fm( this_ ) ) {
		long remain = count;
		while ( remain )
		{
			remain -= Buffer_read_samples( &this_->buf, &out [count - remain], remain );
			if ( remain )
			{	
				if ( this_->buf_changed_count != Buffer_channels_changed_count( &this_->buf ) )
				{
					this_->buf_changed_count = Buffer_channels_changed_count( &this_->buf );
					
					// Remute voices
					Sound_mute_voices( this_, this_->mute_mask_ );
				}
				int msec = Buffer_length( &this_->buf );
				blip_time_t clocks_emulated = (blargg_long) msec * this_->clock_rate_ / 1000 - 100;
				RETURN_ERR( run_clocks( this_, &clocks_emulated, msec ) );
				assert( clocks_emulated );
				Buffer_end_frame( &this_->buf, clocks_emulated );
			}
		}
		
		return 0;
	}
		
	Resampler_play( &this_->resampler, count, out, &this_->stereo_buf );
	return 0;
}

// Vgm_Emu_impl

inline int command_len( int command )
{
	static byte const lens [0x10] ICONST_ATTR = {
	// 0 1 2 3 4 5 6 7 8 9 A B C D E F
	   1,1,1,2,2,3,1,1,1,1,3,3,4,4,5,5
	};
	int len = lens [command >> 4];
	check( len != 1 );
	return len;
}
	
inline fm_time_t to_fm_time( struct Vgm_Emu* this_, vgm_time_t t )
{
	return (t * this_->fm_time_factor + this_->fm_time_offset) >> fm_time_bits;
}

inline blip_time_t to_psg_time( struct Vgm_Emu* this_, vgm_time_t t )
{
	return (t * this_->blip_time_factor) >> blip_time_bits;
}

void write_pcm( struct Vgm_Emu* this_, vgm_time_t vgm_time, int amp )
{
	if ( this_->blip_buf )
	{
		check( amp >= 0 );
		blip_time_t blip_time = to_psg_time( this_, vgm_time );
		int old = this_->dac_amp;
		int delta = amp - old;
		this_->dac_amp = amp;
		Blip_set_modified( this_->blip_buf );
		if ( old >= 0 ) // first write is ignored, to avoid click
			Synth_offset_inline( &this_->pcm, blip_time, delta, this_->blip_buf );
		else
			this_->dac_amp |= this_->dac_disabled;
	}
}

blip_time_t run( struct Vgm_Emu* this_, vgm_time_t end_time )
{
	vgm_time_t vgm_time = this_->vgm_time; 
	byte const* pos = this_->pos;
	if ( pos >= this_->file_end )
	{
		this_->emu_track_ended_ = true;
		/* if ( pos > data_end )
			warning( "Stream lacked end event" ); */
	}
	
	while ( vgm_time < end_time && pos < this_->file_end )
	{
		// TODO: be sure there are enough bytes left in stream for particular command
		// so we don't read past end
		switch ( *pos++ )
		{
		case cmd_end:
			pos = this_->loop_begin; // if not looped, loop_begin == data_end
			break;
		
		case cmd_delay_735:
			vgm_time += 735;
			break;
		
		case cmd_delay_882:
			vgm_time += 882;
			break;
		
		case cmd_gg_stereo:
			Apu_write_ggstereo( &this_->psg, to_psg_time( this_, vgm_time ), *pos++ );
			break;
		
		case cmd_psg:
			Apu_write_data( &this_->psg, to_psg_time( this_, vgm_time ), *pos++ );
			break;
		
		case cmd_delay:
			vgm_time += pos [1] * 0x100 + pos [0];
			pos += 2;
			break;
		
		case cmd_byte_delay:
			vgm_time += *pos++;
			break;
		
		case cmd_ym2413:
			if ( Ym2413_run_until( &this_->ym2413, to_fm_time( this_, vgm_time ) ) )
				Ym2413_write( &this_->ym2413, pos [0], pos [1] );
			pos += 2;
			break;
		
		case cmd_ym2612_port0:
			if ( pos [0] == ym2612_dac_port )
			{
				write_pcm( this_, vgm_time, pos [1] );
			}
			else if ( Ym2612_run_until( &this_->ym2612, to_fm_time( this_, vgm_time ) ) )
			{
				if ( pos [0] == 0x2B )
				{
					this_->dac_disabled = (pos [1] >> 7 & 1) - 1;
					this_->dac_amp |= this_->dac_disabled;
				}
				Ym2612_write0( &this_->ym2612, pos [0], pos [1] );
			}
			pos += 2;
			break;
		
		case cmd_ym2612_port1:
			if ( Ym2612_run_until( &this_->ym2612, to_fm_time( this_, vgm_time ) ) )
			{
				if ( pos [0] == ym2612_dac_pan_port )
				{
					struct Blip_Buffer* blip_buf = NULL;
					switch ( pos [1] >> 6 )
					{
					case 0: blip_buf = NULL; break;
					case 1: blip_buf = &this_->stereo_buf.bufs [2]; break;
					case 2: blip_buf = &this_->stereo_buf.bufs [1]; break;
					case 3: blip_buf = &this_->stereo_buf.bufs [0]; break;
					}
					this_->blip_buf = blip_buf;
				}
			
				Ym2612_write1( &this_->ym2612, pos [0], pos [1] );
			}
			pos += 2;
			break;
			
		case cmd_data_block: {
			check( *pos == cmd_end );
			int type = pos [1];
			long size = get_le32( pos + 2 );
			pos += 6;
			if ( type == pcm_block_type )
				this_->pcm_data = pos;
			pos += size;
			break;
		}
		
		case cmd_pcm_seek:
			this_->pcm_pos = this_->pcm_data + pos [3] * 0x1000000 + pos [2] * 0x10000 +
					pos [1] * 0x100 + pos [0];
			pos += 4;
			break;
		
		default: {
				int cmd = pos [-1];
				switch ( cmd & 0xF0 )
				{
					case cmd_pcm_delay:
						write_pcm( this_, vgm_time, *this_->pcm_pos++ );
						vgm_time += cmd & 0x0F;
						break;
					
					case cmd_short_delay:
						vgm_time += (cmd & 0x0F) + 1;
						break;
					
					case 0x50:
						pos += 2;
						break;
					
					default:
						pos += command_len( cmd ) - 1;
						/* warning( "Unknown stream event" ); */
				}
			}
		}
	}
	vgm_time -= end_time;
	this_->pos = pos;
	this_->vgm_time = vgm_time;
	
	return to_psg_time( this_, end_time );
}

int play_frame( struct Vgm_Emu* this_, blip_time_t blip_time, int sample_count, blip_sample_t out [] )
{
	// to do: timing is working mostly by luck
	int min_pairs = (unsigned) sample_count / 2;
	int vgm_time = (min_pairs << fm_time_bits) / this_->fm_time_factor - 1;
	assert( to_fm_time( this_, vgm_time ) <= min_pairs );
	int pairs;
	while ( (pairs = to_fm_time( this_, vgm_time )) < min_pairs )
		vgm_time++;
	//debug_printf( "pairs: %d, min_pairs: %d\n", pairs, min_pairs );
	
	if ( Ym2612_enabled( &this_->ym2612 ) )
	{
		Ym2612_begin_frame( &this_->ym2612, out );
		memset( out, 0, pairs * stereo * sizeof *out );
	}
	else if ( Ym2413_enabled( &this_->ym2413 ) )
	{
		Ym2413_begin_frame( &this_->ym2413, out );
	}
	
	run( this_, vgm_time );
	Ym2612_run_until( &this_->ym2612, pairs );
	Ym2413_run_until( &this_->ym2413, pairs );
	
	this_->fm_time_offset = (vgm_time * this_->fm_time_factor + this_->fm_time_offset) - (pairs << fm_time_bits);
	
	Apu_end_frame( &this_->psg, blip_time );
	
	return pairs * stereo;
}

// Update pre-1.10 header FM rates by scanning commands
void update_fm_rates( struct Vgm_Emu* this_, int* ym2413_rate, int* ym2612_rate )
{
	byte const* p = this_->file_begin + 0x40;
	while ( p < this_->file_end )
	{
		switch ( *p )
		{
		case cmd_end:
			return;
		
		case cmd_psg:
		case cmd_byte_delay:
			p += 2;
			break;
		
		case cmd_delay:
			p += 3;
			break;
		
		case cmd_data_block:
			p += 7 + get_le32( p + 3 );
			break;
		
		case cmd_ym2413:
			*ym2612_rate = 0;
			return;
		
		case cmd_ym2612_port0:
		case cmd_ym2612_port1:
			*ym2612_rate = *ym2413_rate;
			*ym2413_rate = 0;
			return;
		
		case cmd_ym2151:
			*ym2413_rate = 0;
			*ym2612_rate = 0;
			return;
		
		default:
			p += command_len( *p );
		}
	}
}


// Music Emu

blargg_err_t Vgm_set_sample_rate( struct Vgm_Emu* this_, long rate )
{
	require( !this_->sample_rate ); // sample rate can't be changed once set
	RETURN_ERR( Buffer_set_sample_rate( &this_->stereo_buf, rate, 1000 / 30 ) );
	RETURN_ERR( Buffer_set_sample_rate( &this_->buf, rate, 1000 / 20 ) );
	
	// Set bass frequency
	Buffer_bass_freq( &this_->buf, 80 );
	
	this_->sample_rate = rate;
	return 0;
}

void Sound_mute_voice( struct Vgm_Emu* this_, int index, bool mute )
{
	require( (unsigned) index < (unsigned) this_->voice_count );
	int bit = 1 << index;
	int mask = this_->mute_mask_ | bit;
	if ( !mute )
		mask ^= bit;
	Sound_mute_voices( this_, mask );
}

void Sound_mute_voices( struct Vgm_Emu* this_, int mask )
{
	require( this_->sample_rate ); // sample rate must be set first
	this_->mute_mask_ = mask;

	int i;
	for ( i = this_->voice_count; i--; )
	{
		if ( mask & (1 << i) )
		{
			set_voice( this_, i, 0, 0, 0 );
		}
		else
		{
			struct channel_t ch = Buffer_channel( &this_->buf );
			assert( (ch.center && ch.left && ch.right) ||
					(!ch.center && !ch.left && !ch.right) ); // all or nothing
			set_voice( this_, i, ch.center, ch.left, ch.right );
		}
	}
	
	// TODO: what was this for?
	//core.pcm.output( &core.blip_buf );
	
	// TODO: silence PCM if FM isn't used?
	if ( uses_fm( this_ ) )
	{
		for ( i = osc_count; --i >= 0; )
			Apu_set_output( &this_->psg, i, ( mask & 0x80 ) ? 0 : &this_->stereo_buf.bufs [0], NULL, NULL );
		if ( Ym2612_enabled( &this_->ym2612 ) )
		{
			Synth_volume( &this_->pcm, (mask & 0x40) ? 0.0 : 0.1115 / 256 * fm_gain * this_->gain );
			Ym2612_mute_voices( &this_->ym2612, mask );
		}
		
		if ( Ym2413_enabled( &this_->ym2413 ) )
		{
			int m = mask & 0x3F;
			if ( mask & 0x20 )
				m |= 0x01E0; // channels 5-8
			if ( mask & 0x40 )
				m |= 0x3E00;
			Ym2413_mute_voices( &this_->ym2413, m );
		}
	}
}

void Sound_set_tempo( struct Vgm_Emu* this_, double t )
{
	require( this_->sample_rate ); // sample rate must be set first
	double const min = 0.02;
	double const max = 4.00;
	if ( t < min ) t = min;
	if ( t > max ) t = max;
	this_->tempo = t;
	
	if ( this_->file_begin )
	{
		this_->vgm_rate = (long) (44100 * t + 0.5);
		this_->blip_time_factor = (int) ((double)
				(1 << blip_time_bits) / this_->vgm_rate * Blip_clock_rate( &this_->stereo_buf.bufs [0] ) + 0.5);
		//debug_printf( "blip_time_factor: %ld\n", blip_time_factor );
		//debug_printf( "vgm_rate: %ld\n", vgm_rate );
		// TODO: remove? calculates vgm_rate more accurately (above differs at most by one Hz only)
		//blip_time_factor = (long) floor( double (1L << blip_time_bits) * psg_rate / 44100 / t + 0.5 );
		//vgm_rate = (long) floor( double (1L << blip_time_bits) * psg_rate / blip_time_factor + 0.5 );
		
		this_->fm_time_factor = 2 + (int) (this_->fm_rate * (1 << fm_time_bits) / this_->vgm_rate + 0.5);
	}
}

void fill_buf( struct Vgm_Emu *this_ );
blargg_err_t Vgm_start_track( struct Vgm_Emu* this_ )
{
	clear_track_vars( this_ );
	
	Apu_reset( &this_->psg, get_le16( header( this_ )->noise_feedback ), header( this_ )->noise_width );
	
	this_->blip_buf = &this_->stereo_buf.bufs [0];
	
	this_->dac_disabled = -1;
	this_->pos          = this_->file_begin + header_size;
	this_->pcm_data     = this_->pos;
	this_->pcm_pos      = this_->pos;
	this_->dac_amp      = -1;
	this_->vgm_time     = 0;
	if ( get_le32( header( this_ )->version ) >= 0x150 )
	{
		long data_offset = get_le32( header( this_ )->data_offset );
		check( data_offset );
		if ( data_offset )
			this_->pos += data_offset + offsetof (struct header_t,data_offset) - 0x40;
	}
	
	if ( uses_fm( this_ ) )
	{
		if ( Ym2413_enabled( &this_->ym2413 ) )
			Ym2413_reset( &this_->ym2413 );
		
		if ( Ym2612_enabled( &this_->ym2612 ) )
			Ym2612_reset( &this_->ym2612 );
		
		Buffer_clear( &this_->stereo_buf );
		Resampler_clear( &this_->resampler );
	}
	
	this_->fm_time_offset = 0;
	
	Buffer_clear( &this_->buf );
	
	this_->emu_track_ended_ = false;
	this_->track_ended     = false;
	
	if ( !this_->ignore_silence )
	{
		// play until non-silence or end of track
		long end;
		for ( end = this_->max_initial_silence * stereo * this_->sample_rate; this_->emu_time < end; )
		{
			fill_buf( this_ );
			if ( this_->buf_remain | (int) this_->emu_track_ended_ )
				break;
		}
		
		this_->emu_time      = this_->buf_remain;
		this_->out_time      = 0;
		this_->silence_time  = 0;
		this_->silence_count = 0;
	}
	/* return track_ended() ? warning() : 0; */
	return 0;
}

// Tell/Seek

blargg_long msec_to_samples( blargg_long msec, long sample_rate )
{
	blargg_long sec = msec / 1000;
	msec -= sec * 1000;
	return (sec * sample_rate + msec * sample_rate / 1000) * stereo;
}

long Track_tell( struct Vgm_Emu* this_ )
{
	blargg_long rate = this_->sample_rate * stereo;
	blargg_long sec = this_->out_time / rate;
	return sec * 1000 + (this_->out_time - sec * rate) * 1000 / rate;
}

blargg_err_t Track_seek( struct Vgm_Emu* this_, long msec )
{
	blargg_long time = msec_to_samples( msec, this_->sample_rate );
	if ( time < this_->out_time )
		RETURN_ERR( Vgm_start_track( this_ ) );
	return Track_skip( this_, time - this_->out_time );
}

blargg_err_t skip_( struct Vgm_Emu* this_, long count );
blargg_err_t Track_skip( struct Vgm_Emu* this_, long count )
{
	this_->out_time += count;
	
	// remove from silence and buf first
	{
		long n = min( count, this_->silence_count );
		this_->silence_count -= n;
		count -= n;
		
		n = min( count, this_->buf_remain );
		this_->buf_remain -= n;
		count -= n;
	}
		
	if ( count && !this_->emu_track_ended_ )
	{
		this_->emu_time += count;
		if ( skip_( this_, count ) )
			this_->emu_track_ended_ = true;
	}
	
	if ( !(this_->silence_count | this_->buf_remain) ) // caught up to emulator, so update track ended
		this_->track_ended |= this_->emu_track_ended_;
	
	return 0;
}

blargg_err_t skip_( struct Vgm_Emu* this_, long count )
{
	// for long skip, mute sound
	const long threshold = 30000;
	if ( count > threshold )
	{
		int saved_mute = this_->mute_mask_;
		Sound_mute_voices( this_, ~0 );
		
		while ( count > threshold / 2 && !this_->emu_track_ended_ )
		{
			RETURN_ERR( play_( this_, buf_size, this_->buf_ ) );
			count -= buf_size;
		}
		
		Sound_mute_voices( this_, saved_mute );
	}
	
	while ( count && !this_->emu_track_ended_ )
	{
		long n = buf_size;
		if ( n > count )
			n = count;
		count -= n;
		RETURN_ERR( play_( this_, n, this_->buf_ ) );
	}
	return 0;
}

// Fading

void Track_set_fade( struct Vgm_Emu* this_, long start_msec, long length_msec )
{
	this_->fade_step = this_->sample_rate * length_msec / (fade_block_size * fade_shift * 1000 / stereo);
	this_->fade_start = msec_to_samples( start_msec, this_->sample_rate );
}

// unit / pow( 2.0, (double) x / step )
static int int_log( blargg_long x, int step, int unit )
{
	int shift = x / step;
	int fraction = (x - shift * step) * unit / step;
	return ((unit - fraction) + (fraction >> 1)) >> shift;
}

void handle_fade( struct Vgm_Emu* this_, long out_count, sample_t* out )
{
	int i;
	for ( i = 0; i < out_count; i += fade_block_size )
	{
		int const shift = 14;
		int const unit = 1 << shift;
		int gain = int_log( (this_->out_time + i - this_->fade_start) / fade_block_size,
				this_->fade_step, unit );
		if ( gain < (unit >> fade_shift) )
			this_->track_ended = this_->emu_track_ended_ = true;
		
		sample_t* io = &out [i];
		int count;
		for ( count = min( fade_block_size, out_count - i ); count; --count )
		{
			*io = (sample_t) ((*io * gain) >> shift);
			++io;
		}
	}
}

// Silence detection

void emu_play( struct Vgm_Emu* this_, long count, sample_t* out )
{
	this_->emu_time += count;
	if ( !this_->emu_track_ended_ ) {
		if ( play_( this_, count, out ) )
			this_->emu_track_ended_ = true;
	}
	else
		memset( out, 0, count * sizeof *out );
}

// number of consecutive silent samples at end
static long count_silence( sample_t* begin, long size )
{
	sample_t first = *begin;
	*begin = silence_threshold; // sentinel
	sample_t* p = begin + size;
	while ( (unsigned) (*--p + silence_threshold / 2) <= (unsigned) silence_threshold ) { }
	*begin = first;
	return size - (p - begin);
}

// fill internal buffer and check it for silence
void fill_buf( struct Vgm_Emu* this_ )
{
	assert( !this_->buf_remain );
	if ( !this_->emu_track_ended_ )
	{
		emu_play( this_, buf_size, this_->buf_ );
		long silence = count_silence( this_->buf_, buf_size );
		if ( silence < buf_size )
		{
			this_->silence_time = this_->emu_time - silence;
			this_->buf_remain   = buf_size;
			return;
		}
	}
	this_->silence_count += buf_size;
}

blargg_err_t Vgm_play( struct Vgm_Emu* this_, long out_count, sample_t* out )
{
	if ( this_->track_ended )
	{
		memset( out, 0, out_count * sizeof *out );
	}
	else
	{
		require( out_count % stereo == 0 );
		
		assert( this_->emu_time >= this_->out_time );
		
		// prints nifty graph of how far ahead we are when searching for silence
		//debug_printf( "%*s \n", int ((emu_time - out_time) * 7 / sample_rate()), "*" );
		
		long pos = 0;
		if ( this_->silence_count )
		{
			// during a run of silence, run emulator at >=2x speed so it gets ahead
			long ahead_time = this_->silence_lookahead * (this_->out_time + out_count - this_->silence_time) + this_->silence_time;
			while ( this_->emu_time < ahead_time && !(this_->buf_remain | this_->emu_track_ended_) )
				fill_buf( this_ );
			
			// fill with silence
			pos = min( this_->silence_count, out_count );
			memset( out, 0, pos * sizeof *out );
			this_->silence_count -= pos;
			
			if ( this_->emu_time - this_->silence_time > silence_max * stereo * this_->sample_rate )
			{
				this_->track_ended  = this_->emu_track_ended_ = true;
				this_->silence_count = 0;
				this_->buf_remain    = 0;
			}
		}
		
		if ( this_->buf_remain )
		{
			// empty silence buf
			long n = min( this_->buf_remain, out_count - pos );
			memcpy( &out [pos], this_->buf_ + (buf_size - this_->buf_remain), n * sizeof *out );
			this_->buf_remain -= n;
			pos += n;
		}
		
		// generate remaining samples normally
		long remain = out_count - pos;
		if ( remain )
		{
			emu_play( this_, remain, out + pos );
			this_->track_ended |= this_->emu_track_ended_;
			
			if ( !this_->ignore_silence || this_->out_time > this_->fade_start )
			{
				// check end for a new run of silence
				long silence = count_silence( out + pos, remain );
				if ( silence < remain )
					this_->silence_time = this_->emu_time - silence;
				
				if ( this_->emu_time - this_->silence_time >= buf_size )
					fill_buf( this_ ); // cause silence detection on next play()
			}
		}
		
		if ( this_->out_time > this_->fade_start )
			handle_fade( this_, out_count, out );
	}
	this_->out_time += out_count;
	return 0;
}
