#include "common.h"
typedef unsigned char u8;
extern u8 D_801480D0[];
extern u8 D_801480F4[];
extern s32 func_800D8088(u8 a, u8 b);
s32 func_800D8144(s32 index) {
    if (func_800D8088(0x29, D_801480D0[index])) return D_801480F4[index];
    return -1;
}
