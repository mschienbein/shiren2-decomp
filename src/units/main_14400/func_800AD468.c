#include "common.h"

typedef unsigned char u8;

extern u8 D_80143044[];

s32 func_800AD468(u32 bit)
{
    bit &= 0xFF;
    return (D_80143044[bit >> 3] >> (bit & 7)) & 1;
}
