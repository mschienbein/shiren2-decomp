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
    void *rs_state;   /* 0x40 */
    f32 ratio;        /* 0x44 */
    s32 upitch;       /* 0x48 */
    f32 delta_f;      /* 0x4C */
    s32 rs_first;     /* 0x50 */
    void *state;      /* 0x54 */
    s16 pan;          /* 0x58 */
    s16 volume;       /* 0x5A */
    s16 cvolL;        /* 0x5C */
    s16 cvolR;        /* 0x5E */
    s16 dryamt;       /* 0x60 */
    s16 wetamt;       /* 0x62 */
    u16 lratl;        /* 0x64 */
    s16 lratm;        /* 0x66 */
    s16 ltgt;         /* 0x68 */
    u16 rratl;        /* 0x6A */
    s16 rratm;        /* 0x6C */
    s16 rtgt;         /* 0x6E */
    s32 delta;        /* 0x70 */
    s32 segEnd;       /* 0x74 */
    s32 first;        /* 0x78 */
    void *ctrlList;   /* 0x7C */
    void *ctrlTail;   /* 0x80 */
    s32 motion;       /* 0x84 */
} Voice;

extern s16 D_80148C60[]; /* eqpower[128] */
Acmd *func_8012F160(void *filter, s16 *inp, Acmd *p);
s16 func_8012E8B0(f64 vol, f64 tgt, s32 count, u16 *ratel);
u32 func_800340F0(void *addr);

/*
 * _pullSubFrame variant of this voice mixer: pull one 184-sample frame from
 * the resampler, then emit the envelope set-up (first frame) and the
 * envelope-mixer command.  outp is supplied by every caller (func_8012E040)
 * but this mixer writes to fixed buffers and does not use it.
 */
Acmd *func_8012E698(void *filter, s16 *inp, s16 *outp, s32 outCount, Acmd *p)
{
    Acmd *ptr = p;
    Voice *e = (Voice *)filter;

    if (e->motion != 1 /* AL_PLAYING */ || outCount == 0) {
        return ptr;
    }
    ptr = func_8012F160(e, inp, ptr);
    if (e->first) {
        e->first = 0;
        e->ltgt = (e->volume * D_80148C60[e->pan]) >> 15;
        e->lratm = func_8012E8B0(e->cvolL, e->ltgt, e->segEnd, &e->lratl);
        e->rtgt = (e->volume * D_80148C60[0x7F - e->pan]) >> 15;
        e->rratm = func_8012E8B0(e->cvolR, e->rtgt, e->segEnd, &e->rratl);
        {
            Acmd *a = ptr++;
            a->w0 = 0x09000000 | (u16)e->ltgt;
            a->w1 = ((u16)e->lratm << 16) | e->lratl;
        }
        {
            Acmd *a = ptr++;
            a->w0 = 0x09060000 | (u16)e->cvolL;
            a->w1 = ((u16)e->dryamt << 16) | (u16)e->wetamt;
        }
        {
            Acmd *a = ptr++;
            a->w0 = 0x09040000 | (u16)e->rtgt;
            a->w1 = ((u16)e->rratm << 16) | e->rratl;
        }
        {
            Acmd *a = ptr++;
            a->w0 = 0x03010000 | (u16)e->cvolR;
            a->w1 = func_800340F0(e->state);
        }
    } else {
        Acmd *a = ptr++;
        a->w0 = 0x03000000;
        a->w1 = func_800340F0(e->state);
    }
    *inp += 0x170;
    e->delta += 0xB8;
    return ptr;
}
