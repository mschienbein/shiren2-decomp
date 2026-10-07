#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

extern u8 D_80143044[];

void func_800AD42C(u8 bit) {
    D_80143044[bit / 8] &= ~(1 << (bit % 8));
}
