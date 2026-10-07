#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

s32 func_8010CB2C(void *a, s32 b);
s32 func_80116CD8(void *a, s32 b) {
    s32 r = 0;
    if (b == 0x1D || func_8010CB2C(a, b)) r = 1;
    return r;
}
