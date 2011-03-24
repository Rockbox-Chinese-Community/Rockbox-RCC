
/* Ripped off from Game_Music_Emu 0.5.2. http://www.slack.net/~ant/ */
/* Inflate code taken from WikiViewer plugin by Adam Gashlin */

#include <codecs/lib/codeclib.h>

#include "libvgm/blargg_endian.h"
#include "libvgm/vgm_emu.h" 
#include "libvgm/mallocer.h"
#include "libvgm/inflate.h"

CODEC_HEADER

/* Maximum number of bytes to process in one iteration */
#define CHUNK_SIZE (1024*2)
#define MAINMEMBUF 0

static int16_t samples[CHUNK_SIZE] IBSS_ATTR;
static struct Vgm_Emu vgm_emu IDATA_ATTR CACHEALIGN_ATTR;

static void *inflatebuf;              /* heap for gunzip */
static char *songbuf;               /* destination for uncompressed song */
static uint32_t songbuflen=0;  /* size of the song buffer */
static uint32_t songlen=0;       /* used size of the song buffer */


/****************** rockbox interface ******************/

/* this is the codec entry point */
enum codec_status codec_main(void)
{
    blargg_err_t err;
    uint8_t *buf;
    size_t n;

     uint32_t elapsed_time;

    /* we only render 16 bits */
    ci->configure(DSP_SET_SAMPLE_DEPTH, 16);

    /* 32 Khz, Interleaved stereo */
    ci->configure(DSP_SET_FREQUENCY, 44100);
    ci->configure(DSP_SET_STEREO_MODE, STEREO_INTERLEAVED);

    Vgm_init(&vgm_emu);
    Vgm_set_sample_rate(&vgm_emu, 44100);

next_track:
    elapsed_time = 0;

    DEBUGF("VGM: next_track\n");
    if (codec_init()) {
        return CODEC_ERROR;
    }  

    /* wait for track info to load */
    if (codec_wait_taginfo() != 0)
        goto request_next_track;

    codec_set_replaygain(ci->id3);

    /* Read the entire file */
    DEBUGF("VGM: request file\n");
    ci->seek_buffer(0);
    buf = ci->request_buffer(&n, ci->filesize);
    if (!buf) {
        DEBUGF("VGM: file load failed\n");
        return CODEC_ERROR;
    }
    
    /* If couldn't get the whole buffer
        will trim file and put and 'end_command'
        at the end*/
    if (n < (size_t)ci->filesize) {
        DEBUGF("VGM: file was trimmed\n");
    }
    
    /* If is gzipped decompress it */
    if ( get_le16( buf ) == 0x8b1f ) {
        wpw_init_mempool(MAINMEMBUF);
        inflatebuf=wpw_malloc(MAINMEMBUF,0x13500);
            
        /* Will use available remaining memory 
            as output buffer */
        songbuflen=wpw_available(MAINMEMBUF);
        songbuf=wpw_malloc(MAINMEMBUF,songbuflen);

        songlen=decompress(buf,n,songbuf,songbuflen,0,inflatebuf);  

        if ((err = Vgm_load_mem(&vgm_emu, songbuf, songlen, true))) {
            DEBUGF("VGM: Vgm_load_mem failed (%s)\n", err);
            return CODEC_ERROR;
        }

        /* Since metadata parser doesn't support VGZ 
             will set info it here */
        struct track_info_t* info = &vgm_emu.info;
        ci->id3->title = info->song;
        ci->id3->artist = info->author;
        ci->id3->album = info->game;
        ci->id3->length = Track_get_length( &vgm_emu );
    }
    else if ((err = Vgm_load_mem(&vgm_emu, buf, n, false))) {
        DEBUGF("VGM: Vgm_load failed_mem (%s)\n", err);
        return CODEC_ERROR;
    }

    Vgm_start_track(&vgm_emu); 

    /* for REPEAT_ONE we disable track limits */
    if (ci->global_settings->repeat_mode != REPEAT_ONE) {
        Track_set_fade(&vgm_emu, ci->id3->length, 4000);
    }
    
    ci->set_elapsed(0);

    /* The main decoder loop */
    while (1) {
        ci->yield();
        if (ci->stop_codec || ci->new_track)
            break;

        if (ci->seek_time) {
            ci->set_elapsed(ci->seek_time);
            elapsed_time = ci->seek_time;
            Track_seek(&vgm_emu, ci->seek_time);
            ci->seek_complete();
            
            /* Set fade again in case we seek to start of song */
            Track_set_fade(&vgm_emu, ci->id3->length, 4000);
        }

        /* Generate audio buffer */
        err = Vgm_play(&vgm_emu, CHUNK_SIZE, samples);
        if (err || vgm_emu.track_ended) break;

        ci->pcmbuf_insert(samples, NULL, CHUNK_SIZE >> 1);

        elapsed_time += (CHUNK_SIZE / 2) / 44.1;
        ci->set_elapsed(elapsed_time);
    }

request_next_track:
    if (ci->request_next_track())
        goto next_track; /* when we fall through here we'll reload the file */

    return CODEC_OK;
}
