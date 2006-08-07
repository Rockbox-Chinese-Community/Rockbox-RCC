/***************************************************************************
*             __________               __   ___.
*   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
*   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
*   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
*   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
*                     \/            \/     \/    \/            \/
* $Id$
*
* Greyscale framework
* Scrolling routines
*
* This is a generic framework to display up to 33 shades of grey
* on low-depth bitmap LCDs (Archos b&w, Iriver 4-grey, iPod 4-grey)
* within plugins.
*
* Copyright (C) 2004-2006 Jens Arnold
*
* All files in this archive are subject to the GNU General Public License.
* See the file COPYING in the source tree root for full license agreement.
*
* This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
* KIND, either express or implied.
*
****************************************************************************/

#include "plugin.h"

#ifdef HAVE_LCD_BITMAP
#include "gray.h"

/*** Scrolling ***/

#if LCD_PIXELFORMAT == HORIZONTAL_PACKING

/* Scroll left */
void gray_scroll_left(int count)
{
    unsigned char *data, *data_end;
    int length, blank;

    if ((unsigned)count >= (unsigned)_gray_info.width)
        return;

    data = _gray_info.cur_buffer;
    data_end = data + MULU16(_gray_info.width, _gray_info.height);
    length = _gray_info.width - count;
    blank = (_gray_info.drawmode & DRMODE_INVERSEVID) ?
            _gray_info.fg_index : _gray_info.bg_index;

    do
    {
        _gray_rb->memmove(data, data + count, length);
        _gray_rb->memset(data + length, blank, count);
        data += _gray_info.width;
    }
    while (data < data_end);
}

/* Scroll right */
void gray_scroll_right(int count)
{
    unsigned char *data, *data_end;
    int length, blank;

    if ((unsigned)count >= (unsigned)_gray_info.width)
        return;

    data = _gray_info.cur_buffer;
    data_end = data + MULU16(_gray_info.width, _gray_info.height);
    length = _gray_info.width - count;
    blank = (_gray_info.drawmode & DRMODE_INVERSEVID) ?
            _gray_info.fg_index : _gray_info.bg_index;

    do
    {
        _gray_rb->memmove(data + count, data, length);
        _gray_rb->memset(data, blank, count);
        data += _gray_info.width;
    }
    while (data < data_end);
}

/* Scroll up */
void gray_scroll_up(int count)
{
    long shift, length;
    int blank;

    if ((unsigned)count >= (unsigned)_gray_info.height)
        return;

    shift = MULU16(_gray_info.width, count);
    length = MULU16(_gray_info.width, _gray_info.height - count);
    blank = (_gray_info.drawmode & DRMODE_INVERSEVID) ?
            _gray_info.fg_index : _gray_info.bg_index;

    _gray_rb->memmove(_gray_info.cur_buffer, _gray_info.cur_buffer + shift,
                      length);
    _gray_rb->memset(_gray_info.cur_buffer + length, blank, shift);
}

/* Scroll down */
void gray_scroll_down(int count)
{
    long shift, length;
    int blank;

    if ((unsigned)count >= (unsigned)_gray_info.height)
        return;

    shift = MULU16(_gray_info.width, count);
    length = MULU16(_gray_info.width, _gray_info.height - count);
    blank = (_gray_info.drawmode & DRMODE_INVERSEVID) ?
            _gray_info.fg_index : _gray_info.bg_index;

    _gray_rb->memmove(_gray_info.cur_buffer + shift, _gray_info.cur_buffer,
                      length);
    _gray_rb->memset(_gray_info.cur_buffer, blank, shift);
}

#else /* LCD_PIXELFORMAT == VERTICAL_PACKING */

/* Scroll left */
void gray_scroll_left(int count)
{
    long shift, length;
    int blank;

    if ((unsigned)count >= (unsigned)_gray_info.width)
        return;

    shift = MULU16(_gray_info.height, count);
    length = MULU16(_gray_info.height, _gray_info.width - count);
    blank = (_gray_info.drawmode & DRMODE_INVERSEVID) ?
            _gray_info.fg_index : _gray_info.bg_index;

    _gray_rb->memmove(_gray_info.cur_buffer, _gray_info.cur_buffer + shift,
                      length);
    _gray_rb->memset(_gray_info.cur_buffer + length, blank, shift);
}

