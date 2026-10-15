#include "common.h"
#include "map_rect.h"
typedef struct { MapRect bounds; unsigned char side_counts[4]; } Room;
extern u32 func_800B1C6C(MapPos *);
static inline s32 horizontal_edge(Room *room, MapPos *point) {
    return point->y == room->bounds.min.y - 1 || point->y == room->bounds.max.y + 1;
}
static inline s32 horizontal_range(Room *room, MapPos *point) {
    return point->x >= room->bounds.min.x - 1 && point->x <= room->bounds.max.x + 1;
}
static inline s32 vertical_edge(Room *room, MapPos *point) {
    return point->x == room->bounds.min.x - 1 || point->x == room->bounds.max.x + 1;
}
static inline s32 vertical_range(Room *room, MapPos *point) {
    return point->y >= room->bounds.min.y - 1 && point->y <= room->bounds.max.y + 1;
}
s32 func_800B68F0(Room *room, MapPos *point) {
    if (!(func_800B1C6C(point) & 0x800)) return 0;
    if (horizontal_edge(room, point)) {
        if (horizontal_range(room, point)) return 1;
    }
    if (vertical_edge(room, point)) {
        if (vertical_range(room, point)) return 1;
    }
    return 0;
}
