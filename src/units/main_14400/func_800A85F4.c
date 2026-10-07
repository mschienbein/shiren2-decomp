#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

void *func_800A38FC(s32 size);
void *func_800EF930(void *mem, u8 kind);
s32 func_800A3934(void *obj);
void *func_800A85F4(u8 kind) {
    void *obj = func_800EF930(func_800A38FC(0xAC), kind);
    if (func_800A3934(obj) != 0) {
        obj = 0;
    }
    return obj;
}
