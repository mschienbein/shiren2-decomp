#include "common.h"

s32 D_8013B758 = 0;

s32 func_80061230(s32 value)
{
    s32 previous = D_8013B758;
    D_8013B758 = value;
    return previous;
}

s32 func_80061244(void)
{
    return D_8013B758;
}
