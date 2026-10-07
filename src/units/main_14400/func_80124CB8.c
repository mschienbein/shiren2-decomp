#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
extern u8 D_80153AA0[]; extern void func_800AC68C(void *);
void func_80124CB8(u8 *a, s32 f){ *(void**)(a+8)=D_80153AA0; if (f&1) func_800AC68C(a); }
