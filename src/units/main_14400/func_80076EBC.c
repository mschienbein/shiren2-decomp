#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

s32 func_800625FC(s32 x, s32 y);
s32 func_800627C4(void);
s32 func_80062554(s32 x, s32 y);
s32 func_80076EBC(s32 x, s32 y) {
    if (func_800625FC(x, y) & 0x2000) {
        switch ((u32)func_800627C4()) {
        case 2:
            return -4;
        case 1:
            return -15;
        case 3:
            return -8;
        default:
            return -8;
        }
    }
    return func_80062554(x, y);
}
