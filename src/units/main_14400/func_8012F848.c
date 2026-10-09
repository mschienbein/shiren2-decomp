/* libultra libaudio reverb.c _loadOutputBuffer (main-image copy for the fixed 184-sample frame). */
#include "common.h"

typedef signed short s16;
typedef unsigned char u8;
typedef float f32;

typedef struct { u32 w0, w1; } AudioCommand;

typedef struct {
    u8 filter[0x14];
    void *state;
    f32 ratio;
    s32 upitch;
    f32 delta;
    s32 first;
} ALResampler;

typedef struct {
    u32 input;
    u32 output;
    s16 ffcoef;
    s16 fbcoef;
    s16 gain;
    f32 rsinc;
    f32 rsval;
    s32 rsdelta;
    f32 rsgain;
    void *lp;
    ALResampler *rs;
} ALDelay;

typedef struct {
    u8 filter[0x14];
    s16 *base;
    s16 *input;
    u32 length;
    ALDelay *delay;
    u8 section_count;
} ALFx;

f32 func_80030DDC(ALDelay *d, s32 count); /* _doModFunc */
u32 func_800340F0(void *addr); /* osVirtualToPhysical */
AudioCommand *func_8012FA64(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, AudioCommand *p); /* _loadBuffer */

#define FIXED_SAMPLE 184
#define AL_TEMP_2 0x2E0
#define UNITY_PITCH 0x8000

#define _SHIFTL(v, s, w) ((unsigned int)(((unsigned int)(v) & ((0x01 << (w)) - 1)) << (s)))
#define A_RESAMPLE 5

/* A_RESAMPLE: state address s, first-time flag f, pitch p, DMEM input i, output bank o (buffer >> 8). */
#define aResample(pkt, s, f, p, i, o)                                                    \
    {                                                                                    \
        AudioCommand *_a = (pkt);                                                        \
        _a->w0 = _SHIFTL(A_RESAMPLE, 24, 8) | _SHIFTL(s, 0, 24);                         \
        _a->w1 = _SHIFTL(f, 30, 2) | _SHIFTL(p, 14, 16) | _SHIFTL(i, 2, 12) | _SHIFTL(o, 0, 2); \
    }

AudioCommand *func_8012F848(ALFx *r, ALDelay *d, s32 buff, AudioCommand *p)
{
    AudioCommand *ptr = p;
    s32 ratio, count, rbuff = AL_TEMP_2;
    s32 incount = FIXED_SAMPLE;
    s16 *out_ptr;
    f32 fincount, fratio, delta;
    s32 ramalign = 0, length;
    s32 bank;

    if (d->rs) {
        length = d->output - d->input;
        delta = func_80030DDC(d, incount);
        delta /= length;
        delta = (s32)(delta * UNITY_PITCH);
        delta = delta / UNITY_PITCH;
        fratio = 1.0 - delta;

        fincount = d->rs->delta + (fratio * (f32)incount);
        count = (s32)fincount;
        d->rs->delta = fincount - (f32)count;

        /* ODD_C: libultra reverb ring-buffer idiom (input minus delay offset); the pointer may
         * briefly fall before r->base and func_8012FA64 wraps it (+= r->length) before any access. */
        out_ptr = &r->input[-(d->output - d->rsdelta)];
        /* local-arithmetic-qualification: RAM alignment of the DMA source address in samples. */
        ramalign = ((s32)out_ptr & 0x7) >> 1;
        ptr = func_8012FA64(r, out_ptr - ramalign, rbuff, count + ramalign, ptr);

        ratio = (s32)(fratio * UNITY_PITCH);
        bank = buff >> 8;
        aResample(ptr++, func_800340F0(d->rs->state), d->rs->first, ratio, rbuff + (ramalign << 1), bank);
        d->rs->first = 0;
        d->rsdelta += count - incount;
    } else {
        /* ODD_C: libultra reverb ring-buffer idiom (input minus delay offset); the pointer may
         * briefly fall before r->base and func_8012FA64 wraps it (+= r->length) before any access. */
        out_ptr = &r->input[-d->output];
        ptr = func_8012FA64(r, out_ptr, buff, incount, ptr);
    }

    return ptr;
}
