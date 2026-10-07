#include "common.h"

/* g++ 2.x vtable entry: this-adjust delta, index, function pointer. */
typedef struct { short delta; short index; s32 (*fn)(void *); } VEntry;

typedef struct { s32 x, y; } Pos;
typedef struct { s32 x0; VEntry *vt4; } Obj;
typedef struct { Pos pos; char pad[0x84]; Obj *obj8C; } S;
s32 func_800F17A8(S *p, Pos *pos, s32 arg);
s32 func_80107E40(S *p) {
    Pos t;
    Pos *tp = &t;
    Obj *o;
    s32 r;
    t.x = p->pos.x;
    tp->y = p->pos.y;
    o = p->obj8C;
    r = 0;
    if (o->vt4[4].fn((char *)o + o->vt4[4].delta) == 0) {
        r = func_800F17A8(p, tp, 1) != 0;
    }
    return r;
}
