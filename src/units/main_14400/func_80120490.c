#include "common.h"

typedef unsigned char u8;

s32 func_80120490(u8 *obj, s32 kind)
{
    s32 result;

    if (kind == 0x1D) {
        return obj[0x28] != 0;
    }
    result = 0;
    if (kind == 0x11 || kind == 0xE || kind == 0x13 || kind == 0xB || kind == 0x16) {
        result = 1;
    }
    return result;
}