/* Scroll right */
void gray_scroll_right(int count)
{
    long shift, length;
    int blank;

    if ((unsigned)count >= (unsigned)_gray_info.width)
        return;

    shift = MULU16(_gray_info.height, count);
    length = MULU16(_gray_info.height, _gray_info.width - count);
    blank = (_gray_info.drawmode & DRMODE_INVERSEVID) ?
            _gray_info.fg_index : _gray_info.bg_index;

    _gray_rb->memmove(_gray_info.cur_buffer + shift, _gray_info.cur_buffer,
                      length);
    _gray_rb->memset(_gray_info.cur_buffer, blank, shift);
}

/* Scroll up */
void gray_scroll_up(int count)
{
    unsigned char *data, *data_end;
    int length, blank;

    if ((unsigned)count >= (unsigned)_gray_info.height)
        return;

    data = _gray_info.cur_buffer;
    data_end = data + MULU16(_gray_info.width, _gray_info.height);
    length = _gray_info.height - count;
    blank = (_gray_info.drawmode & DRMODE_INVERSEVID) ?
            _gray_info.fg_index : _gray_info.bg_index;

    do
    {
        _gray_rb->memmove(data, data + count, length);
        _gray_rb->memset(data + length, blank, count);
        data += _gray_info.height;
    }
    while (data < data_end);
}

/* Scroll down */
void gray_scroll_down(int count)
{
    unsigned char *data, *data_end;
    int length, blank;

    if ((unsigned)count >= (unsigned)_gray_info.height)
        return;

    data = _gray_info.cur_buffer;
    data_end = data + MULU16(_gray_info.width, _gray_info.height);
    length = _gray_info.height - count;
    blank = (_gray_info.drawmode & DRMODE_INVERSEVID) ?
            _gray_info.fg_index : _gray_info.bg_index;

    do
    {
        _gray_rb->memmove(data + count, data, length);
        _gray_rb->memset(data, blank, count);
        data += _gray_info.height;
    }
    while (data < data_end);
}
#endif /* LCD_PIXELFORMAT */

/*** Unbuffered scrolling functions ***/

#ifdef SIMULATOR

/* Scroll left */
void gray_ub_scroll_left(int count)
{
    gray_scroll_left(count);
    gray_update();
}

/* Scroll right */
void gray_ub_scroll_right(int count)
{
    gray_scroll_right(count);
    gray_update();
}

/* Scroll up */
void gray_ub_scroll_up(int count)
{
    gray_scroll_up(count);
    gray_update();
}

/* Scroll down */
void gray_ub_scroll_down(int count)
{
    gray_scroll_down(count);
    gray_update();
}

#else /* !SIMULATOR */

#if LCD_PIXELFORMAT == HORIZONTAL_PACKING

