#include "map_rect.h"

/*
 * Straight corridor carver with random jogs (C++ TU). Each leg passes a MapPos
 * by value (user copy constructor: g++ builds a temporary and passes its
 * address) and a Direction by const reference.
 */
typedef unsigned char u8;
typedef unsigned short u16;


struct Direction_800B7A4C {
    u8 value;
    Direction_800B7A4C() {}
    Direction_800B7A4C(s32 v) : value(v & 7) {}
};

/* Corridor span is the caller's shared MapRect with genuine MapPos corners. */

extern "C" {
extern u8 D_80147620[];
s32 func_800C5954(void *rng, u16 min, u16 max);
s32 func_800C5844(void *rng, u8 base, u8 top);
void func_800B8138(void *object, MapPos *position, u8 *direction, s32 count);
}

/* Carves `count` cells from a by-value start point (the callee advances its copy). */
static inline void carve(void *obj, MapPos start, const Direction_800B7A4C &dir, s32 count)
{
    func_800B8138(obj, &start, (u8 *)&dir.value, count);
}


/* The generator receiver is forwarded unchanged to each carve operation. */
extern "C" void func_800B7A4C(void *owner, MapRect *area, s32 vertical)
{
    s32 along = area->min.y;
    s32 end = area->max.y;
    s32 cross = (u16)func_800C5954(D_80147620, area->min.x, area->max.x);

    s32 n;
    while (end - along >= 4) {
        n = (u8)func_800C5844(D_80147620, 3, end - along + 1);
        if (vertical) {
            carve(owner, MapPos(along, cross), Direction_800B7A4C(6), n);
        } else {
            carve(owner, MapPos(cross, along), Direction_800B7A4C(0), n);
        }
        along += n - 1;
        if (end - along >= 2) {
            n = (u16)func_800C5954(D_80147620, area->min.x, area->max.x);
            MapPos point;
            Direction_800B7A4C turn;
            if (vertical) {
                point = MapPos(along, cross);
                turn = Direction_800B7A4C((cross >= n) * 4);
            } else {
                point = MapPos(cross, along);
                turn = Direction_800B7A4C(cross < n ? 6 : 2);
            }
            s32 span;
            if (cross < n) {
                span = n - cross + 1;
            } else {
                span = cross - n + 1;
            }
            carve(owner, point, turn, span);
            cross = n;
        }
    }
    if (vertical) {
        carve(owner, MapPos(along, cross), Direction_800B7A4C(6), end - along + 1);
    } else {
        carve(owner, MapPos(cross, along), Direction_800B7A4C(0), end - along + 1);
    }
}
