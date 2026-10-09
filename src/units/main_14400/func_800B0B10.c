#include "common.h"
typedef unsigned char u8;
extern u8 D_8014313C[];
extern const u8 D_80154894[8];
static __inline__ s32 invalid_id(u8 id)
{
    return (u8)(id - 1) >= 40;
}
static __inline__ void clear_flag(u8 *bits, s32 index)
{
    bits[index >> 3] &= D_80154894[index & 7];
}
void func_800B0B10(s32 id)
{
    u8 value = id;
    s32 index;
    if (!invalid_id(id)) {
        index = value - 1;
        clear_flag(D_8014313C, index);
    }
}
