#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

extern u8 D_80169AFC[][54];
void func_80062658(s32 a, s32 b) { D_80169AFC[a][b] = 0; }
