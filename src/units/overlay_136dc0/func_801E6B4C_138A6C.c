#include "func_801E6B4C_138A6C/types.h"

/* Public slot is full-width; ROM narrows on entry @801E6B54. */
void func_801E6B4C_138A6C(s32 slot)
{
    u8 index = slot;
    HostUnit *p = func_8007946C(0, index);
    if (p->h02 != -1 && p->b07 != 2) func_80075E5C(index, 0);
}
