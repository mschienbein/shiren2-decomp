#include "common.h"
typedef unsigned char u8;
typedef struct { signed char value; } Direction;
typedef struct { u8 value; } Dir;
typedef struct { s32 x; s32 y; } Point;
typedef struct { Point point; u8 field8; } Obj;
extern void *func_800A2594(Point *, Obj *, Dir);
extern u8 func_800A8C00(Obj *);
extern s32 func_800A4360(Obj *, Point *);
extern s32 func_800765E8(s32, s32, s32, s32, s32, s32, s32, s32);
static inline void copy_point(Point *out, Point *in) { out->x = in->x; out->y = in->y; }
void func_80046B3C(Obj *p, Direction direction) { Point start; Point end; Dir dir; s32 kind; s32 first, second, flags; copy_point(&start, &p->point); dir.value = (u8)direction.value; func_800A2594(&end, p, dir); kind = (u8)func_800A8C00(p); first = func_800A4360(p, &start); second = func_800A4360(p, &end); flags = p->field8; func_800765E8(kind, start.y, start.x, first, end.y, end.x, second, flags); }
