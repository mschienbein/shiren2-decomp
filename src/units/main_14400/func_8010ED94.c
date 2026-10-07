#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
s32 func_8010BEC4(void *obj, u8 id);
void func_8010ED94(void *item, u32 *flags) { if ((u8)func_8010BEC4(item, 0x4E)) *flags |= 0x400000; }
