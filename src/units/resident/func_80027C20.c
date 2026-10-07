#include "common.h"

typedef unsigned char u8;

void func_80027C20(u8 *src, u8 *dst, s32 len)
{
    s32 i;

    for (i = 0; i < len; i++) {
        *dst++ = *src++;
    }
}
