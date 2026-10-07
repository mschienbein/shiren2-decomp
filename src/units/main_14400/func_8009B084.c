#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
extern void func_8009B0C0(void *, void *); extern void func_8009B1D8(void *, void *);
void func_8009B084(u8 *a, void *pair){ if (*(s32*)(a+0x80)==3) func_8009B0C0(a, pair); else func_8009B1D8(a, pair); }
