#include "common.h"
typedef unsigned char u8;
extern s32 D_80147620[];
extern u8 func_800C57A0(void *);
extern s32 func_800E7104(void *), func_800A50E8(void *);
s32 func_800FE2EC(void *a) {
    s32 result;
    if (!(func_800C57A0(D_80147620) & 1)) result = func_800E7104(a);
    else result = func_800A50E8(a);
    return result;
}
