#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

void func_80062804(u32 mode, s32 id, s32 arg2, s32 arg3);
void func_800435D0(u8 *arg0) { func_80062804(4, *arg0, 0, 0); }
