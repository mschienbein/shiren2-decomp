#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
typedef struct { s32 a, b; } P2; typedef struct { s32 x0, x4, x8, xC; } Q; extern u8 D_80147620[]; extern s32 func_800C5A1C(void*, s32, s32);
P2 *func_800A33DC(P2 *r, Q *q){ s32 t = func_800C5A1C(D_80147620, q->x4, q->xC); r->a = func_800C5A1C(D_80147620, q->x0, q->x8); r->b = t; return r; }
