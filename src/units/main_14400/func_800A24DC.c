#include "common.h"
typedef struct { s32 x, y; } Pt;
typedef struct { Pt a, b; } Rect;
s32 func_800A2854(void *, Pt *, Pt *);
s32 func_800A24DC(void *self, Rect *r) {
    Rect t;
    t.a.x = r->a.x;
    t.a.y = r->a.y;
    t.b.x = r->b.x;
    t.b.y = r->b.y;
    return func_800A2854(self, &t.a, &t.b);
}
