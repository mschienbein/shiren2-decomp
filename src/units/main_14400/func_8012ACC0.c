#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/* Sequencer channel (0x13C bytes); D_801CA6DC holds D_801CA6D4 of them, the first four special. */
typedef struct {
    s32 flags00;
    u8 *active04;
    s32 mode08;
    s32 time0C;
    s32 countdown10;
    s32 due14;
    s32 due18;
    u8 pad1C[0x30 - 0x1C];
    float gain30;
    u8 *enable34;
    u8 *enable38;
    s32 next3C;
    s32 base40;
    u8 pad44[0x94 - 0x44];
    s32 due94;
    u8 pad98[2];
    u16 rate9A;
    u16 step9C;
    u8 pad9E[0xAA - 0x9E];
    u16 elapsedAA;
    u8 padAC[0xC3 - 0xAC];
    u8 envC3;
    u8 padC4[0xC9 - 0xC4];
    u8 voiceC9;
    u8 padCA[0xCE - 0xCA];
    u8 bendCE;
    u8 padCF[0xD4 - 0xCF];
    u8 timedD4;
    u8 vibratoD5;
    u8 padD6[0x13C - 0xD6];
} Entry;

/* Per-channel voice record (0x1C bytes); record i pairs with channel i + 4. */
typedef struct {
    u8 data[0x1C];
} Record;

extern void func_8012AB24(void);
extern void func_8012B36C(Entry *self, s32 index);
extern void func_8012AF30(Entry *self, s32 index);
extern void func_8012BA20(Entry *t);
extern void func_8012BADC(Entry *t);
/* D_801487D0 handlers accept and return the bytecode cursor. */
extern u8 *func_801291C0(Entry *arg0, u8 *command);
extern void func_80130320(Record *slot);
extern void func_8012B6B4(Entry *env);
extern void func_8012B8B8(Entry *obj);
extern float func_8012B9BC(Entry *self);
extern float func_8012B964(Entry *object);
extern void func_8012B52C(Entry *self, s32 index, float gain);
extern void func_8012B3E8(Entry *ch, s32 index);

extern s32 D_801CA6D4;
extern Record *D_801CA6D8;
extern Entry *D_801CA6DC;
extern s32 D_801CA6E8;

/* Per-frame tick returns the period; the scheduler supplies an unused client pointer. */
s32 func_8012ACC0(void *client)
{
    Entry *e;
    s32 i;
    float gain;

    func_8012AB24();
    e = D_801CA6DC;
    for (i = -4; i < D_801CA6D4 - 4; i++, e++) {
        if (e->active04 == 0) {
            continue;
        }
        if (e->flags00 & 1) {
            continue;
        }
        if (e->mode08 != 0) {
            func_8012B36C(e, i);
        }
        e->time0C += e->step9C;
        if (e->rate9A != 0x7FFF) {
            while (e->next3C - e->time0C < 0) {
                if (e->active04 == 0) {
                    break;
                }
                func_8012AF30(e, i);
            }
            if (e->active04 == 0) {
                continue;
            }
        }
        if (e->enable38 != 0 && e->due14 - e->time0C < 0) {
            func_8012BA20(e);
        }
        if (e->enable34 != 0 && e->due18 - e->time0C < 0) {
            func_8012BADC(e);
        }
        if (e->countdown10 != -1 && --e->countdown10 == -1) {
            e->active04 = func_801291C0(e, 0);
            if (e->voiceC9 != 0) {
                e->voiceC9 = 0;
                func_80130320(&D_801CA6D8[i]);
            }
        }
        if (e->voiceC9 != 0) {
            if (e->envC3 != 0) {
                func_8012B6B4(e);
            }
            if (e->timedD4 != 0 && e->due94 - e->time0C < 0) {
                func_8012B8B8(e);
            }
            gain = e->gain30;
            if (e->vibratoD5 != 0) {
                gain += func_8012B9BC(e);
            }
            if (e->bendCE != 0) {
                gain += func_8012B964(e);
            }
            if (e->mode08 == 0) {
                func_8012B52C(e, i, gain);
                func_8012B3E8(e, i);
            }
        }
        e->elapsedAA = (u32)(e->time0C - e->base40) >> 8;
    }
    return D_801CA6E8;
}
