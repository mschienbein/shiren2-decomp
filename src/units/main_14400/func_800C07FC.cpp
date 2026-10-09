#include "common.h"

typedef unsigned char u8;

/*
 * Floor room painter/corridor pass (g++ 2.8.1 TU). Each room record is copied
 * by value (its implicit copy constructor copies tag[] element-wise), then its
 * corners are assigned into a bounds rectangle that is passed to the painter.
 * The original forms the first corner's source pointer before the
 * destination, and the second corner's destination (a corner reference)
 * before its source.
 */

struct Point {
    s32 x;
    s32 y;
    Point() {}
    Point(const Point &other) : x(other.x), y(other.y) {}
    Point &operator=(const Point &other) { x = other.x; y = other.y; return *this; }
};

struct Rect {
    Point a;
    Point b;
};

/* 0x14-byte room record: bounds rectangle plus four tag bytes. */
struct Room {
    Rect rect;
    u8 tag[4];
    const Point &first() const { return rect.a; }
    const Point &last() const { return rect.b; }
};

/* Partial view of the floor generator object. */
struct Floor {
    u8 pad[0x3DC];
    s32 count;
    u8 pad3E0[0x24];
    Room rooms[17];
    s32 links[16][16];
};

extern "C" {
void func_800B7948(void *self, s32 index, Rect *rect);
void func_800B7E9C(void *arg0, const Rect *arg1, const Rect *arg2);
}

extern "C" void func_800C07FC(Floor *floor)
{
    for (s32 i = 0; i < floor->count; i++) {
        Rect bounds;
        Room room = floor->rooms[i];
        bounds.a = room.first();
        Point &corner = bounds.b;
        corner = room.last();
        func_800B7948(floor, i, &bounds);
    }
    for (s32 i = 0; i < floor->count - 1; i++) {
        for (s32 j = i + 1; j < floor->count; j++) {
            if (floor->links[j][i] == 2) {
                func_800B7E9C(floor, &floor->rooms[i].rect, &floor->rooms[j].rect);
            }
        }
    }
}
