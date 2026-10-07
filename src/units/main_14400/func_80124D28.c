#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

void *func_800AC5B4(s32 size, s32 arg1);
void *func_80124CF0(void *obj);
void *func_80124D28(void) {
    return func_80124CF0(func_800AC5B4(0x10, 0));
}
