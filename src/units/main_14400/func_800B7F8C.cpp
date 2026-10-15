#include "map_types.h"

/*
 * L-shaped corridor carver (C++ TU). Each leg passes a MapPos by value
 * (user copy constructor, so g++ builds a temporary and passes its address)
 * and a Direction by const reference (one temporary per call).
 */
typedef unsigned char u8;


struct Direction_800B7F8C {
    u8 value;
    Direction_800B7F8C(s32 v) : value(v) {}
};

extern "C" {
extern u8 D_80147620[];
s32 func_800C5A48(void *rng, s32 low, s32 high);
void func_800B8138(void *object, MapPos *position, u8 *direction, s32 count);
}

/* Carves `count` cells from a by-value start point (the callee advances its copy). */
static inline void carve(void *map, MapPos start, const Direction_800B7F8C &dir, s32 count)
{
    func_800B8138(map, &start, (u8 *)&dir.value, count);
}

/* map: generator object forwarded to func_800B8138. */
extern "C" void func_800B7F8C(void *map, MapPos *from, MapPos *to, s32 vertical)
{
    s32 mid = (u8)func_800C5A48(D_80147620, (u8)(from->y + 1), (u8)(to->y - 1));
    s32 count;

    if (vertical) {
        carve(map, from->transposed(), Direction_800B7F8C(6), mid - from->y + 1);
    } else {
        carve(map, *from, Direction_800B7F8C(0), mid - from->y + 1);
    }
    from->y = mid;
    if (from->x > to->x) {
        count = from->x - to->x;
        from->x = to->x;
    } else {
        count = to->x - from->x;
        from->x = from->x + 1;
    }
    if (vertical) {
        carve(map, from->transposed(), Direction_800B7F8C(0), count);
    } else {
        carve(map, *from, Direction_800B7F8C(6), count);
    }
    if (vertical) {
        carve(map, to->transposed(), Direction_800B7F8C(2), to->y - mid);
    } else {
        carve(map, *to, Direction_800B7F8C(4), to->y - mid);
    }
}
