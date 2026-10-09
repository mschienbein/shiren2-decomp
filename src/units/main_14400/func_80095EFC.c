#include "common.h"

/* Owned initialized .data word; defining it here lets gas fill the jr delay slot. */
s32 D_80140254 = 0;

void func_80095EFC(void)
{
    D_80140254 = -1;
}
