/* libultra libaudio reverb.c: alFxPull, alFxParam, alFxParamHdl and statics. */
#include "common.h"

typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;
typedef float f32;

typedef union {
    struct {
        u32 w0;
        u32 w1;
    } words;
    long long force_structure_alignment;
} Acmd;

typedef struct ALFilter_s ALFilter;
typedef Acmd *(*ALCmdHandler)(void *, s16 *, s32, s32, Acmd *);
typedef s32 (*ALSetParam)(void *, s32, void *);

struct ALFilter_s {
    ALFilter *source;
    ALCmdHandler handler;
    ALSetParam setParam;
    s16 inp;
    s16 outp;
    s32 type;
};

typedef struct {
    ALFilter filter;
    void *state;
    f32 ratio;
    s32 upitch;
    f32 delta;
    s32 first;
} ALResampler;

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
    ALResampler *rs;
} ALDelay;

typedef struct {
    ALFilter filter;
    s16 *base;
    s16 *input;
    u32 length;
    ALDelay *delay;
    u8 section_count;
    ALSetParam paramHdl;
} ALFx;

typedef struct {
    u8 pad[0x44];
    s32 outputRate;
} ALGlobals;

extern ALGlobals *D_80037320;

u32 func_800340F0(void *addr);
void func_80028500(ALLowPass *lp);

Acmd *func_80030848(ALFx *r, ALDelay *d, s32 buff, s32 incount, Acmd *p);
Acmd *func_80030A60(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, Acmd *p);
Acmd *func_80030BD0(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, Acmd *p);
Acmd *func_80030D40(ALLowPass *lp, s32 buff, s32 count, Acmd *p);
f32 func_80030DDC(ALDelay *d, s32 count);

#define AL_AUX_L_OUT 0x6C0
#define AL_AUX_R_OUT 0x800
#define AL_TEMP_0 0
#define AL_TEMP_1 0x140
#define AL_TEMP_2 0x280

#define _SHIFTL(v, s, w) ((unsigned int)(((unsigned int)(v) & ((0x01 << (w)) - 1)) << (s)))
#define A_CLEARBUFF 2
#define A_LOADBUFF 4
#define A_RESAMPLE 5
#define A_SAVEBUFF 6
#define A_SETBUFF 8
#define A_DMEMMOVE 10
#define A_LOADADPCM 11
#define A_MIX 12
#define A_POLEF 14

#define SWAP(in, out) \
    {                 \
        s16 t = out;  \
        out = in;     \
        in = t;       \
    }

