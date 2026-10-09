/* libultra libaudio reverb.c _loadBuffer (main-image copy, A_LOADBUFF carries the byte count). */
#include "common.h"

typedef signed short s16;
typedef unsigned char u8;

typedef struct { u32 w0, w1; } AudioCommand;
/* ALFx view: delay-line base buffer and its length in samples. */
typedef struct {
    u8 filter[0x14];
    s16 *base;
    s16 *input;
    u32 length;
} ALFx;
extern u32 func_800340F0(void *addr); /* osVirtualToPhysical */

/* A_LOADBUFF: load c bytes from DRAM address s into DMEM offset d. */
#define aLoadBuffer(pkt, c, d, s)                                                   \
    {                                                                               \
        AudioCommand *_a = (pkt);                                                   \
        _a->w0 = (4 << 24) | (((u32)(c) & 0xFFF) << 12) | ((u32)(d) & 0xFFF);       \
        _a->w1 = (u32)(s);                                                          \
    }

/* Load count samples starting at curr_ptr from the delay line, splitting at its wrap point. */
AudioCommand *func_8012FA64(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, AudioCommand *p)
{
    AudioCommand *ptr = p;
    s32 after_end, before_end;
    s16 *updated_ptr, *delay_end;

    delay_end = &r->base[r->length];
    if (curr_ptr < r->base) {
        curr_ptr += r->length;
    }
    updated_ptr = curr_ptr + count;
    if (updated_ptr > delay_end) {
        after_end = updated_ptr - delay_end;
        before_end = delay_end - curr_ptr;
        aLoadBuffer(ptr++, before_end << 1, buff, func_800340F0(curr_ptr));
        aLoadBuffer(ptr++, after_end << 1, buff + (before_end << 1), func_800340F0(r->base));
    } else {
        aLoadBuffer(ptr++, count << 1, buff, func_800340F0(curr_ptr));
    }
    return ptr;
}
