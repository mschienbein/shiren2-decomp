#include "common.h"
typedef unsigned char u8;
extern u8 D_801480DC[];
extern const u8 D_8015488C[8];
s32 func_800D7860(s32 index)
{
    return (D_801480DC[index >> 3] & D_8015488C[index & 7]) != 0;
}
