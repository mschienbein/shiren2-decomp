#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
extern u8 D_801586E8[]; extern void *func_800DA8A0(void *obj, s32 kind, void *src);
void *func_800DCCC0(u8 *a, void *src){ func_800DA8A0(a,0x1D,src); *(void**)(a+4)=D_801586E8; return a; }
