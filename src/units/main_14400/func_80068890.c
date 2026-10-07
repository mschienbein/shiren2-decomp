#include "common.h"

typedef unsigned char u8;

extern s32 func_800625FC(s32 arg0, s32 arg1);

u32 func_80068890(u8 x, u8 y) {
    if (x >= 10 && x < 66 && y >= 10 && y < 44) {
        return (u32)func_800625FC(x, y);
    }
    return 0x4000;
}