Acmd *func_800302D0(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p)
{
    Acmd *ptr = p;
    ALFx *r = (ALFx *)filter;
    ALFilter *source = r->filter.source;
    s16 i, buff1, buff2, input, output;
    s16 *in_ptr, *out_ptr, gain, *prev_out_ptr = 0;
    ALDelay *d, *pd;

    ptr = (*source->handler)(source, outp, outCount, sampleOffset, p);

    input = AL_AUX_L_OUT;
    output = AL_AUX_R_OUT;
    buff1 = AL_TEMP_0;
    buff2 = AL_TEMP_1;

    { Acmd *_a = (Acmd *)ptr++; _a->words.w0 = _SHIFTL(A_SETBUFF, 24, 8) | _SHIFTL(0, 16, 8) | _SHIFTL(0, 0, 16); _a->words.w1 = _SHIFTL(0, 16, 16) | _SHIFTL(outCount << 1, 0, 16); }
    { Acmd *_a = (Acmd *)ptr++; _a->words.w0 = _SHIFTL(A_MIX, 24, 8) | _SHIFTL(0, 16, 8) | _SHIFTL(0xDA83, 0, 16); _a->words.w1 = _SHIFTL(AL_AUX_L_OUT, 16, 16) | _SHIFTL(input, 0, 16); }
    { Acmd *_a = (Acmd *)ptr++; _a->words.w0 = _SHIFTL(A_MIX, 24, 8) | _SHIFTL(0, 16, 8) | _SHIFTL(0x5A82, 0, 16); _a->words.w1 = _SHIFTL(AL_AUX_R_OUT, 16, 16) | _SHIFTL(input, 0, 16); }
    ptr = func_80030BD0(r, r->input, input, outCount, ptr);
    { Acmd *_a = (Acmd *)ptr++; _a->words.w0 = _SHIFTL(A_CLEARBUFF, 24, 8) | _SHIFTL(output, 0, 24); _a->words.w1 = (u32)(outCount << 1); }

    for (i = 0; i < r->section_count; i++) {
        d = &r->delay[i];
        in_ptr = &r->input[-d->input];
        out_ptr = &r->input[-d->output];

        if (in_ptr == prev_out_ptr) {
            SWAP(buff1, buff2);
        } else {
            ptr = func_80030A60(r, in_ptr, buff1, outCount, ptr);
        }
        ptr = func_80030848(r, d, buff2, outCount, ptr);

        if (d->ffcoef) {
            { Acmd *_a = (Acmd *)ptr++; _a->words.w0 = _SHIFTL(A_MIX, 24, 8) | _SHIFTL(0, 16, 8) | _SHIFTL((u16)d->ffcoef, 0, 16); _a->words.w1 = _SHIFTL(buff1, 16, 16) | _SHIFTL(buff2, 0, 16); }
            if (!d->rs && !d->lp) {
                ptr = func_80030BD0(r, out_ptr, buff2, outCount, ptr);
            }
        }

        if (d->fbcoef) {
            { Acmd *_a = (Acmd *)ptr++; _a->words.w0 = _SHIFTL(A_MIX, 24, 8) | _SHIFTL(0, 16, 8) | _SHIFTL((u16)d->fbcoef, 0, 16); _a->words.w1 = _SHIFTL(buff2, 16, 16) | _SHIFTL(buff1, 0, 16); }
            ptr = func_80030BD0(r, in_ptr, buff1, outCount, ptr);
        }

        if (d->lp) {
            ptr = func_80030D40(d->lp, buff2, outCount, ptr);
        }

        if (!d->rs) {
            ptr = func_80030BD0(r, out_ptr, buff2, outCount, ptr);
        }

        if (d->gain) {
            { Acmd *_a = (Acmd *)ptr++; _a->words.w0 = _SHIFTL(A_MIX, 24, 8) | _SHIFTL(0, 16, 8) | _SHIFTL((u16)d->gain, 0, 16); _a->words.w1 = _SHIFTL(buff2, 16, 16) | _SHIFTL(output, 0, 16); }
        }

        prev_out_ptr = &r->input[d->output];
    }

    r->input += outCount;
    if (r->input > &r->base[r->length]) {
        r->input -= r->length;
    }

    { Acmd *_a = (Acmd *)ptr++; _a->words.w0 = _SHIFTL(A_DMEMMOVE, 24, 8) | _SHIFTL(output, 0, 24); _a->words.w1 = _SHIFTL(AL_AUX_L_OUT, 16, 16) | _SHIFTL(outCount << 1, 0, 16); }

    return ptr;
}

s32 func_80030644(void *filter, s32 paramID, void *param)
{
    if (paramID == 1) {
        ((ALFilter *)filter)->source = (ALFilter *)param;
    }
    return 0;
}

#define RANGE 2.0
#define CONVERT 173123.404906676
#define LENGTH (f->delay[s].output - f->delay[s].input)

s32 func_80030658(void *filter, s32 paramID, void *param)
{
    ALFx *f = (ALFx *)filter;
    s32 p = (paramID - 2) % 8;
    s32 s = (paramID - 2) / 8;
    s32 val = *(s32 *)param;

    switch (p) {
        case 0:
            f->delay[s].input = (u32)val & 0xFFFFFFF8;
            break;
        case 1:
            f->delay[s].output = (u32)val & 0xFFFFFFF8;
            break;
        case 3:
            f->delay[s].ffcoef = (s16)val;
            break;
        case 2:
            f->delay[s].fbcoef = (s16)val;
            break;
        case 4:
            f->delay[s].gain = (s16)val;
            break;
        case 5:
            f->delay[s].rsinc = ((((f32)val) / 1000) * RANGE) / D_80037320->outputRate;
            break;
        case 6:
            f->delay[s].rsgain = (((f32)val) / CONVERT) * LENGTH;
            break;
        case 7:
            if (f->delay[s].lp) {
                f->delay[s].lp->fc = (s16)val;
                func_80028500(f->delay[s].lp);
            }
            break;
    }
    return 0;
}

Acmd *func_80030848(ALFx *r, ALDelay *d, s32 buff, s32 incount, Acmd *p)
{
    Acmd *ptr = p;
    s32 ratio, count, rbuff = AL_TEMP_2;
    s16 *out_ptr;
    f32 fincount, fratio, delta;
    s32 ramalign = 0, length;

    if (d->rs) {
        length = d->output - d->input;
        delta = func_80030DDC(d, incount);
        delta /= length;
        delta = (s32)(delta * 32768.0f);
        delta = delta / 32768.0f;
        fratio = 1.0 - delta;

        fincount = d->rs->delta + (fratio * (f32)incount);
        count = (s32)fincount;
        d->rs->delta = fincount - (f32)count;

        out_ptr = &r->input[-(d->output - d->rsdelta)];
        ramalign = ((s32)out_ptr & 0x7) >> 1;

        ptr = func_80030A60(r, out_ptr - ramalign, rbuff, count + ramalign, ptr);

        ratio = (s32)(fratio * 32768.0f);
        { Acmd *_a = ptr++; _a->words.w0 = 0x08000000 | (u16)(rbuff + (ramalign << 1)); _a->words.w1 = (buff << 16) | (u16)(incount << 1); }
        { Acmd *_a = ptr++; _a->words.w0 = 0x05000000 | ((u8)d->rs->first << 16) | (u16)ratio; _a->words.w1 = func_800340F0(d->rs->state); }

        d->rs->first = 0;
        d->rsdelta += count - incount;
    } else {
        out_ptr = &r->input[-d->output];
        ptr = func_80030A60(r, out_ptr, buff, incount, ptr);
    }

    return ptr;
}

