#include "common.h"
extern s32 D_8013A290, D_8013A298, D_801630D0;
extern s32 func_80054DF0(u32);
void func_800581A4(s32 value) { D_8013A298 = value; if (!D_8013A290 && D_801630D0 == 3) func_80054DF0(value + 11); }
