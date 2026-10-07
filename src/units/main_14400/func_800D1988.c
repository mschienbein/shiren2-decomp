#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
s32 func_800D1988(u8 *a, s32 b){ s32 i = 0; b*=4; for (;i<4;i++){ if (a[i+b]==0) return i; } return 4; }
