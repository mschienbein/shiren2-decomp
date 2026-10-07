#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

extern void *D_801476B8;
u16 func_800E08F0(void *obj);

u16 func_80041C1C(void) {
    return (u16)func_800E08F0(D_801476B8);
}
