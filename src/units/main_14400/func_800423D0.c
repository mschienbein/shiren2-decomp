#include "common.h"
typedef unsigned short u16;
extern u16 func_800B5768(void *object);
extern u16 D_80156A04, D_80156A00, D_801569FC;
s32 func_800423D0(s32 y, s32 x) {
    s32 position[2];
    s32 value;
    position[1] = y;
    position[0] = x;
    value = (short)func_800B5768(position);
    if (value == D_80156A04) value = 1;
    else if (value == D_80156A00) value = 2;
    else if (value == D_801569FC) value = 3;
    else value = 0;
    return value;
}
