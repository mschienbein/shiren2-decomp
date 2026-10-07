#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

extern u8 D_80143064[];

void func_800AD308(u8 flag) {
    D_80143064[flag / 8] |= 1 << (flag % 8);
}
