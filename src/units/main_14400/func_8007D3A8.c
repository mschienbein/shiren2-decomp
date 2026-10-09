#include "common.h"

/* D_8013DE98 is owned once by this joint unit: the setter's store and the
 * getter's load both sit in jr delay slots, which needs the local definition. */
s32 D_8013DE98 = 0;

void func_8007D3A8(s32 value)
{
    D_8013DE98 = value;
}

s32 func_8007D3B4(void)
{
    return D_8013DE98;
}
