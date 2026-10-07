#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

s32 func_800B0FFC(u8 c);
s32 func_800B101C(u8 *p) {
    s32 i = 3;
    do {
        s32 failed;
        if (*p == 0) return 1;
        failed = func_800B0FFC(*p) != 1;
        if (failed) return 0;
        p++;
    } while (--i >= 0);
    return 1;
}
