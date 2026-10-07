#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

extern u8 D_80147490;
s32 func_800B74C0(s32 index) {
    return (D_80147490 >> (index - 0x18)) & 1;
}
