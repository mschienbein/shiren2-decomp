#include "map_rect.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
extern u8 D_80147620[]; extern s32 func_800C5A1C(void*, s32, s32);
MapPos *func_800A33DC(MapPos *r, MapRect *q){ s32 t = func_800C5A1C(D_80147620, q->min.y, q->max.y); r->x = func_800C5A1C(D_80147620, q->min.x, q->max.x); r->y = t; return r; }