/* Scroll left */
void gray_ub_scroll_left(int count)
{
    int shift, length;
    unsigned char *ptr, *ptr_end;

    if ((unsigned) count >= (unsigned) _gray_info.width)
        return;

    shift = count >> 3;
    count &= 7;

    if (shift)
    {
        length = _gray_info.bwidth - shift;
        ptr = _gray_info.plane_data;
        ptr_end = ptr + _gray_info.plane_size;

        /* Scroll row by row to minimize flicker */
        do
        {
            unsigned char *ptr_row = ptr;
            unsigned char *row_end = ptr_row
                             + MULU16(_gray_info.plane_size, _gray_info.depth);
            do
            {
                _gray_rb->memmove(ptr_row, ptr_row + shift, length);
                _gray_rb->memset(ptr_row + length, 0, shift);
                ptr_row += _gray_info.plane_size;
            }
            while (ptr_row < row_end);

            ptr += _gray_info.bwidth;
        }
        while (ptr < ptr_end);
    }
    if (count)
    {
        asm (
            "mov     r4, %[high]     \n"

        ".sl_rloop:                  \n"
            "mov     r5, %[addr]     \n"
            "mov     r2, %[dpth]     \n"

        ".sl_oloop:                  \n"
            "mov     r6, r5          \n"
            "mov     r3, %[cols]     \n"
            "mov     r1, #0          \n"

        ".sl_iloop:                          \n"
            "mov     r1, r1, lsr #8          \n"
            "ldrb    r0, [r6, #-1]!          \n"
            "orr     r1, r1, r0, lsl %[cnt]  \n"
            "strb    r1, [r6]                \n"

            "subs    r3, r3, #1      \n"
            "bne     .sl_iloop       \n"

            "add     r5, r5, %[psiz] \n"
            "subs    r2, r2, #1      \n"
            "bne     .sl_oloop       \n"

            "add     %[addr],%[addr],%[bwid] \n"
            "subs    r4, r4, #1              \n"
            "bne     .sl_rloop               \n"
            : /* outputs */
            : /* inputs */
            [dpth]"r"(_gray_info.depth),
            [high]"r"(_gray_info.height),
            [bwid]"r"(_gray_info.bwidth),
            [cols]"r"(_gray_info.bwidth - shift),
            [psiz]"r"(_gray_info.plane_size),
            [addr]"r"(_gray_info.plane_data + _gray_info.bwidth - shift),
            [cnt] "r"(count)
            : /* clobbers */
            "r0", "r1", "r2", "r3", "r4", "r5", "r6"
        );
    }
}

/* Scroll right */
void gray_ub_scroll_right(int count)
{
    int shift, length;
    unsigned char *ptr, *ptr_end;

    if ((unsigned) count >= (unsigned) _gray_info.width)
        return;

    shift = count >> 3;
    count &= 7;

    if (shift)
    {
        length = _gray_info.bwidth - shift;
        ptr = _gray_info.plane_data;
        ptr_end = ptr + _gray_info.plane_size;

        /* Scroll row by row to minimize flicker */
        do
        {
            unsigned char *ptr_row = ptr;
            unsigned char *row_end = ptr_row
                             + MULU16(_gray_info.plane_size, _gray_info.depth);
            do
            {
                _gray_rb->memmove(ptr_row + shift, ptr_row, length);
                _gray_rb->memset(ptr_row, 0, shift);
                ptr_row += _gray_info.plane_size;
            }
            while (ptr_row < row_end);

            ptr += _gray_info.bwidth;
        }
        while (ptr < ptr_end);
    }
    if (count)
    {
        asm (
            "mov     r4, %[high]     \n"

        ".sr_rloop:                  \n"
            "mov     r5, %[addr]     \n"
            "mov     r2, %[dpth]     \n"

        ".sr_oloop:                  \n"
            "mov     r6, r5          \n"
            "mov     r3, %[cols]     \n"
            "mov     r1, #0          \n"

        ".sr_iloop:                          \n"
            "ldrb    r0, [r6]                \n"
            "orr     r1, r0, r1, lsl #8      \n"
            "mov     r0, r1, lsr %[cnt]      \n"
            "strb    r0, [r6], #1            \n"

            "subs    r3, r3, #1      \n"
            "bne     .sr_iloop       \n"

            "add     r5, r5, %[psiz] \n"
            "subs    r2, r2, #1      \n"
            "bne     .sr_oloop       \n"

            "add     %[addr],%[addr],%[bwid] \n"
            "subs    r4, r4, #1              \n"
            "bne     .sr_rloop               \n"
            : /* outputs */
            : /* inputs */
            [dpth]"r"(_gray_info.depth),
            [high]"r"(_gray_info.height),
            [bwid]"r"(_gray_info.bwidth),
            [cols]"r"(_gray_info.bwidth - shift),
            [psiz]"r"(_gray_info.plane_size),
            [addr]"r"(_gray_info.plane_data + shift),
            [cnt] "r"(count)
            : /* clobbers */
            "r0", "r1", "r2", "r3", "r4", "r5", "r6"
        );
    }
}

