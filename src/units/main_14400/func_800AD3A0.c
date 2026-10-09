#include "common.h"

typedef unsigned char u8;

extern u8 D_80143064[32];

void func_800AD3A0(void) {
    s32 i;

    for (i = 31; i != -1; i--) {
        D_80143064[i] = 0xFF;
    }
}
