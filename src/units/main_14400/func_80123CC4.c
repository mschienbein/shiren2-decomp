#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

void *func_800AC5B4(s32 size, s32 flags);
void *func_80123C80(void *obj);
void *func_80123CC4(void) {
    return func_80123C80(func_800AC5B4(0x10, 0));
}
