#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

extern void func_800CA088(void *);
extern u8 D_80149F80[];
typedef struct { u8 pad[0xC]; s32 xC; u8 x10; u8 x11; u8 pad2[6]; void *x18; } S;
S *func_800CA048(S *p, s32 a, s32 b){ p->x18 = D_80149F80; p->x10 = a; p->x11 = b; p->xC = 0; func_800CA088(p); return p;}
