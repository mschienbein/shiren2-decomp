#include "common.h"
s32 func_800CBDB0(s32 index, s32 count)
{
    s32 offset = (index & 0xFF) * 0x730 + 12;
    u32 original = count & 0xFF;
    if (original >= 11) count = 10;
    count &= 0xFF;
    offset += count * 144;
    if (original >= 11) {
        count = original - 10;
        offset += count * 20;
    }
    return offset;
}
