#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

s32 func_800E1CC4(void *arg0, s32 arg1);
s32 func_800E3058(void *arg0) {
    s32 result;
    if (func_800E1CC4(arg0, 3) != 0) {
        result = 2;
    } else {
        result = 1;
    }
    return result;
}
