#include "common.h"

/*
 * Ranged approach: stay `range` cells from the target (C++ TU, g++ 2.8.1).
 * Evidence for C++: every `dir = Dir(i)` builds the Dir temporary in its own
 * stack byte (dead stores at sp+0x31/0x33/0x34) before the member-wise copy
 * into `dir`, and each Pos handed to func_800A23E8/func_800A282C is first
 * copied (user copy constructor) into a temporary whose address is passed.
 * func_800A23E8 rewrites the Pos it receives (it subtracts the origin in
 * place), so it gets a pointer to an explicit scratch copy, matching its C
 * definition; the read-only func_800A282C takes its copy by const reference
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
    Dir() {}
    Dir(s32 d) : v(d & 7) {}
};

struct Unit {
    Pos pos;            /* 0x00 */
    char pad8[0x4C];
    u8 flags;           /* 0x54 */
    char pad55[3];
    Unit *target;       /* 0x58 */
    char pad5C[8];
    Pos dest;           /* 0x64 */
};

extern "C" {
s32 func_800E66EC(Unit *unit);
s32 func_800A650C(Unit *unit, Pos *pos);
u8 func_800A6420(Unit *unit, Unit *target);
void *func_800B1F90(void *pos);
s32 func_800B68F0(void *room, Pos *pos);
s32 func_800E65C0(Unit *unit, Pos *pos, s32 mode);
s32 func_800A282C(Pos *from, const Pos &to);
void *func_800A6538(void *out, void *unit, void *pos);
void func_800A665C(Unit *unit, u8 *dir);
void *func_800A2594(Pos *out, void *from, Dir dir);
s32 func_800A23E8(Pos *origin, Pos *pos);
void *func_800A7DEC(void *unit);
s32 func_800A4CC4(void *unit, void *pos, void *dir);
void *func_800A65E4(void *out, void *unit, void *target);
Dir *func_800A22B8(Dir *out, Pos *from, Pos *to);
void func_800A4F58(void *unit, Dir *dir);
}

static inline s32 canStep(Unit *u, Dir *dir) { return func_800A4CC4(u, func_800A7DEC(u), dir); }

/* func_800A23E8 rewrites the Pos it is given: pass it the full-expression
 * temporary copy (a non-const object, so writing through it is valid). */
static inline Pos *scratch(const Pos &copy) { return const_cast<Pos *>(&copy); }

extern "C" s32 func_800E7AA8(Unit *u, s32 range)
{
    Unit *target = u->target;

    if (target == 0) {
        return func_800E66EC(u);
    }
    Pos me(u->pos);
    Pos goal(target->pos);
    s32 dist = func_800A650C(u, &goal);
    u8 kind = func_800A6420(u, target);

    if (kind == 2) {
        void *room = func_800B1F90(&me);
        if (room != 0 && func_800B68F0(room, &goal) != 0) {
            u->dest = goal;
        }
    } else if (kind == 3) {
        return func_800E66EC(u);
    }
    if (range + 1 < dist) {
        return func_800E65C0(u, &goal, 0);
    }
    if (dist == range && func_800A282C(&me, Pos(goal))) {
        u8 act;
        func_800A6538(&act, u, &goal);
        func_800A665C(u, &act);
        return 0;
    }

    Pos next;
    Dir dir;
    s32 i;
    for (i = 0; i < 8; i++) {
        func_800A2594(&next, &me, dir = Dir(i));
        if (func_800A23E8(&goal, scratch(Pos(next))) != range) {
            continue;
        }
        s32 ok = 0;
        if (canStep(u, &dir)) {
            ok = func_800A282C(&next, Pos(goal)) != 0;
        }
        if (ok) {
            break;
        }
    }
    if (i == 8) {
        for (i = 0; i < 8; i++) {
            func_800A2594(&next, &me, dir = Dir(i));
            if (func_800A23E8(&goal, scratch(Pos(next))) != range) {
                continue;
            }
            if (canStep(u, &dir)) {
                break;
            }
        }
        if (i == 8) {
            for (i = 0; i < 8; i++) {
                func_800A2594(&next, &me, dir = Dir(i));
                if (func_800A23E8(&goal, scratch(Pos(next))) < 2) {
                    continue;
                }
                s32 ok = 0;
                if (canStep(u, &dir)) {
                    ok = func_800A282C(&next, Pos(goal)) != 0;
                }
                if (ok) {
                    break;
                }
            }
            if (i == 8) {
                if (func_800A6420(u, target) == 0) {
                    u8 act;
                    func_800A65E4(&act, u, target);
                    func_800A665C(u, &act);
                    u->flags |= 4;
                }
                return 0;
            }
        }
    }
    func_800A2594(&next, &me, dir);
    Dir step;
    func_800A22B8(&step, &next, &goal);
    func_800A665C(u, &step.v);
    func_800A4F58(u, &dir);
    return 1;
}
