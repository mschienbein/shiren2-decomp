#include "common.h"

/* libultra audio env.c: alEnvmixerPull */

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef float f32;
typedef double f64;
typedef long long Acmd;

typedef struct ALFilter_s {
    struct ALFilter_s *source;
    void *handler;
    s32 (*setParam)(void *, s32, void *);
    s32 inp;
    s32 outp;
} ALFilter;

typedef struct ALParam_s {
    struct ALParam_s *next;
    s32 delta;
    s16 type;
    union {
        f32 f;
        s32 i;
    } data;
    union {
        f32 f;
        s32 i;
    } moredata;
} ALParam;

typedef struct {
    struct ALParam_s *next;
    s32 delta;
    s16 type;
    s16 unity;
    void *wave;
} ALStartParam;

typedef struct {
    struct ALParam_s *next;
    s32 delta;
    s16 type;
    s16 unity;
    f32 pitch;
    s16 volume;
    u8 pan;
    u8 fxMix;
    s32 samples;
    void *wave;
} ALStartParamAlt;

typedef struct {
    u8 pad[0xD8];
    s32 offset;
} ALVoice;

typedef struct {
    struct ALParam_s *next;
    s32 delta;
    s16 type;
    ALVoice *pvoice;
} ALFreeParam;

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
    ALParam *ctrlList;
    ALParam *ctrlTail;
    ALFilter **sources;
    s32 motion;
} ALEnvMixer;

typedef struct {
    u8 drvr[1];
} ALGlobals;

extern ALGlobals *D_80037320; /* alGlobals */

extern Acmd *func_80029304(void *filter, s16 *inp, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p); /* _pullSubFrame */
extern f32 func_8002995C(f32 ivol, s32 samples, s16 ratem, u16 ratel); /* _getVol */
extern void func_80033594(void *drvr, ALVoice *pvoice);                  /* _freePVoice */
extern void func_80033528(ALParam *param);                               /* __freeParam */

#define EQPOWER_LENGTH 128
static s16 eqpower[EQPOWER_LENGTH] = {
    32767, 32764, 32757, 32744, 32727, 32704,
    32677, 32644, 32607, 32564, 32517, 32464,
    32407, 32344, 32277, 32205, 32127, 32045,
    31958, 31866, 31770, 31668, 31561, 31450,
    31334, 31213, 31087, 30957, 30822, 30682,
    30537, 30388, 30234, 30075, 29912, 29744,
    29572, 29395, 29214, 29028, 28838, 28643,
    28444, 28241, 28033, 27821, 27605, 27385,
    27160, 26931, 26698, 26461, 26220, 25975,
    25726, 25473, 25216, 24956, 24691, 24423,
    24151, 23875, 23596, 23313, 23026, 22736,
    22442, 22145, 21845, 21541, 21234, 20924,
    20610, 20294, 19974, 19651, 19325, 18997,
    18665, 18331, 17993, 17653, 17310, 16965,
    16617, 16266, 15913, 15558, 15200, 14840,
    14477, 14113, 13746, 13377, 13006, 12633,
    12258, 11881, 11503, 11122, 10740, 10357,
    9971, 9584, 9196, 8806, 8415, 8023,
    7630, 7235, 6839, 6442, 6044, 5646,
    5246, 4845, 4444, 4042, 3640, 3237,
    2833, 2429, 2025, 1620, 1216, 810,
    405, 0
};

