#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Point;
typedef struct { Point min, max; } Rect;
typedef struct { u8 value; } Dir;
typedef struct { u8 pad_00[0x20]; signed short delta_20; u16 reserved_22; s32 (*count_24)(void *); } ListVtable;
typedef struct { void *table_00; ListVtable *vtable_04; } List;
typedef struct Unit Unit;
struct Unit {
    Point position;
    u8 direction_08, area_09;
    u8 pad_0A[2];
    Rect bounds_0C;
    u16 flags_1C;
    u8 flags_1E;
    u8 pad_1F[0x35];
    u8 flags_54;
    u8 pad_55[3];
    Unit *target_58;
    u8 pad_5C[8];
    Point destination_64;
    u8 pad_6C[6];
    u8 flags_72;
    u8 pad_73[0x19];
    List *list_8C;
    u8 pad_90[0xA];
    u16 flags_9A;
    u8 pad_9C[0x20];
    s32 pursuing_BC;
};
extern Unit *D_801476B8;
extern s32 func_800E7794(Unit *unit);
extern s32 func_800FA710(Unit *unit, Point *position);
extern s32 func_800FA1FC(Unit *unit, Unit *target);
extern Point *func_800B50D4(Point *out, u8 direction);
extern s32 func_800A24DC(void *point, Rect *bounds);
extern Point *func_800FA760(Point *out, Unit *unit);
extern s32 func_800E7104(Unit *unit);
extern s32 func_800A23E8(Point *from, Point *to);
extern void *func_800A27A4(void *out, void *from, void *to);
extern s32 func_800A4360(void *unit, void *position);
extern s32 func_800A4754(Unit *unit, void *position, Dir *direction);
extern void func_800A665C(Unit *unit, u8 *direction);
extern s32 func_800E66EC(Unit *unit);

static inline void point_copy(Point *out, Point *point)
{
    out->x = point->x;
    out->y = point->y;
}

s32 func_800FAA80(Unit *unit)
{
    Point here;
    Rect bounds;
    Point destination;
    Point temporary;
    Dir direction;
    List *list = unit->list_8C;
    ListVtable *vtable;
    Unit *target;
    s32 i;
    unit->flags_72 &= ~2;
    vtable = list->vtable_04;
    if (vtable->count_24((char *)list + vtable->delta_20) != 0) {
        if (!(unit->flags_9A & 0x40)) {
            unit->flags_72 |= 2;
            return func_800E7794(unit);
        }
        goto move;
    }
    point_copy(&here, &unit->position);
    if (func_800FA710(unit, &here)) return 0;
    target = unit->target_58;
    if (unit->pursuing_BC != 0) {
        if (target != 0 && func_800FA1FC(unit, target)) return func_800E7104(unit);
        unit->pursuing_BC = 0;
    }
    point_copy(&bounds.min, &unit->bounds_0C.min);
    point_copy(&bounds.max, &unit->bounds_0C.max);
    for (i = 0;; ++i) {
        s32 in_bounds;
        if (i >= 4) break;
        func_800B50D4(&destination, i);
        in_bounds = 0;
        if ((destination.y | destination.x) != 0) {
            in_bounds = func_800A24DC(&destination, &bounds) != 0;
        }
        if (in_bounds && func_800FA710(unit, &destination)) {
            unit->destination_64 = destination;
            break;
        }
    }
    point_copy(&destination, &unit->destination_64);
    switch (func_800FA710(unit, &destination) ^ 1) {
        case 0: break;
        default:
        func_800FA760(&temporary, unit);
        destination = temporary;
        if ((destination.y | destination.x) == 0) {
            s32 retarget = 0;
            if (target != 0 && func_800FA1FC(unit, target) == 0) {
                u8 flag = (target->flags_1E >> 1) & 1;
                retarget = flag == 0;
            }
            if (retarget) {
                if (unit->flags_9A & 0x40) unit->target_58 = 0;
                else unit->target_58 = D_801476B8;
            }
            return func_800E7104(unit);
        }
        unit->destination_64 = destination;
        break;
    }
    unit->flags_1C |= 0x200;
    point_copy(&temporary, &destination);
    if (func_800A23E8(&here, &temporary) == 1) {
        s32 can_move = 0;
        point_copy(&temporary, &destination);
        func_800A27A4(&direction, &here, &temporary);
        if (func_800A4360(unit, &destination) == (unit->area_09 & 0xF)) {
            can_move = func_800A4754(unit, &here, &direction) != 0;
        }
        if (can_move) {
            unit->target_58 = 0;
            unit->flags_54 |= 4;
            func_800A665C(unit, &direction.value);
            return 0;
        }
    }
move:
    return func_800E66EC(unit);
}
