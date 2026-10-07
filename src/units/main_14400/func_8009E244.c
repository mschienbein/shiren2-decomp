#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

extern u8 D_80151E38[];
extern void func_800D8FA8(void *object);
typedef struct { u8 pad[0x4C]; void *x4C; } S;
void func_8009E244(S *p, s32 flags){ p->x4C = D_80151E38; if (flags & 1) func_800D8FA8(p);}
