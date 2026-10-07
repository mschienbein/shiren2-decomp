#include "common.h"

typedef unsigned char u8;
extern u8 D_80143044[];

void func_800AD3F4(u8 id) {
    D_80143044[id / 8] |= 1 << (id % 8);
}
