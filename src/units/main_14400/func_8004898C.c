#include "common.h"

typedef unsigned char u8;

void func_80083690(u32 index, s32 value);
void func_8004898C(u8 *a, s32 value) { s32 v = *(s32 *)(a + 0xC); if (v >= 0) func_80083690(v, value); }