Acmd *func_80028D30(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p)
{
    Acmd *ptr = p;
    ALEnvMixer *e = (ALEnvMixer *)filter;
    s16 inp;
    s32 lastOffset;
    s32 thisOffset = sampleOffset;
    s32 samples;
    s16 loutp = 0;
    s32 fVol;
    ALParam *thisParam;

    inp = 0;

    while (e->ctrlList != 0) {
        lastOffset = thisOffset;
        thisOffset = e->ctrlList->delta;
        samples = thisOffset - lastOffset;
        if (samples > outCount) {
            break;
        }

        switch (e->ctrlList->type) {
            case 13: {
                ALStartParamAlt *param = (ALStartParamAlt *)e->ctrlList;
                ALFilter *f = (ALFilter *)e;
                s32 tmp;

                if (param->unity) {
                    (*e->filter.setParam)(&e->filter, 8, 0);
                }

                (*e->filter.setParam)(&e->filter, 5, param->wave);
                (*e->filter.setParam)(&e->filter, 9, 0);

                e->first = 1;

                e->delta = 0;
                e->segEnd = param->samples;

                tmp = ((s32)param->volume * (s32)param->volume) >> 15;
                e->volume = (s16)tmp;
                e->pan = param->pan;
                e->dryamt = eqpower[param->fxMix];
                e->wetamt = eqpower[EQPOWER_LENGTH - param->fxMix - 1];

                if (param->samples) {
                    e->cvolL = 1;
                    e->cvolR = 1;
                } else {
                    e->cvolL = (e->volume * eqpower[e->pan]) >> 15;
                    e->cvolR = (e->volume * eqpower[EQPOWER_LENGTH - e->pan - 1]) >> 15;
                }

                if (f->source) {
                    union {
                        f32 f;
                        s32 i;
                    } data;
                    data.f = param->pitch;
                    (*f->source->setParam)(f->source, 7, (void *)data.i);
                }
            } break;

            case 16:
            case 12:
            case 11:
                ptr = func_80029304(e, &inp, &loutp, samples, sampleOffset, ptr);

                if (e->delta >= e->segEnd) {
                    e->ltgt = (e->volume * eqpower[e->pan]) >> 15;
                    e->rtgt = (e->volume * eqpower[EQPOWER_LENGTH - e->pan - 1]) >> 15;
                    e->delta = e->segEnd;
                    e->cvolL = e->ltgt;
                    e->cvolR = e->rtgt;
                } else {
                    e->cvolL = func_8002995C(e->cvolL, e->delta, e->lratm, e->lratl);
                    e->cvolR = func_8002995C(e->cvolR, e->delta, e->rratm, e->rratl);
                }

                if (e->cvolL == 0) {
                    e->cvolL = 1;
                }
                if (e->cvolR == 0) {
                    e->cvolR = 1;
                }

                if (e->ctrlList->type == 12) {
                    e->pan = (s16)e->ctrlList->data.i;
                }

                if (e->ctrlList->type == 11) {
                    e->delta = 0;
                    fVol = (e->ctrlList->data.i);
                    fVol = (fVol * fVol) >> 15;
                    e->volume = (s16)fVol;
                    e->segEnd = e->ctrlList->moredata.i;
                }

                if (e->ctrlList->type == 16) {
                    e->dryamt = eqpower[e->ctrlList->data.i];
                    e->wetamt = eqpower[EQPOWER_LENGTH - e->ctrlList->data.i - 1];
                }

                e->first = 1;
                break;

            case 14: {
                ALStartParam *p = (ALStartParam *)e->ctrlList;

                if (p->unity) {
                    (*e->filter.setParam)(&e->filter, 8, 0);
                }

                (*e->filter.setParam)(&e->filter, 5, p->wave);
                (*e->filter.setParam)(&e->filter, 9, 0);
            } break;

            case 15:
                ptr = func_80029304(e, &inp, &loutp, samples, sampleOffset, ptr);
                (*e->filter.setParam)(&e->filter, 4, 0);
                break;

            case 0: {
                void *drvr = &D_80037320->drvr;
                ALFreeParam *param = (ALFreeParam *)e->ctrlList;
                param->pvoice->offset = 0;
                func_80033594(drvr, param->pvoice);
            } break;

            default:
                ptr = func_80029304(e, &inp, &loutp, samples, sampleOffset, ptr);
                (*e->filter.setParam)(&e->filter, e->ctrlList->type, (void *)e->ctrlList->data.i);
                break;
        }
        loutp += (samples << 1);
        outCount -= samples;

        thisParam = e->ctrlList;
        e->ctrlList = e->ctrlList->next;
        if (e->ctrlList == 0) {
            e->ctrlTail = 0;
        }

        func_80033528(thisParam);
    }

    ptr = func_80029304(e, &inp, &loutp, outCount, sampleOffset, ptr);

    if (e->delta > e->segEnd) {
        e->delta = e->segEnd;
    }

    return ptr;
}

typedef struct {
    u32 w0;
    u32 w1;
} AcmdWords;

