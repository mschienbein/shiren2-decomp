#include "common.h"

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef float f32;

typedef struct {
    u32 w0;
    u32 w1;
} Acmd;

/* Queued parameter update (libultra ALParam and its typed variants). */
typedef struct ALParam {
    struct ALParam *next;
    s32 delta;
    s16 type;
    s16 unity;      /* ALStartParam(Alt) */
    union {
        f32 f;
        s32 i;
        void *p;
    } data;         /* 0x0C: pitch, wave or free-voice pointer */
    union {
        f32 f;
        s32 i;
    } moredata;     /* 0x10 */
} ALParam;

typedef struct {
    struct ALParam *next;
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
    struct ALParam *next;
    s32 delta;
    s16 type;
    s16 unity;
    void *wave;
} ALStartParam;

/* Physical voice handed back by AL_FILTER_FREE_VOICE; 0x88 is its update offset. */
typedef struct {
    u8 pad0[0x88];
    s32 offset;
} PVoice8012E040;

typedef struct {
    struct ALParam *next;
    s32 delta;
    s16 type;
    PVoice8012E040 *pvoice;
} ALFreeParam;

/* Envelope-mixer view of the voice (same layout as func_8012E698's). */
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
    ALParam *ctrlList; /* 0x7C */
    ALParam *ctrlTail; /* 0x80 */
    s32 motion;       /* 0x84 */
} Voice8012E040;

/* Parameter types handled here (libultra AL_FILTER_*). */
#define AL_FILTER_FREE_VOICE 0
#define AL_FILTER_RESET 4
#define AL_FILTER_SET_WAVETABLE 5
#define AL_FILTER_SET_PITCH 7
#define AL_FILTER_SET_UNITY_PITCH 8
#define AL_FILTER_SET_VOLUME 11
#define AL_FILTER_SET_PAN 12
#define AL_FILTER_START_VOICE_ALT 13
#define AL_FILTER_START_VOICE 14
#define AL_FILTER_STOP 15
#define AL_FILTER_SET_FXAMT 16
#define AL_PAN_CENTER 64
#define AL_PAN_RIGHT 127
#define AL_PLAYING 1
/* Updates are applied on whole 184-sample frames (rounded to the nearest frame). */
#define FIXED_SAMPLE 184

extern s16 D_80148C60[]; /* eqpower[128] */
extern u8 D_80148D60;    /* stereo output enabled */
Acmd *func_8012E698(void *filter, s16 *inp, s16 *outp, s32 outCount, Acmd *p);
s16 func_8012EA08(s16 initial, s32 step, s16 integral, u16 fractional);
void func_8012EEA4(Voice8012E040 *object, s32 action, void *data);
s32 func_8012E5F8(Voice8012E040 *p, s32 msg, ALParam *arg);
void func_80130824(void *a);
void func_801307AC(ALParam *node);

/* n_alEnvmixerPull: apply the voice's queued parameter updates at frame granularity,
 * pulling the samples between them, then pull the rest of the 184-sample frame. */
