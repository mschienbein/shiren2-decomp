#include "common.h"
#include "map_rect.h"

/*
 * Chase the current target inside or outside a room (C++ TU, g++ 2.8.1;
 * same file family as func_800E7AA8). MapPos supplies native member-wise
 * copy construction; Dir is a one-byte class whose turned() member keeps
 * `facing` addressable (reloaded from sp+0x3B after each call). The first
 * cell lookup keeps its reference boundary through cellFlags(MapPos &).
 */

typedef unsigned char u8;

/* Borrowed D_801431F0 / func_800B1F90 room domain: retain the complete
 * 0x14-byte record, not just its leading rectangle, for all room providers. */
struct Room_800E7794 {
    MapRect rect;
    u8 sideCounts[4];
};

struct Dir_800E7794 {
    u8 v;
    Dir_800E7794() {}
    Dir_800E7794(s32 d) : v(d & 7) {}
    Dir_800E7794 turned(s32 steps) const { return Dir_800E7794(v + steps); }
};

/* Borrowed unit view; this TU does not allocate or truncate the unit. */
struct Obj_800E7794 {
    MapPos pos;         /* 0x00 */
    Dir_800E7794 facing;         /* 0x08 */
    u8 pad9[0x4F];
    Obj_800E7794 *target;        /* 0x58 */
    u8 pad5C[8];
    MapPos dir;         /* 0x64 */
};

extern "C" {
extern u8 D_80147620[];
s32 func_800E66EC(Obj_800E7794 *obj);
u32 func_800B1C6C(MapPos *pos);
Room_800E7794 *func_800B1F90(MapPos *pos);
s32 func_800B68B0(Room_800E7794 *room);
u8 func_800A6420(Obj_800E7794 *obj, Obj_800E7794 *target);
Dir_800E7794 *func_800A22B8(Dir_800E7794 *out, MapPos *to, MapPos *from);
s32 func_800A4CC4(void *obj, MapPos *pos, Dir_800E7794 *dir);
void func_800A665C(Obj_800E7794 *obj, Dir_800E7794 *dir);
void func_800A4F58(void *obj, Dir_800E7794 *dir);
MapPos *func_800B6A98(MapPos *out, Room_800E7794 *room, s32 index);
s32 func_800C5844(void *rng, u8 base, u8 top);
void *func_800A6CC0(MapPos *out, Obj_800E7794 *obj);
void *func_800A2594(MapPos *out, MapPos *from, Dir_800E7794 dir);
MapPos *func_800B6CEC(MapPos *out, Room_800E7794 *room, MapPos *from, MapPos *to);
void *func_800A6538(Dir_800E7794 *out, void *obj, MapPos *pos);
char *func_800A7DE4(Obj_800E7794 *obj);
s32 func_800A4EFC(void *obj, void *arg);
s32 func_800A50AC(void *obj);
s32 func_800A50E8(void *obj);
}

static inline u32 cellFlags(MapPos &pos) { return func_800B1C6C(&pos); }

extern "C" s32 func_800E7794(Obj_800E7794 *obj)
{
    if ((obj->dir.y | obj->dir.x) != 0) {
        return func_800E66EC(obj);
    }
    Obj_800E7794 *target = obj->target;
    if (target == 0) {
        return func_800E66EC(obj);
    }
    MapPos targetPos(target->pos);
    MapPos cur(obj->pos);
    MapPos next;

    if (cellFlags(cur) & 0x1000) {
        Room_800E7794 *room = func_800B1F90(&cur);
        if (func_800B68B0(room) == 1) {
            u8 mode = func_800A6420(obj, target);
            if (mode != 3) {
                Dir_800E7794 toward;
                func_800A22B8(&toward, &targetPos, &cur);
                if (func_800A4CC4(obj, &cur, &toward)) {
                    Dir_800E7794 away(toward.turned(4));
                    func_800A665C(obj, &away);
                    func_800A4F58(obj, &toward);
                    return 1;
                }
                if (mode == 0) {
                    func_800B6A98(&next, room, 0);
                    obj->dir = next;
                    return func_800E66EC(obj);
                }
                Dir_800E7794 wander(toward.turned((u8)func_800C5844(D_80147620, 3, 5)));
                func_800A665C(obj, &wander);
                return 0;
            }
            func_800A6CC0(&next, obj);
            if (!(cellFlags(next) & 0x800)) {
                Dir_800E7794 facing(obj->facing);
                MapPos left;
                func_800A2594(&left, &cur, facing.turned(2));
                if (cellFlags(left) & 0x800) {
                    Dir_800E7794 turn(facing.turned(2));
                    func_800A665C(obj, &turn);
                } else {
                    MapPos right;
                    func_800A2594(&right, &cur, facing.turned(-2));
                    if (cellFlags(right) & 0x800) {
                        Dir_800E7794 turn(facing.turned(-2));
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
        Dir_800E7794 chase;
        func_800A6538(&chase, obj, &targetPos);
        Dir_800E7794 back(chase.turned(4));
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
