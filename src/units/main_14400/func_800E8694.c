#include "common.h"
typedef unsigned char u8;
extern s32 func_800E1D14(void *, s32);
extern s32 func_800E1CC4(void *, s32);
extern s32 func_800E1CD4(void *, s32);
s32 func_800E8694(u8 *object) {
    s32 result = 0;
    if (func_800E1D14(object, 0x13) || func_800E1CC4(object, 0) ||
        (func_800E1CC4(object, 1) && !((object[0x1E] >> 2) & 1)) ||
        func_800E1CD4(object, 0x10)) result = 1;
    return result;
}
