#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
s32 func_800F3B00(u8 *a){ if (*(u16*)(a+0x9A)&0x10) return 1; return 0; }
