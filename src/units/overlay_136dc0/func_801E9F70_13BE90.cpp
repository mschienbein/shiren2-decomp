#include "func_801E9F70_13BE90/types.h"

extern "C" u8 func_801E9F70_13BE90(s32 bit) {
    u32 index = (u32)bit;
    return (D_801F5248_147168[index >> 3] >> (index & 7)) & 1;
}
