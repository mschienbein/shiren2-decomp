#include "common.h"

/* Initialized .data word at its original address (value 0); this file owns it. */
s32 D_8013D8F8 = 0;

void func_800740CC(s32 arg) {
    D_8013D8F8 = arg;
}
