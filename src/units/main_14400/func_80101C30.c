#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
extern u8 D_8015B688[]; extern void func_800EFD28(void*, s32); extern void func_800A3918(void*);
void func_80101C30(u8 *a, s32 f){ *(void**)(a+0x24)=D_8015B688; func_800EFD28(a,0); if (f&1) func_800A3918(a); }
