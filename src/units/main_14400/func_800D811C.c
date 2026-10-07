#include "common.h"

extern unsigned char D_801480D0[];
extern s32 func_800D8088(unsigned char kind, unsigned char level);
s32 func_800D811C(s32 index) { return func_800D8088(0x29,D_801480D0[index]); }
