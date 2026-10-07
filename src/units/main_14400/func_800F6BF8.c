#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

void *func_800A38FC(s32 size);
void *func_800F6BB0(void *obj, u8 arg1);
void *func_800F6BF8(u8 arg0) {
    return func_800F6BB0(func_800A38FC(0x80), arg0);
}
