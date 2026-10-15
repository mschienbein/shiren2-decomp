/* func_8008A198 (main_14400, ROM 0x5D1D8, 732 bytes).
 * Base source: opus-10 cand/func_8008A198.v1.c (wave17/a5 lineage; sibling func_80089BCC field views);
 * tail reworked by opus-09 (Anthropic, claude-opus-5-5, xhigh) in wave39. */
#include "common.h"

/* Sprite record from func_8007946C (field view shared with func_80089BCC). */
typedef struct {
    short field_00, field_02;
    unsigned char field_04[2];
    unsigned char field_06, field_07, field_08, field_09;
    unsigned char field_0a[2];
    short field_0c, field_0e, field_10;
    unsigned char field_12[2];
    float field_14;
    unsigned char field_18[0x24];
    unsigned short field_3c;
    unsigned char field_3e[8];
    unsigned char field_46;
    unsigned char field_47[3];
    short field_4a;
} Entity;

/* Effect task (field view shared with func_80089BCC). */
typedef struct {
    void (*field_00)(void *);
    short field_04;
    unsigned short field_06, field_08;
    unsigned char field_0a[4];
    short field_0e;
    unsigned char field_10[2];
    unsigned short field_12;
    s32 field_14, field_18;
    u32 field_1c;
    s32 field_20, field_24, field_28, field_2c;
    float field_30, field_34, field_38, field_3c, field_40, field_44, field_48, field_4c;
    float field_50, field_54, field_58;
    s32 field_5c, field_60, field_64, field_68, field_6c, field_70;
} Object;

extern Entity *func_8007946C(s32, s32);
extern s32 func_80076044(s32, s32, s32, s32, s32);
extern s32 func_800751B4(s32, s32);
extern void func_80079560(s32, s32, s32);
extern float func_80027C60(float);
extern float func_80032360(float);

void func_8008A198(Object *self) {
    Entity *entity = func_8007946C(0, self->field_14);
    u32 remaining;

    switch (self->field_08) {
    case 0: {
        s32 dx = self->field_5c - self->field_60;
        s32 dy = self->field_68 - self->field_6c;

        if (dx == 0 && dy == 0) {
            self->field_3c = self->field_48 = 0.0f;
        } else {
            self->field_3c = (float)(dx << 7);
            self->field_48 = (float)(dy << 7);
            if (dx < -18 || dx > 18 || dy > 18 || dy < -18) {
                self->field_3c *= 0.6f;
                self->field_48 *= 0.6f;
            }
        }
        func_80076044(self->field_14, entity->field_3c, 2, 0, 1);
        func_800751B4(self->field_14, 0x8000);
        entity->field_46 = 0xFE;
        self->field_1c = 0x17;
        self->field_30 = 0.0f;
        self->field_08++;
        break;
    }
    case 1:
        remaining = self->field_1c--;
        if (remaining != 0) {
            float x;
            float y;

            self->field_0e = 1;
            func_80079560(0, self->field_14, 0);
            entity->field_4a = (entity->field_4a + 1) % 8;
            entity->field_0c = (self->field_60 << 7) + 0x40;
            entity->field_10 = (self->field_6c << 7) + 0x40;
            x = entity->field_0c;
            entity->field_0c = x + self->field_3c * func_80027C60(self->field_30 / 180.0f * 3.14);
            y = entity->field_10;
            entity->field_10 = y + self->field_48 * -func_80032360(self->field_30 / 180.0f * 3.14);
            /* SCHEDULING BARRIER under the scoped user ruling
             * scratch/omp/rulings/DOWHILE0-BARRIER-SCOPED.md (func_8008A198 only).
             * The do { } while (0) below never breaks and has no semantic effect. It
             * is the only barrier in this function. GCC 2.8.1 emits
             * NOTE_INSN_LOOP_BEG/END for it, and its old sched.c (sched_analyze_insn,
             * loop_notes) treats loop notes as full scheduling barriers. Without one,
             * the backward list scheduler always emits the +0x30 store before the
             * +0x3C store (sched priorities 36 vs 38); the ROM stores +0x3C first
             * (0x8008A42C/0x8008A430). nx and nt hold the new values computed before
             * any store, and decay is the one constant used by both multiplies.
             * Measured ladder (tools/match.py, main_14400, gcc281pm-gnu291-O2-unsigned;
             * receipts in opus-09/result.json):
             *   barrier alone (before the +0x48 load, all stores inside):  8 words
             *   + named decay constant:                                    6 words
             *   + +0x30/+0x48 stores after the barrier (this form):        0 words
             *   best compliant form without a barrier:                     6 words
             *   (opus-10 v1, opus-09 k1-k4; the tools/permute.py beam rung
             *   found no improvement, per the ruling)
             * [INFERENCE] The original most plausibly used a multi-statement
             * do { ... } while (0) macro here; no such macro is recovered. */
            {
                float decay = 0.935f;
                float nx = self->field_3c * decay;
                float nt = self->field_30 + 24.0f;
                float nz;

                do {
                    nz = self->field_48 * decay;
                    self->field_3c = nx;
                } while (0);
                self->field_30 = nt;
                self->field_48 = nz;
            }
        } else {
            func_80079560(0, self->field_14, 1);
            self->field_04 = 4;
        }
        break;
    }
}
