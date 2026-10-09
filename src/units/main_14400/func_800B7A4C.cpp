#include "common.h"

/*
 * Straight corridor carver with random jogs (C++ TU). Each leg passes a Point
 * by value (user copy constructor: g++ builds a temporary and passes its
 * address) and a Direction by const reference.
 */
typedef unsigned char u8;
typedef unsigned short u16;

struct Point {
    s32 x;
    s32 y;
    Point() {}
    Point(s32 px, s32 py) : x(px), y(py) {}
    Point(const Point &o) : x(o.x), y(o.y) {}
};

struct Direction {
    u8 value;
    Direction() {}
    Direction(s32 v) : value(v & 7) {}
};

/* Corridor span: the caller's Rect (four s32 corner coordinates). */
struct Rect {
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
};

extern "C" {
extern u8 D_80147620[];
s32 func_800C5954(void *rng, u16 min, u16 max);
s32 func_800C5844(void *rng, u8 base, u8 top);
void func_800B8138(void *object, Point *position, u8 *direction, s32 count);
}

/* Carves `count` cells from a by-value start point (the callee advances its copy). */
static inline void carve(void *obj, Point start, const Direction &dir, s32 count)
{
    func_800B8138(obj, &start, (u8 *)&dir.value, count);
}


/* The generator receiver is forwarded unchanged to each carve operation. */
extern "C" void func_800B7A4C(void *owner, Rect *area, s32 vertical)
{
    s32 along = area->y0;
    s32 end = area->y1;
    s32 cross = (u16)func_800C5954(D_80147620, area->x0, area->x1);

    s32 n;
    while (end - along >= 4) {
        n = (u8)func_800C5844(D_80147620, 3, end - along + 1);
        if (vertical) {
            carve(owner, Point(along, cross), Direction(6), n);
        } else {
            carve(owner, Point(cross, along), Direction(0), n);
        }
        along += n - 1;
        if (end - along >= 2) {
            n = (u16)func_800C5954(D_80147620, area->x0, area->x1);
            Point point;
            Direction turn;
            if (vertical) {
                point = Point(along, cross);
                turn = Direction((cross >= n) * 4);
            } else {
                point = Point(cross, along);
                turn = Direction(cross < n ? 6 : 2);
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
        carve(owner, Point(along, cross), Direction(6), end - along + 1);
    } else {
        carve(owner, Point(cross, along), Direction(0), end - along + 1);
    }
}
