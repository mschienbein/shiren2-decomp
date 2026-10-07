#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

extern void *func_800DA904(void *obj, s32 kind, u8 *params);
extern u8 D_801586B8[];
typedef struct { u8 pad[4]; void *x4; } S;
S *func_800DCC00(S *p, u8 *a){ func_800DA904(p, 0x1C, a); p->x4 = D_801586B8; return p;}
