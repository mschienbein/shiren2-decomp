#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

extern u32 D_8013CA10;
u32 func_8006A75C(void) {
    D_8013CA10 = D_8013CA10 * 0x343FD + 0x269EC3;
    return D_8013CA10 >> 16;
}
