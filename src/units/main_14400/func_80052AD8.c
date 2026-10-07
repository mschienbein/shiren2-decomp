#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

u8 func_80042498(s32 kind);
s32 func_80129F10(void *arg, u8 value);
s32 func_80052AD8(void *arg) {
    return func_80129F10(arg, func_80042498(2));
}
