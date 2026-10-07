#include "common.h"

typedef short s16;
typedef unsigned short u16;
extern u32 func_8009FF80(u16, u16);
extern u32 func_800C59B4(void *, u32);
extern s32 func_800C5B20(void *, u16);
extern s32 D_80147620[];

s16 func_800A0014(s16 x, u16 y) {
    u32 r;
    u32 d;

    if (x == 0) {
        return 0;
    }
    r = func_8009FF80(x < 0 ? -x : x, y);
    d = func_800C59B4(D_80147620, r >> 3);
    if (func_800C5B20(D_80147620, 2)) {
        r += d;
    } else {
        r -= d;
    }
    r >>= 16;
    if ((s16)r < 0) {
        r = 0x7FFF;
    }
    if (x < 0) {
        r = -r;
    }
    return r;
}
