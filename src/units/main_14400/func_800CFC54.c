#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

extern s32 func_800AD8AC(void *obj, void *pos);
void func_800CFC54(u8 *p, s32 flag, void *x){ if (!flag) func_800AD8AC(x, p + 8);}
