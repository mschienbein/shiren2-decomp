#include "common.h"

/* libultra libaudio resample.c: alResamplePull + alResampleParam */

typedef signed short s16;
typedef unsigned short u16;
typedef float f32;

typedef union {
    struct {
        u32 w0;
        u32 w1;
    } words;
} Acmd;

#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((1 << (w)) - 1)) << (s)))

#define aDMEMMove(pkt, i, o, c)                                       \
    {                                                                 \
        Acmd *_a = (Acmd *)pkt;                                       \
        _a->words.w0 = _SHIFTL(10, 24, 8) | _SHIFTL(i, 0, 24);        \
        _a->words.w1 = _SHIFTL(o, 16, 16) | _SHIFTL(c, 0, 16);        \
    }

#define aSetBuffer(pkt, f, i, o, c)                                               \
    {                                                                             \
        Acmd *_a = (Acmd *)pkt;                                                   \
        _a->words.w0 = (_SHIFTL(8, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(i, 0, 16)); \
        _a->words.w1 = _SHIFTL(o, 16, 16) | _SHIFTL(c, 0, 16);                    \
    }

#define aResample(pkt, f, p, s)                                                   \
    {                                                                             \
        Acmd *_a = (Acmd *)pkt;                                                   \
        _a->words.w0 = (_SHIFTL(5, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(p, 0, 16)); \
        _a->words.w1 = (u32)(s);                                                  \
    }

typedef struct ALFilter_s {
    struct ALFilter_s *source;
    Acmd *(*handler)(void *, s16 *, s32, s32, Acmd *);
    s32 (*setParam)(void *, s32, void *);
    s16 inp;
    s16 outp;
    s32 type;
} ALFilter;

typedef struct {
    ALFilter filter;
    void *state;   /* 0x14 */
    f32 ratio;     /* 0x18 */
    s32 upitch;    /* 0x1C */
    f32 delta;     /* 0x20 */
    s32 first;     /* 0x24 */
    void *ctrlList; /* 0x28 */
    void *ctrlTail; /* 0x2C */
    s32 motion;    /* 0x30 */
} ALResampler;

#define AL_DECODER_OUT 0x140
#define UNITY_PITCH 0x8000
#define MAX_RATIO 1.99996

#define AL_FILTER_SET_SOURCE 1
#define AL_FILTER_RESET 4
#define AL_FILTER_SET_PITCH 7
#define AL_FILTER_SET_UNITY_PITCH 8
#define AL_FILTER_START 9

#define AL_STOPPED 0
#define AL_PLAYING 1

extern u32 func_800340F0(void *addr); /* osVirtualToPhysical */

Acmd *func_8002FFD0(void *filter, s16 *outp, s32 outCnt, s32 sampleOffset, Acmd *p)
{
    ALResampler *f = (ALResampler *)filter;
    Acmd *ptr = p;
    s16 inp;
    s32 inCount;
    ALFilter *source = f->filter.source;
    s32 incr;
    f32 finCount;

    inp = AL_DECODER_OUT;

    if (!outCnt) {
        return ptr;
    }

    if (f->upitch) {
        ptr = (*source->handler)(source, &inp, outCnt, sampleOffset, p);
        aDMEMMove(ptr++, inp, *outp, outCnt << 1);
    } else {
        if (f->ratio > MAX_RATIO) {
            f->ratio = MAX_RATIO;
        }

        f->ratio = (s32)(f->ratio * UNITY_PITCH);
        f->ratio = f->ratio / UNITY_PITCH;

        finCount = f->delta + (f->ratio * (f32)outCnt);
        inCount = (s32)finCount;
        f->delta = finCount - (f32)inCount;

        ptr = (*source->handler)(source, &inp, inCount, sampleOffset, p);

        incr = (s32)(f->ratio * UNITY_PITCH);
        aSetBuffer(ptr++, 0, inp, *outp, outCnt << 1);
        aResample(ptr++, f->first, incr, func_800340F0(f->state));
        f->first = 0;
    }

    return ptr;
}

s32 func_800301BC(void *filter, s32 paramID, void *param)
{
    ALFilter *f = (ALFilter *)filter;
    ALResampler *r = (ALResampler *)filter;
    union {
        f32 f;
        s32 i;
    } data;

    switch (paramID) {
    case AL_FILTER_SET_SOURCE:
        f->source = (ALFilter *)param;
        break;
    case AL_FILTER_RESET:
        r->delta = 0.0;
        r->first = 1;
        r->motion = AL_STOPPED;
        r->upitch = 0;
        if (f->source) {
            (*f->source->setParam)(f->source, AL_FILTER_RESET, 0);
        }
        break;
    case AL_FILTER_START:
        r->motion = AL_PLAYING;
        if (f->source) {
            (*f->source->setParam)(f->source, AL_FILTER_START, 0);
        }
        break;
    case AL_FILTER_SET_PITCH:
        data.i = (s32)param;
        r->ratio = data.f;
        break;
    case AL_FILTER_SET_UNITY_PITCH:
        r->upitch = 1;
        break;
    default:
        if (f->source) {
            (*f->source->setParam)(f->source, paramID, param);
        }
        break;
    }
    return 0;
}
