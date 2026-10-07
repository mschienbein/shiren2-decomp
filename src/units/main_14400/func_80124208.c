#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

void *func_800AC5B4(s32 size, s32 flags);
void *func_801241D0(void *obj);
void *func_80124208(void) {
    return func_801241D0(func_800AC5B4(0x10, 0));
}
