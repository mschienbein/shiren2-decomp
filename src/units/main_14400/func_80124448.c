#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

void *func_800AC5B4(s32 size, s32 arg1);
void *func_80124400(void *obj);

void *func_80124448(void) {
    return func_80124400(func_800AC5B4(0x14, 0));
}
