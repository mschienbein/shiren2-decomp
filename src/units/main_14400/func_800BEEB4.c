#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s32 x; s32 y; } Pos800BEEB4;
typedef struct { u8 value; } Dir;
extern u8 D_80147620[];
u8 func_800C57A0(void *rng);
void *func_800A2594(void *out, void *from, Dir dir);
s32 func_801368B4(Pos800BEEB4 *pos, s32 mask);
void func_800B1B58(Pos800BEEB4 *pos, u16 flags);
void func_800BEEB4(s32 arg0, Pos800BEEB4 *pos) {
    s32 start;
    u8 i;
    Dir dir;
    Dir *dp;
    if (func_800C57A0(D_80147620) & 1) {
        return;
    }
    start = func_800C57A0(D_80147620);
    for (i = 0; ; i++) {
        Pos800BEEB4 next;
        Pos800BEEB4 *np;
        s32 ok;
        s32 v;
        if (i >= 8) {
            break;
        }
        v = (start & 7) + i;
        dp = &dir;
        dp->value = v & 7;
        if (dp->value % 2) {
            continue;
        }
        func_800A2594(&next, pos, dir);
        ok = 0;
        np = &next;
        if (func_801368B4(np, 0x4000)) {
            ok = !func_801368B4(np, 0x8020);
        }
        if (ok) {
            func_800B1B58(np, 0x20);
            return;
        }
    }
}
