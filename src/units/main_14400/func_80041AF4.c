#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

extern void *D_801476B8;
s32 func_800EB37C(void *obj);
s32 func_80041AF4(void) {
    return (u8)func_800EB37C(D_801476B8);
}
