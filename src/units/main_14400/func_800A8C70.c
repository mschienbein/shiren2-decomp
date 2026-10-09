#include "common.h"

typedef unsigned char u8;

extern u8 D_801C51A4[];
extern const u8 D_8015488C[8];

s32 func_800A8C70(u8 id) {
    s32 bit;

    if (id == 0) {
        return 1;
    }
    bit = id - 1;
    return (D_801C51A4[bit >> 3] & D_8015488C[bit & 7]) != 0;
}
