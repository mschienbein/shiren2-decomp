#include "common.h"

typedef unsigned char u8;

u8 *func_8006A810(u8 *dst, s32 value, s32 count) {
    u8 *p = dst;

    while (count--) {
        *p++ = value;
    }
    return dst;
}
