#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

extern u32 D_8016DB18;
s32 func_80041574(s32 a, s32 b);
s32 func_800669FC(s32 id);
s32 func_80062C64(s32 a, s32 b) {
    s32 result = 0;

    switch (D_8016DB18) {
    case 1:
    case 2:
        result = func_800669FC(func_80041574(a, b));
        break;
    }
    return result;
}
