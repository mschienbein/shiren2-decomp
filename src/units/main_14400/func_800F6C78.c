#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

void *func_800A38FC(s32 size);
void *func_800F6C30(void *obj, u8 id);
void *func_800F6C78(u8 id) {
    return func_800F6C30(func_800A38FC(0x80), id);
}
