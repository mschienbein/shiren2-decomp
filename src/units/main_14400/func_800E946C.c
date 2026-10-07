#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

extern u8 D_80147620[];
extern u32 D_8013960C;
s32 func_800C5844(void *rng, u8 base, u8 top);
u16 func_800E08F0(void *obj);
u16 func_800E08B0(void *obj);
s32 func_800E07A8(void *obj, s32 amount);
s32 func_800E0534(void *obj, s32 amount);
void func_800E946C(void *obj, s16 count) {
    s32 total;
    s32 delta;

    if (count == 0) {
        return;
    }
    total = 0;
    delta = -1;
    if (count > 0) {
        delta = 1;
    }
    do {
        total += delta * (u8)func_800C5844(D_80147620, 3, 7);
        count -= delta;
    } while (count != 0);
    D_8013960C <<= 1;
    if (func_800E08F0(obj) + total < 15) {
        total = -func_800E08F0(obj) + 15;
    }
    delta = total;
    if (func_800E08B0(obj) + total <= 0) {
        delta = -func_800E08B0(obj) + 1;
    }
    if (total > 0) {
        func_800E07A8(obj, total);
        func_800E0534(obj, delta);
    } else {
        func_800E0534(obj, delta);
        func_800E07A8(obj, total);
    }
    D_8013960C >>= 1;
}
