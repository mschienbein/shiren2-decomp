#include "common.h"
typedef unsigned char u8;
extern u8 D_80147620[];
u8 func_800C57A0(void *rng);
s32 func_800A4EFC(void *a, u8 *dir);
void func_800A2F80(u8 *dir, s32 step);
s32 func_800A50E8(void *a) {
    u8 dir[8];
    u8 *pdir;
    s32 i;
    dir[0] = func_800C57A0(D_80147620) & 7;
    for (i = 0, pdir = dir; i < 8; i++) {
        if (func_800A4EFC(a, pdir)) return 1;
        func_800A2F80(pdir, 1);
    }
    return 0;
}
