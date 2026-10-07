#include "common.h"

extern unsigned short D_801531F0[];

u32 func_8009FF80(unsigned short value, unsigned short mask) {
    u32 result = (u32)value << 16;
    s32 i;

    for (i = 0; i < 9; i++) {
        if (mask == 0) {
            return result;
        }
        if (mask & 1) {
            result >>= 16;
            result *= D_801531F0[i];
        }
        mask >>= 1;
    }
    if (mask != 0) {
        return 0;
    }
    return result;
}
