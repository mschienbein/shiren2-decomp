#include "common.h"

typedef unsigned char u8;
extern u8 D_8015374C[];
extern u8 func_800AC1AC(u8 value);

u8 func_800AF4B0(u8 input) {
    s32 value = input;
    value -= D_8015374C[func_800AC1AC(value)];
    return value;
}
