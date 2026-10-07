#include "common.h"
extern unsigned short D_80163110[];
s32 func_80058A20(s32 a) {
    s32 result;
    if (!(a & 0x10)) result = a;
    else result = D_80163110[a & 0xF];
    return result;
}
