#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

extern u8 D_80157FA8[];
extern u8 D_801582D8[];
typedef struct { u16 x0; u16 pad; void *x4; } S;
S *func_800DA4D0(S *s){ s->x4 = D_80157FA8; s->x0 = 0x3A; s->x4 = D_801582D8; return s;}
