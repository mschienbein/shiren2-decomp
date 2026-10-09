#include "common.h"

/* Reverb section setup: libultra alFxNew's parameter parsing (cf. func_800285A4),
 * drawing every delay-line, resampler and low-pass block from the tables that
 * func_8012CA10 preallocates instead of the heap. */

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef long long s64;
typedef float f32;

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

/* Effect bank of the synthesizer (see func_8012C9C0): the reverb filter at 0x24. */
typedef struct {
    u8 pad_00[0x24];
    ALFx *fx_24;
} FxBank8012CB48;

/* Synthesizer state (same object as D_80148D84); 0x40 holds the output rate. */
typedef struct {
    u8 pad_00[0x34];
    FxBank8012CB48 *bank_34;
    u8 pad_38[0x40 - 0x38];
    s32 outputRate_40;
} Synth8012CB48;

/* Six preallocated tables (element sizes 0x28, 2, 0x34, 0x20, 0x30, 8). */
typedef struct {
    void *table_28;
    void *table_02;
    void *table_34;
    void *table_20;
    void *table_30;
    void *table_08;
} Tables;

extern Synth8012CB48 *D_801DFF54;
extern Tables D_801DFF64;
void func_80028500(ALLowPass *lp);

/* `entry` is an s32 parameter list (D_80148A84 or the config entry). */
void func_8012CB48(void *entry) {
    ALFx *r = D_801DFF54->bank_34->fx_24;
    ALDelay *d;
    u16 i, j;

    j = 0;
    r->section_count = ((s32 *)entry)[j++];
    r->length = ((s32 *)entry)[j++];
    r->delay = D_801DFF64.table_28;
    r->base = D_801DFF64.table_02;
    r->input = r->base;

    for (i = 0; i < r->length; i++) {
        r->base[i] = 0;
    }

    for (i = 0; i < r->section_count; i++) {
        d = &r->delay[i];
        d->input = ((s32 *)entry)[j++];
        d->output = ((s32 *)entry)[j++];
        d->fbcoef = ((s32 *)entry)[j++];
        d->ffcoef = ((s32 *)entry)[j++];
        d->gain = ((s32 *)entry)[j++];

        if (((s32 *)entry)[j]) {
            d->rsinc = ((((f32)((s32 *)entry)[j++]) / 1000) * 2.0) / D_801DFF54->outputRate_40;
            d->rsgain = (((f32)((s32 *)entry)[j++]) / 173123.404906676) * (d->output - d->input);
            d->rsval = 1.0;
            d->rsdelta = 0.0;
            d->rs = D_801DFF64.table_34;
            d->rs->state = D_801DFF64.table_20;
            d->rs->delta = 0.0;
            d->rs->first = 1;
        } else {
            d->rs = 0;
            j++;
            j++;
        }

        if (((s32 *)entry)[j]) {
            d->lp = D_801DFF64.table_30;
            d->lp->fstate = D_801DFF64.table_08;
            d->lp->fc = ((s32 *)entry)[j++];
            func_80028500(d->lp);
        } else {
            d->lp = 0;
            j++;
        }
    }
}
