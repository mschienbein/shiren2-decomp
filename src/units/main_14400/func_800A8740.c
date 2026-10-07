#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

extern u8 D_801C51A4[];
extern s32 D_80142B00;
void func_800A8740(void){ u8 *p = D_801C51A4; s32 i = 3; do { *p++ = 0; } while (i-- > 0); D_80142B00 = -1;}