typedef Acmd *(*ALCmdHandler)(void *, s16 *, s32, s32, Acmd *);

#define AL_FILTER_SET_SOURCE 1
#define AL_FILTER_ADD_UPDATE 3
#define AL_FILTER_RESET 4
#define AL_FILTER_START 9

#define AL_STOPPED 0
#define AL_PLAYING 1

#define AL_MAIN_L_OUT 0x440
#define AL_MAIN_R_OUT 0x580
#define AL_AUX_L_OUT 0x6C0
#define AL_AUX_R_OUT 0x800


#define A_ENVMIXER 3
#define A_SETBUFF 8
#define A_SETVOL 9

#define A_INIT 0x01
#define A_CONTINUE 0x00
#define A_RIGHT 0x00
#define A_LEFT 0x02
#define A_RATE 0x00
#define A_VOL 0x04
#define A_AUX 0x08

#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((0x01 << (w)) - 1)) << (s)))

#define aSetBuffer(pkt, f, i, o, c)                                                         \
    {                                                                                       \
        AcmdWords *_a = (AcmdWords *)pkt;                                                             \
        _a->w0 = (_SHIFTL(A_SETBUFF, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(i, 0, 16));       \
        _a->w1 = (_SHIFTL(o, 16, 16) | _SHIFTL(c, 0, 16));                                  \
    }

#define aSetVolume(pkt, f, v, t, r)                                                         \
    {                                                                                       \
        AcmdWords *_a = (AcmdWords *)pkt;                                                             \
        _a->w0 = (_SHIFTL(A_SETVOL, 24, 8) | _SHIFTL(f, 16, 16) | _SHIFTL(v, 0, 16));       \
        _a->w1 = _SHIFTL(t, 16, 16) | _SHIFTL(r, 0, 16);                                    \
    }

#define aEnvMixer(pkt, f, s)                                                                \
    {                                                                                       \
        AcmdWords *_a = (AcmdWords *)pkt;                                                             \
        _a->w0 = (_SHIFTL(A_ENVMIXER, 24, 8) | _SHIFTL(f, 16, 8));                          \
        _a->w1 = (u32)(s);                                                                  \
    }

extern u32 func_800340F0(void *addr); /* osVirtualToPhysical */

s16 func_800296DC(f64 vol, f64 tgt, s32 count, u16 *ratel);

/* alEnvmixerParam */
s32 func_80029230(void *filter, s32 paramID, void *param)
{
    ALFilter *f = (ALFilter *)filter;
    ALEnvMixer *e = (ALEnvMixer *)filter;

    switch (paramID) {
        case AL_FILTER_ADD_UPDATE:
            if (e->ctrlTail) {
                e->ctrlTail->next = (ALParam *)param;
            } else {
                e->ctrlList = (ALParam *)param;
            }
            e->ctrlTail = (ALParam *)param;
            break;
        case AL_FILTER_RESET:
            e->first = 1;
            e->motion = AL_STOPPED;
            e->volume = 1;
            if (f->source) {
                (*f->source->setParam)(f->source, AL_FILTER_RESET, param);
            }
            break;
        case AL_FILTER_START:
            e->motion = AL_PLAYING;
            if (f->source) {
                (*f->source->setParam)(f->source, AL_FILTER_START, param);
            }
            break;
        case AL_FILTER_SET_SOURCE:
            f->source = (ALFilter *)param;
            break;
        default:
            if (f->source) {
                (*f->source->setParam)(f->source, paramID, param);
            }
            break;
    }
    return 0;
}

/* _pullSubFrame */
Acmd *func_80029304(void *filter, s16 *inp, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p)
{
    Acmd *ptr = p;
    ALEnvMixer *e = (ALEnvMixer *)filter;
    ALFilter *source = e->filter.source;

    if ((e->motion != AL_PLAYING) || (outCount == 0)) {
        return ptr;
    }

    ptr = (*(ALCmdHandler)source->handler)(source, inp, outCount, sampleOffset, p);

    aSetBuffer(ptr++, 0, *inp, AL_MAIN_L_OUT + *outp, outCount << 1);
    aSetBuffer(ptr++, A_AUX, AL_MAIN_R_OUT + *outp, AL_AUX_L_OUT + *outp, AL_AUX_R_OUT + *outp);

    if (e->first) {
        e->first = 0;
        e->ltgt = (e->volume * eqpower[e->pan]) >> 15;
        e->lratm = func_800296DC((f64)e->cvolL, (f64)e->ltgt, e->segEnd, &e->lratl);
        e->rtgt = (e->volume * eqpower[EQPOWER_LENGTH - e->pan - 1]) >> 15;
        e->rratm = func_800296DC((f64)e->cvolR, (f64)e->rtgt, e->segEnd, &e->rratl);

        aSetVolume(ptr++, A_VOL | A_LEFT, e->cvolL, 0, 0);
        aSetVolume(ptr++, A_VOL | A_RIGHT, e->cvolR, 0, 0);
        aSetVolume(ptr++, A_RATE | A_LEFT, e->ltgt, e->lratm, e->lratl);
        aSetVolume(ptr++, A_RATE | A_RIGHT, e->rtgt, e->rratm, e->rratl);
        aSetVolume(ptr++, A_AUX, e->dryamt, 0, e->wetamt);
        aEnvMixer(ptr++, A_INIT | A_AUX, func_800340F0(e->state));
    } else {
        aEnvMixer(ptr++, A_CONTINUE | A_AUX, func_800340F0(e->state));
    }

    *inp += (outCount << 1);
    e->delta += outCount;

    return ptr;
}

/* _frexpf */
f64 func_800295D8(f64 value, s32 *eptr)
{
    f64 absvalue;

    *eptr = 0;
    if (value == 0.0) {
        return value;
    }
    absvalue = (value > 0.0) ? value : -value;
    for (; absvalue >= 1.0; absvalue *= 0.5) {
        ++*eptr;
    }
    for (; absvalue < 0.5; absvalue += absvalue) {
        --*eptr;
    }
    return (value > 0.0) ? absvalue : -absvalue;
}

/* _ldexpf */
f64 func_800296B8(f64 in, s32 ex)
{
    s32 exp;

    if (ex) {
        exp = 1 << ex;
        in *= (f64)exp;
    }
    return in;
}

/* _getRate */
s16 func_800296DC(f64 vol, f64 tgt, s32 count, u16 *ratel)
{
    s16 s;
    f64 invn = 1.0 / count;
    f64 eps;
    f64 a;
    f64 fs;
    f64 mant;
    s32 i_invn;
    s32 ex;
    s32 indx;

    if (count == 0) {
        if (tgt >= vol) {
            *ratel = 0xFFFF;
            return 0x7FFF;
        } else {
            *ratel = 0;
            return 0;
        }
    }

    if (tgt < 1.0) {
        tgt = 1.0;
    }
    if (vol <= 0) {
        vol = 1;
    }

    {
    f64 logtab[8] = {
        -0.912537, -0.752072, -0.607683, -0.476438,
        -0.356144, -0.245112, -0.142019, -0.045804,
    };

    a = tgt / vol;
    i_invn = (s32)(invn * 1073741824.0);
    mant = func_800295D8(a, &ex);
    indx = (s32)(mant * 16);
    eps = (logtab[indx - 8] + ex) * 0.6931471805599453 / 1073741824.0 + 1.0;

    fs = 1.0;
    for (; i_invn; i_invn >>= 1) {
        if (i_invn & 1) {
            fs *= eps;
        }
        eps *= eps;
    }
    fs *= fs;
    fs *= fs;
    fs *= fs;

    s = (s16)fs;
    *ratel = (s32)((fs - (f32)s) * 65535.0);
    return s;
    }
}

/* _getVol */
f32 func_8002995C(f32 ivol, s32 samples, s16 ratem, u16 ratel)
{
    f32 r;
    f32 a;
    s32 i;

    samples >>= 3;
    if (samples == 0) {
        return ivol;
    }
    r = ((f32)(ratem << 16) + (f32)ratel) / 65536;
    a = 1.0f;
    for (i = 0; i < 32; i++) {
        if (samples & 1) {
            a *= r;
        }
        samples >>= 1;
        if (samples == 0) {
            break;
        }
        r *= r;
    }
    ivol *= a;
    return ivol;
}
