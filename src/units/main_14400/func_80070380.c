#include "common.h"

typedef unsigned char u8;

s32 func_80070380(u8 *object)
{
    s32 size = 0x26;
    if (object[0x39] != 0)
        size = 0x48;
    return size;
}
