#include "common.h"

/* Owned here (original initializer 1) so gas can place the %lo store in the jr delay slot. */
s32 D_8013D900 = 1;

void func_800740E4(s32 arg) {
    D_8013D900 = arg;
}