/* Scroll up */
void gray_ub_scroll_up(int count)
{
    long blockshift;
    unsigned char *ptr, *ptr_end1, *ptr_end2;

    if ((unsigned) count >= (unsigned) _gray_info.height)
        return;

    blockshift = MULU16(_gray_info.bwidth, count);
    ptr      = _gray_info.plane_data;
    ptr_end2 = ptr + _gray_info.plane_size;
    ptr_end1 = ptr_end2 - blockshift;
    /* Scroll row by row to minimize flicker */
    do
    {
        unsigned char *ptr_row = ptr;
        unsigned char *row_end = ptr_row
                             + MULU16(_gray_info.plane_size, _gray_info.depth);
        if (ptr < ptr_end1)
        {
            do
            {
                _gray_rb->memcpy(ptr_row, ptr_row + blockshift,
                                 _gray_info.bwidth);
                ptr_row += _gray_info.plane_size;
            }
            while (ptr_row < row_end);
        }
        else
        {
            do
            {
                _gray_rb->memset(ptr_row, 0, _gray_info.bwidth);
                ptr_row += _gray_info.plane_size;
            }
            while (ptr_row < row_end);
        }

        ptr += _gray_info.bwidth;
    }
    while (ptr < ptr_end2);
}

/* Scroll down */
void gray_ub_scroll_down(int count)
{
    long blockshift;
    unsigned char *ptr, *ptr_end1, *ptr_end2;

    if ((unsigned) count >= (unsigned) _gray_info.height)
        return;

    blockshift = MULU16(_gray_info.bwidth, count);
    ptr_end2 = _gray_info.plane_data;
    ptr_end1 = ptr_end2 + blockshift;
    ptr      = ptr_end2 + _gray_info.plane_size;
    /* Scroll row by row to minimize flicker */
    do
    {
        unsigned char *ptr_row, *row_end;

        ptr -= _gray_info.bwidth;
        ptr_row = ptr;
        row_end = ptr_row + MULU16(_gray_info.plane_size, _gray_info.depth);

        if (ptr >= ptr_end1)
        {
            do
            {
                _gray_rb->memcpy(ptr_row, ptr_row - blockshift,
                                 _gray_info.bwidth);
                ptr_row += _gray_info.plane_size;
            }
            while (ptr_row < row_end);
        }
        else
        {
            do
            {
                _gray_rb->memset(ptr_row, 0, _gray_info.bwidth);
                ptr_row += _gray_info.plane_size;
            }
            while (ptr_row < row_end);
        }
    }
    while (ptr > ptr_end2);
}
#else /* LCD_PIXELFORMAT == VERTICAL_PACKING */

/* Scroll left */
void gray_ub_scroll_left(int count)
{
    int length;
    unsigned char *ptr, *ptr_end;

    if ((unsigned) count >= (unsigned) _gray_info.width)
        return;

    length = _gray_info.width - count;
    ptr = _gray_info.plane_data;
    ptr_end = ptr + _gray_info.plane_size;

    /* Scroll row by row to minimize flicker (pixel block rows) */
    do
    {
        unsigned char *ptr_row = ptr;
        unsigned char *row_end = ptr_row
                             + MULU16(_gray_info.plane_size, _gray_info.depth);
        do
        {
            _gray_rb->memmove(ptr_row, ptr_row + count, length);
            _gray_rb->memset(ptr_row + length, 0, count);
            ptr_row += _gray_info.plane_size;
        }
        while (ptr_row < row_end);

        ptr += _gray_info.width;
    }
    while (ptr < ptr_end);
}

