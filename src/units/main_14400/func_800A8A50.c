#include "common.h"

typedef unsigned char u8;

extern u8 D_801C51A4[];
extern u8 D_8015488C[];

s32 func_800A8A50(void) {
    s32 count = 0;
    s32 i = 0;
    u8 *flags = D_801C51A4;

    for (;;) {
        u8 bits;

        if (i >= 29) {
            break;
        }
        bits = *(u8 *)((i >> 3) + (s32)flags);
        if (!(bits & D_8015488C[i & 7])) {
            count++;
        }
        i++;
    }
    return count;
}
