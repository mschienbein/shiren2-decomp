#include "common.h"

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef float f32;
typedef double f64;

typedef struct {
    u32 w0;
    u32 w1;
} Acmd;

typedef struct {
    u8 pad0[0x40];
    void *state;      /* 0x40 */
    f32 ratio;        /* 0x44 */
    s32 upitch;       /* 0x48 */
    f32 delta;        /* 0x4C */
    s32 first;        /* 0x50 */
} Resampler;

Acmd *func_8012EA60(void *filter, s16 *outp, s32 outCount, Acmd *p);
u32 func_800340F0(void *addr);

#define UNITY_PITCH 0x8000
#define MAX_RATIO 1.99996
#define FIXED_SAMPLE 184

/* alResamplePull variant: decode FIXED_SAMPLE output samples through the
 * ADPCM loader, then copy them (unity pitch) or resample them into *outp. */
Acmd *func_8012F160(void *filter, s16 *outp, Acmd *p)
{
    Acmd *ptr = p;
    Resampler *f = (Resampler *)filter;
    s16 inp;
    s32 incount;
    s32 ratio;
    f32 fincount;

    inp = FIXED_SAMPLE << 1;
    if (f->upitch) {
        ptr = func_8012EA60(f, &inp, FIXED_SAMPLE, ptr);
        {
            Acmd *a = ptr++;
            a->w0 = 0x0A000000 | (inp & 0xFFFFFF);
            a->w1 = ((u16)*outp << 16) | (FIXED_SAMPLE << 1);
        }
    } else {
        if (f->ratio > MAX_RATIO) {
            f->ratio = (f32)MAX_RATIO;
        }
        f->ratio = (s32)(f->ratio * UNITY_PITCH);
        f->ratio = f->ratio / UNITY_PITCH;
        fincount = f->delta + (f->ratio * (f32)FIXED_SAMPLE);
        incount = (s32)fincount;
        f->delta = fincount - (f32)incount;
        ptr = func_8012EA60(f, &inp, incount, ptr);
        ratio = (s32)(f->ratio * UNITY_PITCH);
        {
            Acmd *a = ptr++;
            a->w0 = 0x05000000 | (func_800340F0(f->state) & 0xFFFFFF);
            a->w1 = (f->first << 30) | ((ratio & 0xFFFF) << 14) | ((inp & 0xFFF) << 2);
        }
        f->first = 0;
    }
    return ptr;
}
