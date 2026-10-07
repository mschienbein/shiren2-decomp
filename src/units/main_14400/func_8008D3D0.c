#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

u8 *func_8006A810(void *dst, s32 val, s32 size);

void func_8008D3D0(void *dst) {
    func_8006A810(dst, 0, 0x1B8);
}
