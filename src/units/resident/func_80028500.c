#include "common.h"

/* libultra audio drvrNew.c: _init_lpfilter, alFxNew, alEnvmixerNew, alLoadNew, alResampleNew */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef long long s64;
typedef float f32;
typedef double f64;

/* Per-TU command view; callbacks use the same five-argument pull ABI. */
typedef union {
    struct { u32 w0; u32 w1; } words;
    s64 force_structure_alignment;
} Acmd;

typedef Acmd *(*ALCmdHandler)(void *, s16 *, s32, s32, Acmd *);
typedef s32 (*ALSetParam)(void *, s32, void *);

/* DMA consumes a sample-source pointer and returns an RSP physical address. */
typedef s32 (*ALDMAproc)(u8 *addr, s32 len, void *state);
typedef ALDMAproc (*ALDMANew)(void **state);

typedef struct ALFilter_s {
    struct ALFilter_s *source;
    ALCmdHandler handler;
    ALSetParam setParam;
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
    ALSetParam paramHdl;
} ALFx;

typedef struct {
    s32 maxVVoices;
    s32 maxPVoices;
    s32 maxUpdates;
    s32 maxFXbusses;
    ALDMANew dmaproc;
    void *heap;
    s32 outputRate;
    u8 fxType;
    s32 *params;
} ALSynConfig;

typedef struct {
    ALFilter filter;
    void *state;
    s16 pan;
    s16 volume;
    s16 cvolL;
    s16 cvolR;
    s16 dryamt;
    s16 wetamt;
    u16 lratl;
    s16 lratm;
    s16 ltgt;
    u16 rratl;
    s16 rratm;
    s16 rtgt;
    s32 delta;
    s32 segEnd;
    s32 first;
    void *ctrlList;
    void *ctrlTail;
    ALFilter **sources;
    s32 motion;
} ALEnvMixer;

typedef struct {
    ALFilter filter;
    void *state;
    void *lstate;
    u32 loopStart;
    u32 loopEnd;
    u32 loopCount;
    void *table;
    s32 bookSize;
    ALDMAproc dma;
    void *dmaState;
    s32 sample;
    s32 lastsam;
    s32 first;
    u8 *memin;
} ALLoadFilter;

extern void func_8002A980(void *f, ALCmdHandler pull, ALSetParam param, s32 type); /* alFilterNew */
extern void *func_8002AB40(char *file, s32 line, void *hp, s32 num, s32 size); /* alHeapDBAlloc */
extern Acmd *func_800302D0(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p); /* alFxPull */
extern s32 func_80030644(void *filter, s32 paramID, void *param); /* alFxParam */
extern s32 func_80030658(void *filter, s32 paramID, void *param); /* alFxParamHdl */
extern Acmd *func_80028D30(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p); /* alEnvmixerPull */
extern s32 func_80029230(void *filter, s32 paramID, void *param); /* alEnvmixerParam */
extern Acmd *func_8002B430(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p); /* alAdpcmPull */
extern s32 func_8002BC10(void *filter, s32 paramID, void *param); /* alLoadParam */
extern Acmd *func_8002FFD0(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p); /* alResamplePull */
extern s32 func_800301BC(void *filter, s32 paramID, void *param); /* alResampleParam */

#define alHeapAlloc(hp, elem, size) func_8002AB40(0, 0, (hp), (elem), (size))

#define SCALE 16384
#define ms *(((s32)((f32)44.1)) & ~0x7)

static s32 SMALLROOM_PARAMS[26] = {
    3, 100 ms,
    0, 54 ms, 9830, -9830, 0, 0, 0, 0,
    19 ms, 38 ms, 3276, -3276, 0x3fff, 0, 0, 0,
    0, 60 ms, 5000, 0, 0, 0, 0, 0x5000
};

static s32 BIGROOM_PARAMS[34] = {
    4, 100 ms,
    0, 66 ms, 9830, -9830, 0, 0, 0, 0,
    22 ms, 54 ms, 3276, -3276, 0x3fff, 0, 0, 0,
    66 ms, 91 ms, 3276, -3276, 0x3fff, 0, 0, 0,
    0, 94 ms, 8000, 0, 0, 0, 0, 0x5000
};

static s32 ECHO_PARAMS[10] = {
    1, 200 ms,
    0, 179 ms, 12000, 0, 0x7fff, 0, 0, 0
};

