#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
extern u8 D_801545E0[]; extern u8 D_80154300[]; extern void func_800D8FA8(void *obj);
void func_800D03C4(u8 *a, s32 f){ *(void**)(a+4)=D_801545E0; *(s32*)(a+0x10)=0; *(void**)(a+4)=D_80154300; if (f&1) func_800D8FA8(a); }
