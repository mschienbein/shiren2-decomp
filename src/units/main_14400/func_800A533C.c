#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
extern s32 func_800A422C(void*, void*, s32); extern s32 func_800A529C(void*, s32);
s32 func_800A533C(u8 *a){ s32 failed = func_800A422C(a, a, a[9]&0xF) != 1; if (failed) return func_800A529C(a, 0); return 1; }
