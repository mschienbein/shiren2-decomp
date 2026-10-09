#include "common.h"
/* Original initialized data word at 0x8013E920 is 1. */
s32 D_8013E920 = 1;
void func_80083FE0(s32 arg) { D_8013E920 = arg; }
