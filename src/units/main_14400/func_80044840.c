#include "common.h"
typedef struct { s32 x, y; } Vec;
typedef struct { s32 id; Vec pos; } End;
typedef struct { s32 pad; End *ends; } Link;
typedef struct Query Query;
extern Vec *D_801476B8;
extern Link *D_80138BB0;
extern s32 func_800A23E8(Vec *, Vec *);
/* The original caller supplies a receiver; this method instead uses globals. */
End *func_80044840(Query *unused_receiver) {
    Vec me;
    Vec t;
    Vec c;
    Vec *pm = &me;
    Vec *src;
    End *e0;
    End *e1;
    Vec *pos;
    s32 d0;
    src = D_801476B8;
    pm->x = src->x;
    pm->y = src->y;
    e0 = D_80138BB0->ends;
    e1 = e0 + 1;
    pos = &e0->pos;
    t.x = pos->x;
    t.y = pos->y;
    if ((t.y | t.x) == 0) return e1;
    t = e1->pos;
    if ((t.y | t.x) == 0) return e0;
    pos = &e0->pos;
    c.x = pos->x;
    c.y = pos->y;
    d0 = func_800A23E8(pm, &c);
    pos = &e1->pos;
    c.x = pos->x;
    c.y = pos->y;
    if (func_800A23E8(pm, &c) >= d0) return e0;
    return e1;
}
