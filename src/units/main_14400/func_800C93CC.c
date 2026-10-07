#include "common.h"

typedef unsigned short u16;

extern u16 D_8014767C;

s32 func_800C93CC(void)
{
    u16 flags = D_8014767C;
    s32 result = 0;

    if ((flags >> 7) & 1) {
        result = (flags >> 8) & 1;
    }
    return result;
}