/* Scroll right */
void gray_ub_scroll_right(int count)
{
    int length;
    unsigned char *ptr, *ptr_end;

    if ((unsigned) count >= (unsigned) _gray_info.width)
        return;

    length = _gray_info.width - count;
    ptr = _gray_info.plane_data;
    ptr_end = ptr + _gray_info.plane_size;

    /* Scroll row by row to minimize flicker (pixel block rows) */
    do
    {
        unsigned char *ptr_row = ptr;
        unsigned char *row_end = ptr_row
                             + MULU16(_gray_info.plane_size, _gray_info.depth);
        do
        {
            _gray_rb->memmove(ptr_row + count, ptr_row, length);
            _gray_rb->memset(ptr_row, 0, count);
            ptr_row += _gray_info.plane_size;
        }
        while (ptr_row < row_end);

        ptr += _gray_info.width;
    }
    while (ptr < ptr_end);
}

/* Scroll up */
void gray_ub_scroll_up(int count)
{
    int shift;
    long blockshift = 0;
    unsigned char *ptr, *ptr_end1, *ptr_end2;

    if ((unsigned) count >= (unsigned) _gray_info.height)
        return;

    shift = count >> 3;
    count &= 7;

    if (shift)
    {
        blockshift = MULU16(_gray_info.width, shift);
        ptr      = _gray_info.plane_data;
        ptr_end2 = ptr + _gray_info.plane_size;
        ptr_end1 = ptr_end2 - blockshift;
        /* Scroll row by row to minimize flicker (pixel block rows) */
        do
        {
            unsigned char *ptr_row = ptr;
            unsigned char *row_end = ptr_row
                             + MULU16(_gray_info.plane_size, _gray_info.depth);
            if (ptr < ptr_end1)
            {
                do
                {
                    _gray_rb->memcpy(ptr_row, ptr_row + blockshift,
                                     _gray_info.width);
                    ptr_row += _gray_info.plane_size;
                }
                while (ptr_row < row_end);
            }
            else
            {
                do
                {
                    _gray_rb->memset(ptr_row, 0, _gray_info.width);
                    ptr_row += _gray_info.plane_size;
                }
                while (ptr_row < row_end);
            }

            ptr += _gray_info.width;
        }
        while (ptr < ptr_end2);
    }
    if (count)
    {
#if CONFIG_CPU == SH7034
        /* scroll column by column to minimize flicker */
        asm (
            "mov     #0,r6       \n"  /* x = 0 */
            "mova    .su_shifttbl,r0 \n"  /* calculate jump destination for */
            "mov.b   @(r0,%[cnt]),%[cnt] \n"  /* shift amount from table */
            "bra     .su_cloop   \n"  /* skip table */
            "add     r0,%[cnt]   \n"

            ".align  2           \n"
        ".su_shifttbl:           \n"  /* shift jump offset table */
            ".byte   .su_shift0 - .su_shifttbl \n"
            ".byte   .su_shift1 - .su_shifttbl \n"
            ".byte   .su_shift2 - .su_shifttbl \n"
            ".byte   .su_shift3 - .su_shifttbl \n"
            ".byte   .su_shift4 - .su_shifttbl \n"
            ".byte   .su_shift5 - .su_shifttbl \n"
            ".byte   .su_shift6 - .su_shifttbl \n"
            ".byte   .su_shift7 - .su_shifttbl \n"

        ".su_cloop:              \n"  /* repeat for every column */
            "mov     %[addr],r2  \n"  /* get start address */
            "mov     #0,r3       \n"  /* current_plane = 0 */

        ".su_oloop:              \n"  /* repeat for every bitplane */
            "mov     r2,r4       \n"  /* get start address */
            "mov     #0,r5       \n"  /* current_row = 0 */
            "mov     #0,r1       \n"  /* fill with zero */

        ".su_iloop:              \n"  /* repeat for all rows */
            "sub     %[wide],r4  \n"  /* address -= width */
            "mov.b   @r4,r0      \n"  /* get data byte */
            "shll8   r1          \n"  /* old data to 2nd byte */
            "extu.b  r0,r0       \n"  /* extend unsigned */
            "or      r1,r0       \n"  /* combine old data */
            "jmp     @%[cnt]     \n"  /* jump into shift "path" */
            "extu.b  r0,r1       \n"  /* store data for next round */

        ".su_shift6:             \n"  /* shift right by 0..7 bits */
            "shll2   r0          \n"
            "bra     .su_shift0  \n"
            "shlr8   r0          \n"
        ".su_shift4:             \n"
            "shlr2   r0          \n"
        ".su_shift2:             \n"
            "bra     .su_shift0  \n"
            "shlr2   r0          \n"
        ".su_shift7:             \n"
            "shlr2   r0          \n"
        ".su_shift5:             \n"
            "shlr2   r0          \n"
        ".su_shift3:             \n"
            "shlr2   r0          \n"
        ".su_shift1:             \n"
            "shlr    r0          \n"
        ".su_shift0:             \n"

            "mov.b   r0,@r4      \n"  /* store data */
            "add     #1,r5       \n"  /* current_row++ */
            "cmp/hi  r5,%[rows]  \n"  /* current_row < bheight - shift ? */
            "bt      .su_iloop   \n"

            "add     %[psiz],r2  \n"  /* start_address += plane_size */
            "add     #1,r3       \n"  /* current_plane++ */
            "cmp/hi  r3,%[dpth]  \n"  /* current_plane < depth ? */
            "bt      .su_oloop   \n"

            "add     #1,%[addr]  \n"  /* start_address++ */
            "add     #1,r6       \n"  /* x++ */
            "cmp/hi  r6,%[wide]  \n"  /* x < width ? */
            "bt      .su_cloop   \n"
            : /* outputs */
            : /* inputs */
            [dpth]"r"(_gray_info.depth),
            [addr]"r"(_gray_info.plane_data + _gray_info.plane_size - blockshift),
            [wide]"r"(_gray_info.width),
            [rows]"r"(_gray_info.bheight - shift),
            [psiz]"r"(_gray_info.plane_size),
            [cnt] "r"(count)
            : /* clobbers */
            "r0", "r1", "r2", "r3", "r4", "r5", "r6"
        );
#elif defined(CPU_COLDFIRE)
        /* scroll column by column to minimize flicker */
        asm (
            "move.l  %[wide],%%d4\n"  /* columns = width */

        ".su_cloop:              \n"  /* repeat for every column */
            "move.l  %[addr],%%a0\n"  /* get start address */
            "move.l  %[dpth],%%d2\n"  /* planes = depth */

        ".su_oloop:              \n"  /* repeat for every bitplane */
            "move.l  %%a0,%%a1   \n"  /* get start address */
            "move.l  %[rows],%%d3\n"  /* rows = row_count */
            "clr.l   %%d1        \n"  /* fill with zero */

        ".su_iloop:              \n"  /* repeat for all rows */
            "sub.l   %[wide],%%a1\n"  /* address -= width */

            "clr.l   %%d0        \n"
            "move.b  (%%a1),%%d0 \n"  /* get data byte */
            "lsl.l   #8,%%d1     \n"  /* old data to 2nd byte */
            "or.l    %%d1,%%d0   \n"  /* combine old data */
            "clr.l   %%d1        \n"
            "move.b  %%d0,%%d1   \n"  /* keep data for next round */
            "lsr.l   %[cnt],%%d0 \n"  /* shift right */
            "move.b  %%d0,(%%a1) \n"  /* store data */

            "subq.l  #1,%%d3     \n"  /* rows-- */
            "bne.b   .su_iloop   \n"

            "add.l   %[psiz],%%a0\n"  /* start_address += plane_size */
            "subq.l  #1,%%d2     \n"  /* planes-- */
            "bne.b   .su_oloop   \n"

            "addq.l  #1,%[addr]  \n"  /* start_address++ */
            "subq.l  #1,%%d4     \n"  /* columns-- */
            "bne.b   .su_cloop   \n"
            : /* outputs */
            : /* inputs */
            [psiz]"r"(_gray_info.plane_size),
            [dpth]"r"(_gray_info.depth),
            [wide]"r"(_gray_info.width),
            [rows]"r"(_gray_info.bheight - shift),
            [addr]"a"(_gray_info.plane_data + _gray_info.plane_size - blockshift),
            [cnt] "d"(count)
            : /* clobbers */
            "a0", "a1", "d0", "d1", "d2", "d3", "d4"
        );
#endif
    }
}

