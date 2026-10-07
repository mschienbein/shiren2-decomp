#include "common.h"

typedef unsigned char u8;

extern u8 D_00E53DB0[];
extern u8 D_00E7BE70[];
u32 func_80072440(u32 addr) {
    s32 mask = 0xFFFFFF;
    s32 seg = (s32)addr >> 24;

    seg &= 0xF;
    addr &= mask;

    switch (seg) {
    case 6:
        addr += (u32)D_00E7BE70;
        break;
    case 5:
        addr += (u32)D_00E53DB0;
        break;
    default:
        addr = 0;
        break;
    }
    return addr;
}
