#include "common.h"

typedef unsigned char u8;

extern u8 D_8014313C[];
extern const u8 D_8015488C[8];

/* Returns 1 when any of the 40 flag bits in D_8014313C is clear. */
s32 func_800B0954(void)
{
    s32 bit = 0x28;
    s32 result;

    while (1) {
        if (bit-- <= 0) {
            result = 0;
            break;
        }
        if (!(D_8014313C[bit >> 3] & D_8015488C[bit & 7])) {
            result = 1;
            break;
        }
    }
    return result;
}
