#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
extern u8 D_80151E38[]; extern void func_800D8FA8(void *object);
void func_8009E520(u8 *a, s32 f){ *(void**)(a+0x4C)=D_80151E38; if (f&1) func_800D8FA8(a); }
