#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
extern u8 D_80142F26[];
s32 func_800AA48C(u8 a, u8 b){ s32 r=0; if (a==D_80142F26[0]) r = b==D_80142F26[1]; return r; }
