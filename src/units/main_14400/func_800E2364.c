#include "common.h"

/* Status-effect bit mask, initialized by this static constructor (in the ctor list). */
s32 D_80148270 = 0;

void func_800E2364(void)
{
    D_80148270 = 0x108F000;
}
