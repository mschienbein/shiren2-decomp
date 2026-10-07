#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

extern void *D_801476B8;
u16 func_800E08B0(void *obj);
u16 func_80041BF4(void) {
    return func_800E08B0(D_801476B8);
}
