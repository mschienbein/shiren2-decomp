#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

s32 func_800A422C(void *a, void *b, s32 mode);
s32 func_800B4888(void *a);
s32 func_800A42CC(void *a, void *b, s32 mode) {
    s32 r = 0;
    if (func_800A422C(a, b, mode)) r = func_800B4888(b) == 0;
    return r;
}
