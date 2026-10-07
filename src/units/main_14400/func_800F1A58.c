#include "common.h"
typedef struct { s32 x, y; } Point;
typedef struct { Point position; unsigned char field_8; } Obj;
typedef struct { char pad[0x18]; Obj *field_18; s32 field_1C; } Iterator;
extern void func_800C2B40(Iterator *, Point *, unsigned char *, s32);
extern s32 func_800C2BBC(Iterator *, unsigned char), func_800A4520(Obj *, Obj *), func_800A674C(Obj *, Obj *), func_800A6E90(Obj *);
Obj *func_800F1A58(Obj *a, s32 b, s32 c) {
    Point position; Iterator iter; unsigned char kind; Point *pos = &position;
    pos->x = a->position.x; pos->y = a->position.y; kind = a->field_8;
    func_800C2B40(&iter, pos, &kind, b);
    while (func_800C2BBC(&iter, 0x7C)) {
        s32 found = 0; Obj *p = iter.field_18;
        if (func_800A4520(a, p) && (c || func_800A674C(a, p)) && !func_800A6E90(p)) found = 1;
        if (found) return p;
    }
    return 0;
}
