#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

extern void func_800D4F5C(void *, s32);
extern void func_800D0690(void *, s32);
extern u8 D_80147FB8[], D_80147FAC[], D_80147FA0[];
void func_800D50CC(void){ func_800D4F5C(D_80147FB8, 2); func_800D0690(D_80147FAC, 2); func_800D0690(D_80147FA0, 2);}
