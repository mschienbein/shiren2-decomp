#include "common.h"

typedef unsigned char u8;
extern u8 D_80143064[];

s32 func_800AD37C(u8 index) {
    return (D_80143064[index / 8] >> (index % 8)) & 1;
}
