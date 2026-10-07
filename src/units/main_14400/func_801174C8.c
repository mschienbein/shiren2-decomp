#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

void *func_800AC5B4(s32 size, s32 arg1);
void *func_80117490(void *obj);
void *func_801174C8(void) {
    return func_80117490(func_800AC5B4(0xC, 0));
}
