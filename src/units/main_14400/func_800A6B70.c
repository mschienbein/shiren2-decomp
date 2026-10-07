#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

void func_800A694C(void *, u8 *, u8 *, u8, s32);

void *func_800A6B70(void *dst, u8 *src, u8 mode, s32 arg) {
    func_800A694C(dst, src, src + 0xC, mode, arg);
    return dst;
}
