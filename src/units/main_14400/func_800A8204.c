#include "common.h"

typedef unsigned short u16;

s32 func_800A8204(unsigned char *obj)
{
    s32 f = *(u16 *)(obj + 0x1C) & 0x20;

    return f != 0;
}
