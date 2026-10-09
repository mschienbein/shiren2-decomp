#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

s32 func_800A3B10(u8 kind)
{
    return (u8)(kind - 2) < 0x15;
}
