#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

u8 func_80042498(s32 kind);
int func_80129F10(void *arg, int value);
s32 func_80052AD8(void *arg) {
    return func_80129F10(arg, (u8)func_80042498(2));
}
