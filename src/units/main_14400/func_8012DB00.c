#include "common.h"

/* libultra alFxNew without the filter registration (cf. func_800285A4): allocate the
 * reverb and every delay section from the heap, configured from the preset chosen by
 * the configuration's effect type. */

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef long long s64;
typedef float f32;
typedef struct ALHeap ALHeap;

typedef struct ALFilter_s {
    struct ALFilter_s *source;
    void *handler;
    void *setParam;
    s16 inp;
    s16 outp;
    s32 type;
} ALFilter;

typedef struct {
    ALFilter filter;
    void *state;
    f32 ratio;
    s32 upitch;
    f32 delta;
    s32 first;
    void *ctrlList;
    void *ctrlTail;
    s32 motion;
} ALResampler;

typedef struct {
    s16 fc;
    s16 fgain;
    union {
        s16 fccoef[16];
        s64 force_aligned;
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
    void *paramHdl;
} ALFx;

/* Synthesizer configuration (same view as func_801303A0's). */
typedef struct {
    s32 field_0;
    s32 voiceCount;
    s32 nodeCount;
    u8 padC[0x4];
    void *field_10;
    void *heap;
    s32 outputRate;
    u8 fxType;
    s32 *params;
} Config8012DB00;

/* Preset section tables: count, length, then seven words per section. */
extern s32 D_80148AD0[]; /* small room */
extern s32 D_80148B38[]; /* big room */
extern s32 D_80148BC0[]; /* echo */
extern s32 D_80148BE8[]; /* chorus */
extern s32 D_80148C10[]; /* flange */
extern s32 D_80148C38[]; /* none */
void *func_8002AB40(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size);
void func_80028500(ALLowPass *lp);

void func_8012DB00(void **slot, Config8012DB00 *c, void *hp) {
    u16 i, j, k;
    s32 *param;
    ALFx *r;
    ALDelay *d;

    r = func_8002AB40(0, 0, hp, 1, sizeof(ALFx));
    *slot = r;

    switch (c->fxType) {
        case 1: param = D_80148AD0; break;
        case 2: param = D_80148B38; break;
        case 5: param = D_80148BC0; break;
        case 3: param = D_80148BE8; break;
        case 4: param = D_80148C10; break;
        case 6: param = c->params; break;
        default: param = D_80148C38; break;
    }

    j = 0;
    r->section_count = param[j++];
    r->length = param[j++];
    r->delay = func_8002AB40(0, 0, hp, r->section_count, sizeof(ALDelay));
    r->base = func_8002AB40(0, 0, hp, r->length, sizeof(s16));
    r->input = r->base;

    for (k = 0; k < r->length; k++) {
        r->base[k] = 0;
    }

    for (i = 0; i < r->section_count; i++) {
        d = &r->delay[i];
        d->input = param[j++];
        d->output = param[j++];
        d->fbcoef = param[j++];
        d->ffcoef = param[j++];
        d->gain = param[j++];

        if (param[j]) {
            d->rsinc = ((((f32)param[j++]) / 1000) * 2.0) / c->outputRate;
            d->rsgain = (((f32)param[j++]) / 173123.404906676) * (d->output - d->input);
            d->rsval = 1.0;
            d->rsdelta = 0.0;
            d->rs = func_8002AB40(0, 0, hp, 1, sizeof(ALResampler));
            d->rs->state = func_8002AB40(0, 0, hp, 1, 0x20);
            d->rs->delta = 0.0;
            d->rs->first = 1;
        } else {
            d->rs = 0;
            j++;
            j++;
        }

        if (param[j]) {
            d->lp = func_8002AB40(0, 0, hp, 1, sizeof(ALLowPass));
            d->lp->fstate = func_8002AB40(0, 0, hp, 1, 8);
            d->lp->fc = param[j++];
            func_80028500(d->lp);
        } else {
            d->lp = 0;
            j++;
        }
    }
}
