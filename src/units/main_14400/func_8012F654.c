/* libultra libaudio reverb.c alFxParamHdl (main-image copy). */
#include "common.h"

typedef signed short s16;
typedef unsigned char u8;
typedef float f32;

typedef struct ALFilter_s ALFilter;
typedef struct { u32 w0, w1; } AudioCommand;
typedef AudioCommand *(*ALCmdHandler)(void *, s16 *, s32, s32, AudioCommand *);
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
    ALFilter filter;
    s16 *base;
    s16 *input;
    u32 length;
    ALDelay *delay;
    u8 section_count;
    ALSetParam paramHdl;
} ALFx;

/* Audio player globals; outputRate is the driver output frequency. */
typedef struct {
    u8 pad0[0x40];
    s32 outputRate;
} ALGlobals;

extern ALGlobals *D_80148D84;
void func_80028500(ALLowPass *lp); /* _init_lpfilter */

#define RANGE 2.0
#define CONVERT 173123.404906676
#define LENGTH (f->delay[s].output - f->delay[s].input)

s32 func_8012F654(void *filter, s32 paramID, void *param)
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
            f->delay[s].rsinc = ((((f32)val) / 1000) * RANGE) / D_80148D84->outputRate;
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
