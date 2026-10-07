#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef float f32;

extern f32 *D_8013B804;
s32 func_80062554(s32 x, s32 y) {
    x -= 10;
    if ((u32)x >= 0x38 || y < 10 || y >= 0x2C || D_8013B804 == 0) {
        return 0;
    }
    y -= 10;
    return D_8013B804[y * 0x38 + x];
}
