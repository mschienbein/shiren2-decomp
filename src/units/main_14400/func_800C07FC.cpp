#include "common.h"
#include "map_rect.h"

typedef unsigned char u8;

/*
 * Floor room painter/corridor pass (g++ 2.8.1 TU). Each room record is copied
 * by value (its implicit copy constructor copies tag[] element-wise), then its
 * shared MapPos corners are assigned, coordinate by coordinate, into an actual
 * MapRect for the painter. The original forms the first corner's source pointer
 * before the destination, and the second corner's destination (a corner
 * reference) before its source.
 */

/* 0x14-byte room record: shared bounds rectangle plus four tag bytes. */
struct Room_800C07FC {
    MapRect rect;
    u8 tag[4];
    const MapPos &first() const { return rect.min; }
    const MapPos &last() const { return rect.max; }
};

/* Partial view of the floor generator object. */
struct Floor_800C07FC {
    u8 pad[0x3DC];
    s32 count;
    u8 pad3E0[0x24];
    Room_800C07FC rooms[17];
    s32 links[16][16];
};

/* Corner assignment, x then y. The original copies each corner coordinate by
   coordinate through hoisted corner pointers (0x800C08A0..0x800C08D0: x via sp,
   y via s7 -> s2 and s5 -> s6); the implicit MapPos assignment is instead one
   8-byte block copy (two loads, then two stores). */
static inline void assign(MapPos &dst, const MapPos &src)
{
    dst.x = src.x;
    dst.y = src.y;
}

extern "C" void func_800C07FC(Floor_800C07FC *floor)
{
    for (s32 i = 0; i < floor->count; i++) {
        MapRect bounds;
        Room_800C07FC room = floor->rooms[i];
        assign(bounds.min, room.first());
        MapPos &corner = bounds.max;
        assign(corner, room.last());
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