static s32 CHORUS_PARAMS[10] = {
    1, 20 ms,
    0, 5 ms, 0x4000, 0, 0x7fff, 7600, 700, 0
};

static s32 FLANGE_PARAMS[10] = {
    1, 20 ms,
    0, 5 ms, 0, 0x5fff, 0x7fff, 380, 500, 0
};

static s32 NULL_PARAMS[10] = {
    0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

void func_80028500(ALLowPass *lp)
{
    s32 i, temp;
    s16 fc;
    f64 ffc, fcoef;

    temp = lp->fc * SCALE;
    fc = temp >> 15;
    lp->fgain = SCALE - fc;

    lp->first = 1;
    for (i = 0; i < 8; i++) {
        lp->fcvec.fccoef[i] = 0;
    }

    lp->fcvec.fccoef[i++] = fc;
    fcoef = ffc = (f64)fc / SCALE;

    for (; i < 16; i++) {
        fcoef *= ffc;
        lp->fcvec.fccoef[i] = (s16)(fcoef * SCALE);
    }
}

void func_800285A4(ALFx *r, ALSynConfig *c, void *hp)
{
    u16 i, j, k;
    s32 *param = 0;
    ALFilter *f = (ALFilter *)r;
    ALDelay *d;

    func_8002A980(f, 0, func_80030644, 5);
    f->handler = func_800302D0;
    r->paramHdl = func_80030658;

    switch (c->fxType) {
        case 1: param = SMALLROOM_PARAMS; break;
        case 2: param = BIGROOM_PARAMS; break;
        case 5: param = ECHO_PARAMS; break;
        case 3: param = CHORUS_PARAMS; break;
        case 4: param = FLANGE_PARAMS; break;
        case 6: param = c->params; break;
        default: param = NULL_PARAMS; break;
    }

    j = 0;

    r->section_count = param[j++];
    r->length = param[j++];

    r->delay = alHeapAlloc(hp, r->section_count, sizeof(ALDelay));
    r->base = alHeapAlloc(hp, r->length, sizeof(s16));
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
            d->rs = alHeapAlloc(hp, 1, sizeof(ALResampler));
            d->rs->state = alHeapAlloc(hp, 1, 0x20);
            d->rs->delta = 0.0;
            d->rs->first = 1;
        } else {
            d->rs = 0;
            j++;
            j++;
        }

        if (param[j]) {
            d->lp = alHeapAlloc(hp, 1, sizeof(ALLowPass));
            d->lp->fstate = alHeapAlloc(hp, 1, 8);
            d->lp->fc = param[j++];
            func_80028500(d->lp);
        } else {
            d->lp = 0;
            j++;
        }
    }
}

void func_80028A3C(ALEnvMixer *e, void *hp)
{
    func_8002A980(e, func_80028D30, func_80029230, 4);

    e->state = alHeapAlloc(hp, 1, 0x50);

    e->first = 1;
    e->motion = 0;
    e->volume = 1;
    e->ltgt = 1;
    e->rtgt = 1;
    e->cvolL = 1;
    e->cvolR = 1;
    e->dryamt = 0;
    e->wetamt = 0;
    e->lratm = 1;
    e->lratl = 0;
    e->lratm = 1;
    e->lratl = 0;
    e->delta = 0;
    e->segEnd = 0;
    e->pan = 0;
    e->ctrlList = 0;
    e->ctrlTail = 0;
    e->sources = 0;
}

void func_80028AF4(ALLoadFilter *f, ALDMANew dmaNew, void *hp)
{
    s32 i;

    func_8002A980(f, func_8002B430, func_8002BC10, 0);

    f->state = alHeapAlloc(hp, 1, 0x20);
    f->lstate = alHeapAlloc(hp, 1, 0x20);

    f->dma = dmaNew(&f->dmaState);

    f->lastsam = 0;
    f->first = 1;
    f->memin = 0;
}

void func_80028BA4(ALResampler *r, void *hp)
{
    func_8002A980(r, func_8002FFD0, func_800301BC, 1);

    r->state = alHeapAlloc(hp, 1, 0x20);
    r->delta = 0.0;
    r->first = 1;
    r->motion = 0;
    r->ratio = 1.0;
    r->upitch = 0;
    r->ctrlList = 0;
    r->ctrlTail = 0;
}
