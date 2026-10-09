#include "common.h"

typedef struct { s32 x, y; } Pos;
typedef struct { unsigned char v; } Dir;
typedef struct { Pos pos; Dir dir; char pad[3]; s32 xC; char pad2[0x18]; s32 x28; Pos prev; } S;
void *func_800A2594(Pos *, void *, Dir);
u32 func_800B1C6C(void *);
static inline s32 dir_get(Dir *d) { return d->v; }
static inline void dir_set(Dir *d, s32 v) { d->v = v & 7; }
/* ODD_C: Preserve the inline equality predicate rather than branch assignments. */
static inline unsigned char unblocked(u32 flags) {
    return flags == 0;
}
s32 func_800C414C(S *s, Pos *from, Dir dir) {
    s32 step;
    s32 ok;
    Pos a, b, c;
    Dir t;
    Dir d;
    if (s->x28 == 0) return 0;
    step = 1;
    {
        s32 even = (dir_get(&dir) & 1) == 0;
        if (even) step = 2;
    }
    ok = 1;
    t.v = (dir_get(&dir) + 4) & 7;
    func_800A2594(&a, from, t);
    dir_set(&d, dir_get(&dir) - step);
    func_800A2594(&b, &a, d);
    if (func_800B1C6C(&b) & 0x4000) {
        dir_set(&d, dir_get(&dir) + step);
        func_800A2594(&c, &a, d);
        b = c;
        ok = unblocked(func_800B1C6C(&b) & 0x4000);
    }
    if (ok) {
        s->prev = a;
        s->pos = b;
        s->dir = d;
        if (s->xC > 0) s->xC--;
        return 1;
    }
    return 0;
}
