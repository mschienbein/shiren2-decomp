#include "common.h"
typedef unsigned char u8;

struct Point {
    s32 x, y;
    Point() {}
    Point(s32 px, s32 py) : x(px), y(py) {}
    Point(const Point &p) : x(p.x), y(p.y) {}
};
struct Box {
    Point lo, hi;
    Box() {}
    Box(const Box &b) : lo(b.lo), hi(b.hi) {}
    Point lower() const { return lo; }
    Point upper() const { return hi; }
};
struct Room { Box bounds; u8 tag[4]; };
struct Map {
    u8 pad0[0x3DC];
    s32 count;
    u8 pad3E0[0x404 - 0x3E0];
    Room rooms[17];
    s32 links[16][16];
};
extern "C" {
s32 func_800A3308(Room *a, Room *b);
Box *func_800A324C(Box *out, Room *a, Room *b);
void func_800A3180(Box *box);
Point *func_800A256C(Point *out, Point *a, Point *b);
Point *func_800A2544(Point *out, Point *a, Point *b);
s32 func_800C00B4(Map *map, Box *box);
}
/* ODD_C: retain the two-axis emptiness predicate as a materialized result. */
static inline s32 box_empty(Box *b) {
    s32 result = 0;
    s32 rows = b->lo.y <= b->hi.y;
    if (!rows) result = b->lo.x > b->hi.x;
    return result;
}
/* The overlap test takes the rectangle by value (a native copy). */
static inline s32 check(Map *map, Box b) { return func_800C00B4(map, &b); }
static inline Room *room_at(Map *map, s32 index) {
    /* local-arithmetic-qualification: &map->rooms[index] folds 0x404 into
       the index, and pointer arithmetic always places the object base first;
       the original adds the room offset to the object base, then adds the
       rooms[] offset as a separate step. The result denotes one complete
       room in map->rooms[17]. */
    s32 address = index * (s32)sizeof(Room) + (s32)map;
    address += 0x404;
    return (Room *)address;
}
/* Links every pair of rooms; a pair whose shared rectangle, grown by one
   tile on each side, fails the overlap test gets an extra corridor count. */
extern "C" void func_800C0304(Map *map) {
    for (u8 i = 0; i < map->count - 1; i++) {
        Box box;
        for (u8 j = i + 1; j < map->count; j++) {
            map->links[i][j] = func_800A3308(room_at(map, i), room_at(map, j));
            map->links[j][i] = 0;
            func_800A324C(&box, &map->rooms[i], &map->rooms[j]);
            if (box_empty(&box)) continue;
            func_800A3180(&box);
            /* The corner operations take their operands by address; each
               operand is a scoped value copy (corner getter, unit offset). */
            Point lo;
            {
                Point corner = box.lower();
                Point one(1, 1);
                func_800A256C(&lo, &corner, &one);
            }
            Point hi;
            {
                Point corner = box.upper();
                Point one(1, 1);
                func_800A2544(&hi, &corner, &one);
            }
            box.lo = lo;
            box.hi = hi;
            if ((check(map, box) ^ 1) != 0) map->links[j][i]++;
        }
        map->links[i][i] = 0;
    }
    map->links[map->count - 1][map->count - 1] = 0;
}
