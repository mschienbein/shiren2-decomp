#include "common.h"

typedef unsigned char u8;

s32 func_800B0FFC(u8 c) {
    if (c < 0xB4) return 0;
    return c < 0xBE;
}
