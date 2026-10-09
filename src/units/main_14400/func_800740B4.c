#include "common.h"

/* Owned initialized .data word; defining it here lets gas fill the jr delay slot. */
s32 D_8013D8F0 = 0;

void func_800740B4(s32 arg)
{
    D_8013D8F0 = arg;
}