Acmd *func_80030A60(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, Acmd *p)
{
    Acmd *ptr = p;
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

        { Acmd *_a = ptr++; _a->words.w0 = 0x08000000 | (u16)buff; _a->words.w1 = (u16)(before_end << 1); }
        { Acmd *_a = ptr++; _a->words.w0 = 0x04000000; _a->words.w1 = func_800340F0(curr_ptr); }
        { Acmd *_a = ptr++; _a->words.w0 = 0x08000000 | (u16)(buff + (before_end << 1)); _a->words.w1 = (u16)(after_end << 1); }
        { Acmd *_a = ptr++; _a->words.w0 = 0x04000000; _a->words.w1 = func_800340F0(r->base); }
    } else {
        { Acmd *_a = ptr++; _a->words.w0 = 0x08000000 | (u16)buff; _a->words.w1 = (u16)(count << 1); }
        { Acmd *_a = ptr++; _a->words.w0 = 0x04000000; _a->words.w1 = func_800340F0(curr_ptr); }
    }

    { Acmd *_a = ptr++; _a->words.w0 = 0x08000000; _a->words.w1 = (u16)(count << 1); }

    return ptr;
}

Acmd *func_80030BD0(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, Acmd *p)
{
    Acmd *ptr = p;
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

        { Acmd *_a = ptr++; _a->words.w0 = 0x08000000; _a->words.w1 = (buff << 16) | (u16)(before_end << 1); }
        { Acmd *_a = ptr++; _a->words.w0 = 0x06000000; _a->words.w1 = func_800340F0(curr_ptr); }
        { Acmd *_a = ptr++; _a->words.w0 = 0x08000000; _a->words.w1 = ((buff + (before_end << 1)) << 16) | (u16)(after_end << 1); }
        { Acmd *_a = ptr++; _a->words.w0 = 0x06000000; _a->words.w1 = func_800340F0(r->base); }
        { Acmd *_a = ptr++; _a->words.w0 = 0x08000000; _a->words.w1 = (u16)(count << 1); }
    } else {
        { Acmd *_a = ptr++; _a->words.w0 = 0x08000000; _a->words.w1 = (buff << 16) | (u16)(count << 1); }
        { Acmd *_a = ptr++; _a->words.w0 = 0x06000000; _a->words.w1 = func_800340F0(curr_ptr); }
    }

    return ptr;
}

Acmd *func_80030D40(ALLowPass *lp, s32 buff, s32 count, Acmd *p)
{
    Acmd *ptr = p;

    { Acmd *_a = (Acmd *)ptr++; _a->words.w0 = _SHIFTL(A_SETBUFF, 24, 8) | _SHIFTL(0, 16, 8) | _SHIFTL(buff, 0, 16); _a->words.w1 = _SHIFTL(buff, 16, 16) | _SHIFTL(count << 1, 0, 16); }
    { Acmd *_a = (Acmd *)ptr++; _a->words.w0 = _SHIFTL(A_LOADADPCM, 24, 8) | _SHIFTL(32, 0, 24); _a->words.w1 = (unsigned int)func_800340F0(lp->fcvec.fccoef); }
    { Acmd *_a = (Acmd *)ptr++; _a->words.w0 = _SHIFTL(A_POLEF, 24, 8) | _SHIFTL(lp->first, 16, 8) | _SHIFTL(lp->fgain, 0, 16); _a->words.w1 = (unsigned int)func_800340F0(lp->fstate); }
    lp->first = 0;

    return ptr;
}

f32 func_80030DDC(ALDelay *d, s32 count)
{
    f32 val;

    d->rsval += d->rsinc * count;
    d->rsval = (d->rsval > RANGE) ? d->rsval - (RANGE * 2) : d->rsval;

    val = d->rsval;
    val = (val < 0) ? -val : val;

    val -= RANGE / 2;

    return d->rsgain * val;
}
