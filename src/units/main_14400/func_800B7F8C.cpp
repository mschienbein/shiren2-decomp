#include "common.h"

/*
 * L-shaped corridor carver (C++ TU). Each leg passes a Point by value
 * (user copy constructor, so g++ builds a temporary and passes its address)
 * and a Direction by const reference (one temporary per call).
 */
typedef unsigned char u8;

struct Point {
    s32 x;
    s32 y;
    Point(s32 px, s32 py) : x(px), y(py) {}
    Point(const Point &o) : x(o.x), y(o.y) {}
    Point transposed() const { return Point(y, x); }
};

struct Direction {
    u8 value;
    Direction(s32 v) : value(v) {}
};

extern "C" {
extern u8 D_80147620[];
s32 func_800C5A48(void *rng, s32 low, s32 high);
void func_800B8138(void *object, Point *position, u8 *direction, s32 count);
}

/* Carves `count` cells from a by-value start point (the callee advances its copy). */
static inline void carve(void *map, Point start, const Direction &dir, s32 count)
{
    func_800B8138(map, &start, (u8 *)&dir.value, count);
}

/* map: generator object forwarded to func_800B8138. */
extern "C" void func_800B7F8C(void *map, Point *from, Point *to, s32 vertical)
{
    s32 mid = (u8)func_800C5A48(D_80147620, (u8)(from->y + 1), (u8)(to->y - 1));
    s32 count;

    if (vertical) {
        carve(map, from->transposed(), Direction(6), mid - from->y + 1);
    } else {
        carve(map, *from, Direction(0), mid - from->y + 1);
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
        carve(map, from->transposed(), Direction(0), count);
    } else {
        carve(map, *from, Direction(6), count);
    }
    if (vertical) {
        carve(map, to->transposed(), Direction(2), to->y - mid);
    } else {
        carve(map, *to, Direction(4), to->y - mid);
    }
}
