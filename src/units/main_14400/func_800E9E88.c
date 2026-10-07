#include "common.h"

typedef unsigned short u16;

extern u16 D_80156A10;
u32 func_800B1C6C(void *pos); u16 func_800E08F0(void *); void func_800A7B18(void *target, void *source, s32 amount, s32 kind);
s32 func_800E9E88(void *s) {
    u16 v;
    if (!(func_800B1C6C(s) & 0x4000)) return 0;
    v = func_800E08F0(s) * D_80156A10 / 100;
    if (v == 0) v = 1;
    func_800A7B18(s, (void *)0, (u16)v, 0x24);
    return 1;
}
