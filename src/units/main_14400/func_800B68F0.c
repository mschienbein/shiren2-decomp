#include "common.h"
typedef struct { s32 x; s32 y; } Point;
typedef struct { s32 x0; s32 y0; s32 x1; s32 y1; } Room;
extern u32 func_800B1C6C(Point *);
static inline s32 horizontal_edge(Room *room, Point *point) {
    return point->y == room->y0 - 1 || point->y == room->y1 + 1;
}
static inline s32 horizontal_range(Room *room, Point *point) {
    return point->x >= room->x0 - 1 && point->x <= room->x1 + 1;
}
static inline s32 vertical_edge(Room *room, Point *point) {
    return point->x == room->x0 - 1 || point->x == room->x1 + 1;
}
static inline s32 vertical_range(Room *room, Point *point) {
    return point->y >= room->y0 - 1 && point->y <= room->y1 + 1;
}
s32 func_800B68F0(Room *room, Point *point) {
    if (!(func_800B1C6C(point) & 0x800)) return 0;
    if (horizontal_edge(room, point)) {
        if (horizontal_range(room, point)) return 1;
    }
    if (vertical_edge(room, point)) {
        if (vertical_range(room, point)) return 1;
    }
    return 0;
}
