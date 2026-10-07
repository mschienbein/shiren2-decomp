#include "common.h"
typedef unsigned char u8;
typedef struct { s32 a; s32 b; } Pos;
void *func_800A8CB0(s32 cell);
s32 func_800A8974(Pos *p);
void func_800B48C0(Pos *copy, Pos *p);
void func_800B2D50(void) {
    s32 i;
    for (i = 0; ; i++) {
        Pos *p;
        Pos copy;
        if (i >= 29) break;
        p = func_800A8CB0((u8)i);
        if (func_800A8974(p)) continue;
        copy = *p;
        func_800B48C0(&copy, p);
    }
}