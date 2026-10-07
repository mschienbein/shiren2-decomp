#include "common.h"

/* osAiSetFrequency */

extern s32 D_80037258; /* osViClock */

s32 func_80025EE0(u32 frequency)
{
    register unsigned int dacRate;
    register unsigned char bitRate;
    register float f;

    f = D_80037258 / (float)frequency + .5f;
    dacRate = f;

    if (dacRate < 132) {
        return -1;
    }

    bitRate = dacRate / 66;
    if (bitRate > 16) {
        bitRate = 16;
    }

    *(volatile u32 *)0xA4500010 = dacRate - 1;
    *(volatile u32 *)0xA4500014 = bitRate - 1;
    return D_80037258 / (s32)dacRate;
}
