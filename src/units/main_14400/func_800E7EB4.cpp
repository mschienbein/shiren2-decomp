#include "common.h"

/*
 * Walk toward the nearest 0x2000-flagged cell of the unit's area (C++ TU, g++ 2.8.1).
 * Evidence for C++: every Pos handed to func_800A23E8/func_800A27A4 is first
 * copied member-wise (user copy constructor) into a temporary whose stack slot
 * is reused by later temporaries and block-scope locals (sp+0x38), and loop 2
 * recomputes that address each iteration; the C spelling hoists it.
 * func_800A23E8 rewrites the Pos it receives (it subtracts the origin in
 * place), so it gets a pointer to an explicit scratch copy, matching its C
 * definition; the read-only func_800A27A4 takes its copy by const reference
 * (pointer ABI).
 */

typedef unsigned char u8;

struct Pos {
    s32 x;
    s32 y;
    Pos() {}
    Pos(const Pos &o) : x(o.x), y(o.y) {}
};

struct Dir {
    u8 v;
};

struct RectIter {
    Pos pos;    /* 0x00 */
    Pos start;  /* 0x08 */
    Pos end;    /* 0x10 */
};

struct Obj {
    Pos pos;            /* 0x00 */
    char pad8[4];
    Pos min;            /* 0x0C */
    Pos max;            /* 0x14 */
    char pad1C[0x48];
    Pos target;         /* 0x64 */
    char pad6C[6];
    u8 flags;           /* 0x72 */
};

extern "C" {
Pos *func_800A3610(Pos *out, RectIter *it);
s32 func_800A23E8(Pos *origin, Pos *pos);
u32 func_800B1C6C(Pos *pos);
void *func_800A27A4(Dir *out, Pos *from, const Pos &to);
void *func_800A2594(Pos *out, Pos *from, Dir dir);
s32 func_800A4360(Obj *obj, Pos *pos);
s32 func_800A422C(Obj *obj, Pos *pos, s32 arg);
s32 func_800E66EC(Obj *obj);
}

static inline s32 RectIter_valid(RectIter *it) { return it->pos.x <= it->end.x; }
static inline s32 isPositive(s32 v) { return v > 0; }

/* func_800A23E8 rewrites the Pos it is given: pass it the full-expression
 * temporary copy (a non-const object, so writing through it is valid). */
static inline Pos *scratch(const Pos &copy) { return const_cast<Pos *>(&copy); }

extern "C" s32 func_800E7EB4(Obj *obj)
{
    obj->flags |= 2;
    if ((obj->target.y | obj->target.x) != 0) {
        return func_800E66EC(obj);
    }
    Pos cur(obj->pos);
    Pos best;
    RectIter iter;
    iter.start = Pos(obj->min);
    iter.pos = iter.start;
    iter.end = Pos(obj->max);
    s32 bestDist = 1000;
    while (RectIter_valid(&iter)) {
        Pos p;
        func_800A3610(&p, &iter);
        s32 dist = func_800A23E8(&cur, scratch(Pos(p)));
        if ((func_800B1C6C(&p) & 0x2000) && dist < bestDist) {
            best = p;
            bestDist = dist;
        }
    }
    if (bestDist == 1000) {
        return 0;
    }
    s32 steps = func_800A23E8(&cur, scratch(Pos(best))) * 2;
    while (isPositive(steps--)) {
        Dir dir;
        func_800A27A4(&dir, &cur, Pos(best));
        Pos next;
        func_800A2594(&next, &cur, dir);
        if (!func_800A422C(obj, &next, func_800A4360(obj, &next))) {
            continue;
        }
        cur = next;
        if (!(func_800B1C6C(&cur) & 0x2000)) {
            continue;
        }
        obj->target = cur;
        return func_800E66EC(obj);
    }
    return 0;
}
