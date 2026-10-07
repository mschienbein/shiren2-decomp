#include "common.h"

typedef unsigned char u8;
extern u8 D_80143064[];

void func_800AD340(u8 bit) {
    s32 index = bit / 8;
    s32 mask = 1 << (bit % 8);

    D_80143064[index] &= ~mask;
}