Acmd *func_8012E040(Voice8012E040 *filter, s32 sampleOffset, Acmd *p) {
    Acmd *ptr = p;
    Voice8012E040 *e = filter;
    s16 inp;
    s32 lastOffset;
    s32 thisOffset = sampleOffset;
    s32 samples;
    s16 loutp = 0;
    s32 fVol;
    ALParam *thisParam;
    s32 outCount = FIXED_SAMPLE;

    inp = 0;

    while (e->ctrlList != 0) {
        lastOffset = thisOffset;
        thisOffset = e->ctrlList->delta;
        samples = (thisOffset - lastOffset + FIXED_SAMPLE / 2) / FIXED_SAMPLE * FIXED_SAMPLE;
        if (samples == 0) {
            thisOffset = lastOffset;
        }
        if (samples > outCount) {
            break;
        }

        switch (e->ctrlList->type) {
            case AL_FILTER_START_VOICE_ALT: {
                ALStartParamAlt *param = (ALStartParamAlt *)e->ctrlList;
                s32 tmp;

                if (param->unity) {
                    e->upitch = 1;
                }
                func_8012EEA4(e, AL_FILTER_SET_WAVETABLE, param->wave);
                e->motion = AL_PLAYING;
                e->first = 1;
                e->delta = 0;
                e->segEnd = (param->samples + FIXED_SAMPLE / 2) / FIXED_SAMPLE * FIXED_SAMPLE;

                tmp = ((s32)param->volume * (s32)param->volume) >> 15;
                e->volume = (s16)tmp;
                if (D_80148D60 == 1) {
                    e->pan = param->pan;
                } else {
                    e->pan = AL_PAN_CENTER;
                }
                e->dryamt = D_80148C60[param->fxMix];
                e->wetamt = D_80148C60[AL_PAN_RIGHT - param->fxMix];

                if (param->samples) {
                    e->cvolL = 1;
                    e->cvolR = 1;
                } else {
                    e->cvolL = (e->volume * D_80148C60[e->pan]) >> 15;
                    e->cvolR = (e->volume * D_80148C60[AL_PAN_RIGHT - e->pan]) >> 15;
                }
                e->ratio = param->pitch;
                break;
            }

            case AL_FILTER_SET_FXAMT:
            case AL_FILTER_SET_PAN:
            case AL_FILTER_SET_VOLUME:
                ptr = func_8012E698(e, &inp, &loutp, samples, ptr);

                if (e->delta >= e->segEnd) {
                    e->ltgt = (e->volume * D_80148C60[e->pan]) >> 15;
                    e->rtgt = (e->volume * D_80148C60[AL_PAN_RIGHT - e->pan]) >> 15;
                    e->delta = e->segEnd;
                    e->cvolL = e->ltgt;
                    e->cvolR = e->rtgt;
                } else {
                    e->cvolL = func_8012EA08(e->cvolL, e->delta, e->lratm, e->lratl);
                    e->cvolR = func_8012EA08(e->cvolR, e->delta, e->rratm, e->rratl);
                }
                if (e->cvolL == 0) {
                    e->cvolL = 1;
                }
                if (e->cvolR == 0) {
                    e->cvolR = 1;
                }
                if (e->ctrlList->type == AL_FILTER_SET_PAN) {
                    if (D_80148D60 == 1) {
                        e->pan = (s16)e->ctrlList->data.i;
                    } else {
                        e->pan = AL_PAN_CENTER;
                    }
                }
                if (e->ctrlList->type == AL_FILTER_SET_VOLUME) {
                    ALParam *param;

                    e->delta = 0;
                    param = e->ctrlList;
                    fVol = param->data.i;
                    fVol = (fVol * fVol) >> 15;
                    e->volume = fVol;
                    e->segEnd = (param->moredata.i + FIXED_SAMPLE / 2) / FIXED_SAMPLE * FIXED_SAMPLE;
                }
                if (e->ctrlList->type == AL_FILTER_SET_FXAMT) {
                    e->dryamt = D_80148C60[e->ctrlList->data.i];
                    e->wetamt = D_80148C60[AL_PAN_RIGHT - e->ctrlList->data.i];
                }
                e->first = 1;
                break;

            case AL_FILTER_START_VOICE: {
                ALStartParam *param = (ALStartParam *)e->ctrlList;

                if (param->unity) {
                    e->upitch = 1;
                }
                func_8012EEA4(e, AL_FILTER_SET_WAVETABLE, param->wave);
                e->motion = AL_PLAYING;
                break;
            }

            case AL_FILTER_STOP:
                ptr = func_8012E698(e, &inp, &loutp, samples, ptr);
                func_8012E5F8(e, AL_FILTER_RESET, 0);
                break;

            case AL_FILTER_FREE_VOICE: {
                ALFreeParam *param = (ALFreeParam *)e->ctrlList;

                param->pvoice->offset = 0;
                func_80130824(param->pvoice);
                break;
            }

            case AL_FILTER_SET_PITCH:
                ptr = func_8012E698(e, &inp, &loutp, samples, ptr);
                e->ratio = e->ctrlList->data.f;
                break;

            case AL_FILTER_SET_UNITY_PITCH:
                ptr = func_8012E698(e, &inp, &loutp, samples, ptr);
                e->upitch = 1;
                break;

            case AL_FILTER_SET_WAVETABLE:
                ptr = func_8012E698(e, &inp, &loutp, samples, ptr);
                func_8012EEA4(e, AL_FILTER_SET_WAVETABLE, e->ctrlList->data.p);
                break;

            default:
                ptr = func_8012E698(e, &inp, &loutp, samples, ptr);
                func_8012E5F8(e, e->ctrlList->type, e->ctrlList->data.p);
                break;
        }

        loutp += samples << 1;
        outCount -= samples;

        thisParam = e->ctrlList;
        e->ctrlList = e->ctrlList->next;
        if (e->ctrlList == 0) {
            e->ctrlTail = 0;
        }
        func_801307AC(thisParam);
    }

    ptr = func_8012E698(e, &inp, &loutp, outCount, ptr);

    if (e->delta > e->segEnd) {
        e->delta = e->segEnd;
    }
    return ptr;
}
