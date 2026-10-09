#include "common.h"

/*
 * Chase the current target inside or outside a room (C++ TU, g++ 2.8.1;
 * same file family as func_800E7AA8). Pos has a user copy constructor, Dir
 * is a one-byte class whose turned() member keeps `facing` addressable
 * (reloaded from sp+0x3B after each call), and the first cell lookup goes
 * through the inline cellFlags(Pos &) so its &cur argument is substituted
 * directly while later uses share one hoisted register.
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
    Dir turned(s32 steps) const { return Dir(v + steps); }
};

struct Obj {
    Pos pos;            /* 0x00 */
    Dir facing;         /* 0x08 */
    u8 pad9[0x4F];
    Obj *target;        /* 0x58 */
    u8 pad5C[8];
    Pos dir;            /* 0x64 */
};

extern "C" {
extern u8 D_80147620[];
s32 func_800E66EC(Obj *obj);
u32 func_800B1C6C(Pos *pos);
void *func_800B1F90(void *pos);
s32 func_800B68B0(void *room);
u8 func_800A6420(Obj *obj, Obj *target);
Dir *func_800A22B8(Dir *out, Pos *to, Pos *from);
s32 func_800A4CC4(void *obj, void *pos, void *dir);
void func_800A665C(Obj *obj, Dir *dir);
void func_800A4F58(void *obj, Dir *dir);
void *func_800B6A98(void *out, void *room, s32 index);
s32 func_800C5844(void *rng, u8 base, u8 top);
void *func_800A6CC0(void *out, Obj *obj);
void *func_800A2594(Pos *out, void *from, Dir dir);
void *func_800B6CEC(Pos *out, void *room, Pos *from, Pos *to);
void *func_800A6538(void *out, void *obj, void *pos);
char *func_800A7DE4(Obj *obj);
s32 func_800A4EFC(void *obj, void *arg);
s32 func_800A50AC(void *obj);
s32 func_800A50E8(void *obj);
}

static inline u32 cellFlags(Pos &pos) { return func_800B1C6C(&pos); }

extern "C" s32 func_800E7794(Obj *obj)
{
    if ((obj->dir.y | obj->dir.x) != 0) {
        return func_800E66EC(obj);
    }
    Obj *target = obj->target;
    if (target == 0) {
        return func_800E66EC(obj);
    }
    Pos targetPos(target->pos);
    Pos cur(obj->pos);
    Pos next;

    if (cellFlags(cur) & 0x1000) {
        void *room = func_800B1F90(&cur);
        if (func_800B68B0(room) == 1) {
            u8 mode = func_800A6420(obj, target);
            if (mode != 3) {
                Dir toward;
                func_800A22B8(&toward, &targetPos, &cur);
                if (func_800A4CC4(obj, &cur, &toward)) {
                    Dir away(toward.turned(4));
                    func_800A665C(obj, &away);
                    func_800A4F58(obj, &toward);
                    return 1;
                }
                if (mode == 0) {
                    func_800B6A98(&next, room, 0);
                    obj->dir = next;
                    return func_800E66EC(obj);
                }
                Dir wander(toward.turned((u8)func_800C5844(D_80147620, 3, 5)));
                func_800A665C(obj, &wander);
                return 0;
            }
            func_800A6CC0(&next, obj);
            if (!(cellFlags(next) & 0x800)) {
                Dir facing(obj->facing);
                Pos left;
                func_800A2594(&left, &cur, facing.turned(2));
                if (cellFlags(left) & 0x800) {
                    Dir turn(facing.turned(2));
                    func_800A665C(obj, &turn);
                } else {
                    Pos right;
                    func_800A2594(&right, &cur, facing.turned(-2));
                    if (cellFlags(right) & 0x800) {
                        Dir turn(facing.turned(-2));
                        func_800A665C(obj, &turn);
                    }
                }
            }
        } else {
            if (func_800A6420(obj, target) == 3) {
                return func_800E66EC(obj);
            }
            func_800B6CEC(&next, room, &cur, &targetPos);
            obj->dir = next;
            return func_800E66EC(obj);
        }
    } else if (func_800A6420(obj, target) != 3) {
        Dir chase;
        func_800A6538(&chase, obj, &targetPos);
        Dir back(chase.turned(4));
        func_800A665C(obj, &back);
    }
    if (func_800A4EFC(obj, func_800A7DE4(obj))) {
        return 1;
    }
    if (func_800A50AC(obj)) {
        return 1;
    }
    return func_800A50E8(obj);
}
