#include "common.h"

/* Owned here: gas 2.9.1 fills the jr delay slot only for a locally defined symbol. */
s32 D_8013D8E0 = 0;

void func_80074084(s32 arg)
{
    D_8013D8E0 = arg;
}
