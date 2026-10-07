#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

u32 func_8006A75C(void);
u32 func_8006A7A8(u16 max) {
    u32 limit = max;
    u32 value = limit;
    u32 mask = 0;

    while (value != 0) {
        mask |= value;
        value >>= 1;
    }
    do {
        value = func_8006A75C() & mask;
    } while (value > limit);
    return value;
}
