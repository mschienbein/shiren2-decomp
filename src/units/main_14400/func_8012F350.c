/* libultra libaudio reverb.c alFxPull (main-image copy for the fixed 184-sample driver frame). */
#include "common.h"

typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;
typedef float f32;

typedef struct { u32 w0, w1; } AudioCommand;

typedef struct {
    s16 fc;
    s16 fgain;
    union {
        s16 fccoef[16];
        long long force_aligned;
    } fcvec;
    void *fstate;
    s32 first;
} ALLowPass;

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
    ALLowPass *lp;
    void *rs;
} ALDelay;

typedef struct {
    u8 filter[0x14];
    s16 *base;
    s16 *input;
    u32 length;
    ALDelay *delay;
    u8 section_count;
} ALFx;

typedef struct {
    u8 pad0[0x20];
    ALFx *fx;
} AudioVoiceList;

typedef struct {
    u8 pad0[0x34];
    AudioVoiceList *voiceList;
} AudioPlayer;

extern AudioPlayer *D_80148D84;

AudioCommand *func_8012DA60(s32 sampleCount, AudioCommand *commands);
AudioCommand *func_8012F848(ALFx *r, ALDelay *d, s32 buff, AudioCommand *p);
AudioCommand *func_8012FA64(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, AudioCommand *p);
AudioCommand *func_8012FB8C(ALFx *r, s16 *curr_ptr, s32 buff, AudioCommand *p);
AudioCommand *func_8012FCA4(ALLowPass *lp, s32 buff, AudioCommand *p);

#define FIXED_SAMPLE 184
#define AL_AUX_L_OUT 0x7C0
#define AL_AUX_R_OUT 0x930
#define AL_TEMP_0 0
#define AL_TEMP_1 0x170

#define _SHIFTL(v, s, w) ((unsigned int)(((unsigned int)(v) & ((0x01 << (w)) - 1)) << (s)))
#define A_CLEARBUFF 2
#define A_DMEMMOVE 10
#define A_MIX 12

#define aClearBuffer(pkt, d, c)                                              \
    {                                                                        \
        AudioCommand *_a = (pkt);                                            \
        _a->w0 = _SHIFTL(A_CLEARBUFF, 24, 8) | _SHIFTL(d, 0, 24);            \
        _a->w1 = (unsigned int)(c);                                          \
    }
#define aMix(pkt, f, g, i, o)                                                \
    {                                                                        \
        AudioCommand *_a = (pkt);                                            \
        _a->w0 = _SHIFTL(A_MIX, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(g, 0, 16); \
        _a->w1 = _SHIFTL(i, 16, 16) | _SHIFTL(o, 0, 16);                    \
    }
#define aDMEMMove(pkt, i, o, c)                                              \
    {                                                                        \
        AudioCommand *_a = (pkt);                                            \
        _a->w0 = _SHIFTL(A_DMEMMOVE, 24, 8) | _SHIFTL(i, 0, 24);             \
        _a->w1 = _SHIFTL(o, 16, 16) | _SHIFTL(c, 0, 16);                    \
    }

#define SWAP(in, out) \
    {                 \
        s16 t = out;  \
        out = in;     \
        in = t;       \
    }

/* Driver callback: render the voices, then run every reverb delay section over the aux bus. */
AudioCommand *func_8012F350(s32 sampleCount, AudioCommand *commands)
{
    AudioCommand *ptr;
    ALFx *r = D_80148D84->voiceList->fx;
    s16 i, buff1, buff2, input, output;
    s16 *in_ptr, *out_ptr, *prev_out_ptr = 0;
    ALDelay *d;

    ptr = func_8012DA60(sampleCount, commands);

    input = AL_AUX_L_OUT;
    output = AL_AUX_R_OUT;
    buff1 = AL_TEMP_0;
    buff2 = AL_TEMP_1;

    aMix(ptr++, 0, 0xDA83, AL_AUX_L_OUT, input);
    aMix(ptr++, 0, 0x5A82, AL_AUX_R_OUT, input);
    ptr = func_8012FB8C(r, r->input, input, ptr);
    aClearBuffer(ptr++, output, FIXED_SAMPLE << 1);

    for (i = 0; i < r->section_count; i++) {
        d = &r->delay[i];
        in_ptr = &r->input[-d->input];
        out_ptr = &r->input[-d->output];

        if (in_ptr == prev_out_ptr) {
            SWAP(buff1, buff2);
        } else {
            ptr = func_8012FA64(r, in_ptr, buff1, FIXED_SAMPLE, ptr);
        }
        ptr = func_8012F848(r, d, buff2, ptr);

        if (d->ffcoef) {
            aMix(ptr++, 0, (u16)d->ffcoef, buff1, buff2);
            if (!d->rs && !d->lp) {
                ptr = func_8012FB8C(r, out_ptr, buff2, ptr);
            }
        }

        if (d->fbcoef) {
            aMix(ptr++, 0, (u16)d->fbcoef, buff2, buff1);
            ptr = func_8012FB8C(r, in_ptr, buff1, ptr);
        }

        if (d->lp) {
            ptr = func_8012FCA4(d->lp, buff2, ptr);
        }

        if (!d->rs) {
            ptr = func_8012FB8C(r, out_ptr, buff2, ptr);
        }

        if (d->gain) {
            aMix(ptr++, 0, (u16)d->gain, buff2, output);
        }

        prev_out_ptr = &r->input[d->output];
    }

    r->input += FIXED_SAMPLE;
    if (r->input > &r->base[r->length]) {
        r->input -= r->length;
    }

    aDMEMMove(ptr++, output, AL_AUX_L_OUT, FIXED_SAMPLE << 1);

    return ptr;
}
