#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

typedef struct { u8 pad[0x24]; void *x24; u8 pad28[0x4A]; u8 x72; } S;
extern S *func_800EFC70(S *, s32, u8);
extern void func_800E4D88(S *, s32);
extern u8 D_8015B688[];
S *func_80101BC0(S *p, u8 a){ S *self = p; func_800EFC70(p, 0x3E, a); self->x24 = D_8015B688; func_800E4D88(p, 0); self->x72 |= 2; return self;}
