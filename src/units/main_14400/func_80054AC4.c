#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
extern u8 D_80161B45[], D_80162B4A[];
extern s16 D_80162B46, D_80162B48;
extern u8 D_801398B8;
void func_800265E0(void *, s32);
void func_80054AC4(void) { func_800265E0(D_80161B45, 0x1000); D_80162B46 = 0; D_80162B48 = 0; D_801398B8 = 0; func_800265E0(D_80162B4A, 0x1CC); }
