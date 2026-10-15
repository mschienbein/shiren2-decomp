#include "func_801E9F90_13BEB0/types.h"

extern "C" void func_801E9F90_13BEB0(s32 bit, u8 set) {
    u32 index = (u32)bit;
    s32 shift = index % 8;
    if (set) {
        D_801F5248_147168[index / 8] |= 1 << shift;
    } else {
        D_801F5248_147168[index / 8] &= ~(1 << shift);
    }
}
