#include "common.h"

typedef unsigned char u8;

extern u8 D_801480D0[];

s32 func_800D818C(s32 value)
{
    s32 i;

    for (i = 1; i < 9; i++) {
        if (value < D_801480D0[i]) {
            break;
        }
    }
    return i - 1;
}
