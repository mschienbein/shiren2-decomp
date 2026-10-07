#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 value; } Dir;
typedef struct { s32 x; s32 y; } Pos800BE0A0;
typedef struct { u8 pad0[0x402]; u16 flags; } Obj800BE0A0;
/* Whole RNG object (state pointers plus backing words); only its base is passed here. */
extern u8 D_80147620[];
u8 func_800C57A0(void *rng);
void *func_800A2594(void *out, void *from, Dir dir);
s32 func_801368B4(Pos800BE0A0 *pos, s32 mask);
void func_800B1BE0(Pos800BE0A0 *pos, s32 mask);
void func_800B1B58(Pos800BE0A0 *pos, u16 flags);
void func_800BE0A0(Obj800BE0A0 *obj, Pos800BE0A0 *from) {
    Dir dir;
    s32 rnd;
    u8 i;

    if (func_800C57A0(D_80147620) & 1) {
        return;
    }
    rnd = func_800C57A0(D_80147620);
    for (i = 0; ; i++) {
        Pos800BE0A0 pos;
        Pos800BE0A0 *p;
        Dir *d;
        s32 ok;
        u8 value;
        if (i >= 8) {
            break;
        }
        value = ((rnd & 7) + i) % 8u;
        d = &dir;
        d->value = value;
        if (d->value % 2) {
            continue;
        }
        func_800A2594(&pos, from, dir);
        p = &pos;
        ok = 0;
        if (func_801368B4(p, 0x4000) != 0) {
            ok = func_801368B4(p, 0x8020) == 0;
        }
        if (ok) {
            func_800B1BE0(p, 0x4000);
            func_800B1B58(p, obj->flags | 0x200);
            return;
        }
    }
}
