#include "common.h"

typedef unsigned char u8;

extern u8 D_801C35E0[];
extern u8 D_801C51A4[];
extern const u8 D_80154894[8];
u8 func_800A8C00(void *obj);

void func_800A89EC(void *obj) {
    u8 id;
    s32 bit;
    u8 *bits;

    if (obj != D_801C35E0) {
        id = func_800A8C00(obj);
        if (id != 0xFF) {
            bits = D_801C51A4;
            bit = id - 1;
            bits[bit >> 3] &= D_80154894[bit & 7];
        }
    }
}
