#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

void func_8007C224(void);
void func_80076734(s32 a, s32 b);
void func_80046BF4(s32 a, s32 b) { func_8007C224(); func_80076734(a, b); }