/* Scroll down */
void gray_ub_scroll_down(int count)
{
    int shift;
    long blockshift = 0;
    unsigned char *ptr, *ptr_end1, *ptr_end2;

    if ((unsigned) count >= (unsigned) _gray_info.height)
        return;

    shift = count >> 3;
    count &= 7;

    if (shift)
    {
        blockshift = MULU16(_gray_info.width, shift);
        ptr_end2 = _gray_info.plane_data;
        ptr_end1 = ptr_end2 + blockshift;
        ptr      = ptr_end2 + _gray_info.plane_size;
        /* Scroll row by row to minimize flicker (pixel block rows) */
        do
        {
            unsigned char *ptr_row, *row_end;

            ptr -= _gray_info.width;
            ptr_row = ptr;
            row_end = ptr_row + MULU16(_gray_info.plane_size, _gray_info.depth);

            if (ptr >= ptr_end1)
            {
                do
                {
                    _gray_rb->memcpy(ptr_row, ptr_row - blockshift,
                                     _gray_info.width);
                    ptr_row += _gray_info.plane_size;
                }
                while (ptr_row < row_end);
            }
            else
            {
                do
                {
                    _gray_rb->memset(ptr_row, 0, _gray_info.width);
                    ptr_row += _gray_info.plane_size;
                }
                while (ptr_row < row_end);
            }
        }
        while (ptr > ptr_end2);
    }
    if (count)
    {
#if CONFIG_CPU == SH7034
        /* scroll column by column to minimize flicker */
        asm (
            "mov     #0,r6       \n"  /* x = 0 */
            "mova    .sd_shifttbl,r0 \n"  /* calculate jump destination for */
            "mov.b   @(r0,%[cnt]),%[cnt] \n"  /* shift amount from table */
            "bra     .sd_cloop   \n"  /* skip table */
            "add     r0,%[cnt]   \n"

            ".align  2           \n"
        ".sd_shifttbl:           \n"  /* shift jump offset table */
            ".byte   .sd_shift0 - .sd_shifttbl \n"
            ".byte   .sd_shift1 - .sd_shifttbl \n"
            ".byte   .sd_shift2 - .sd_shifttbl \n"
            ".byte   .sd_shift3 - .sd_shifttbl \n"
            ".byte   .sd_shift4 - .sd_shifttbl \n"
            ".byte   .sd_shift5 - .sd_shifttbl \n"
            ".byte   .sd_shift6 - .sd_shifttbl \n"
            ".byte   .sd_shift7 - .sd_shifttbl \n"

        ".sd_cloop:              \n"  /* repeat for every column */
            "mov     %[addr],r2  \n"  /* get start address */
            "mov     #0,r3       \n"  /* current_plane = 0 */

        ".sd_oloop:              \n"  /* repeat for every bitplane */
            "mov     r2,r4       \n"  /* get start address */
            "mov     #0,r5       \n"  /* current_row = 0 */
            "mov     #0,r1       \n"  /* fill with zero */

        ".sd_iloop:              \n"  /* repeat for all rows */
            "shlr8   r1          \n"  /* shift right to get residue */
            "mov.b   @r4,r0      \n"  /* get data byte */
            "jmp     @%[cnt]     \n"  /* jump into shift "path" */
            "extu.b  r0,r0       \n"  /* extend unsigned */

        ".sd_shift6:             \n"  /* shift left by 0..7 bits */
            "shll8   r0          \n"
            "bra     .sd_shift0  \n"
            "shlr2   r0          \n"
        ".sd_shift4:             \n"
            "shll2   r0          \n"
        ".sd_shift2:             \n"
            "bra     .sd_shift0  \n"
            "shll2   r0          \n"
        ".sd_shift7:             \n"
            "shll2   r0          \n"
        ".sd_shift5:             \n"
            "shll2   r0          \n"
        ".sd_shift3:             \n"
            "shll2   r0          \n"
        ".sd_shift1:             \n"
            "shll    r0          \n"
        ".sd_shift0:             \n"

            "or      r0,r1       \n"  /* combine with last residue */
            "mov.b   r1,@r4      \n"  /* store data */
            "add     %[wide],r4  \n"  /* address += width */
            "add     #1,r5       \n"  /* current_row++ */
            "cmp/hi  r5,%[rows]  \n"  /* current_row < bheight - shift ? */
            "bt      .sd_iloop   \n"

            "add     %[psiz],r2  \n"  /* start_address += plane_size */
            "add     #1,r3       \n"  /* current_plane++ */
            "cmp/hi  r3,%[dpth]  \n"  /* current_plane < depth ? */
            "bt      .sd_oloop   \n"

            "add     #1,%[addr]  \n"  /* start_address++ */
            "add     #1,r6       \n"  /* x++ */
            "cmp/hi  r6,%[wide]  \n"  /* x < width ? */
            "bt      .sd_cloop   \n"
            : /* outputs */
            : /* inputs */
            [dpth]"r"(_gray_info.depth),
            [addr]"r"(_gray_info.plane_data + blockshift),
            [wide]"r"(_gray_info.width),
            [rows]"r"(_gray_info.bheight - shift),
            [psiz]"r"(_gray_info.plane_size),
            [cnt] "r"(count)
            : /* clobbers */
            "r0", "r1", "r2", "r3", "r4", "r5", "r6"
        );
#elif defined(CPU_COLDFIRE)
        /* scroll column by column to minimize flicker */
        asm (
            "move.l  %[wide],%%d4\n"  /* columns = width */

        ".sd_cloop:              \n"  /* repeat for every column */
            "move.l  %[addr],%%a0\n"  /* get start address */
            "move.l  %[dpth],%%d2\n"  /* planes = depth */

        ".sd_oloop:              \n"  /* repeat for every bitplane */
            "move.l  %%a0,%%a1   \n"  /* get start address */
            "move.l  %[rows],%%d3\n"  /* rows = row_count */
            "clr.l   %%d1        \n"  /* fill with zero */

        ".sd_iloop:              \n"  /* repeat for all rows */
            "lsr.l   #8,%%d1     \n"  /* shift right to get residue */
            "clr.l   %%d0        \n"
            "move.b  (%%a1),%%d0 \n"  /* get data byte */
            "lsl.l   %[cnt],%%d0 \n"
            "or.l    %%d0,%%d1   \n"  /* combine with last residue */
            "move.b  %%d1,(%%a1) \n"  /* store data */

            "add.l   %[wide],%%a1\n"  /* address += width */
            "subq.l  #1,%%d3     \n"  /* rows-- */
            "bne.b   .sd_iloop   \n"

            "add.l   %[psiz],%%a0\n"  /* start_address += plane_size */
            "subq.l  #1,%%d2     \n"  /* planes-- */
            "bne.b   .sd_oloop   \n"

            "addq.l  #1,%[addr]  \n"  /* start_address++ */
            "subq.l  #1,%%d4     \n"  /* columns-- */
            "bne.b   .sd_cloop   \n"
            : /* outputs */
            : /* inputs */
            [dpth]"r"(_gray_info.depth),
            [wide]"r"(_gray_info.width),
            [rows]"r"(_gray_info.bheight - shift),
            [psiz]"r"(_gray_info.plane_size),
            [addr]"a"(_gray_info.plane_data + blockshift),
            [cnt] "d"(count)
            : /* clobbers */
            "a0", "a1", "d0", "d1", "d2", "d3", "d4"
        );
#endif
    }
}
#endif /* LCD_PIXELFORMAT */

#endif /* !SIMULATOR */

#endif /* HAVE_LCD_BITMAP */
