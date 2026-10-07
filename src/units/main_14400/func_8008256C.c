#include "common.h"

extern s32 D_8013E818;
void func_800823D8(s32, s32);
void func_8008256C(s32 a, s32 b) { if (b >= D_8013E818 - 1) b = 0; else b = D_8013E818 - b - 1; func_800823D8(a, b); }
