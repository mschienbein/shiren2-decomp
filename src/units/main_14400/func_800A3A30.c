#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

extern s32 func_800A3A20(u8);
s32 func_800A3A30(u8 a){ s32 r = 0; if (func_800A3A20(a)) r = !func_800A3A20(a); return r;}
