#include "common.h"
#include "map_rect.h"
typedef unsigned char u8;

struct Room_800C0304 { MapRect bounds; u8 tag[4]; };
struct Map_800C0304 {
    u8 pad0[0x3DC];
    s32 count;
    u8 pad3E0[0x404 - 0x3E0];
    Room_800C0304 rooms[17];
    s32 links[16][16];
};
extern "C" {
void func_800A3180(MapRect *box);
MapPos *func_800A256C(MapPos *out, MapPos *a, MapPos *b);
MapPos *func_800A2544(MapPos *out, MapPos *a, MapPos *b);
s32 func_800C00B4(void *map, const MapRect *box);
}
/* The corner getters return native nontrivial MapPos values, not references. */
static inline MapPos lower(const MapRect &b) { return b.min; }
static inline MapPos upper(const MapRect &b) { return b.max; }
/* ODD_C: retain the two-axis emptiness predicate as a materialized result. */
static inline s32 box_empty(MapRect *b) {
    s32 result = 0;
    s32 rows = b->min.y <= b->max.y;
    if (!rows) result = b->min.x > b->max.x;
    return result;
}
/* The overlap test takes the rectangle by value (a native copy). */
static inline s32 check(Map_800C0304 *map, MapRect b) { return func_800C00B4(map, &b); }
static inline Room_800C0304 *room_at(Map_800C0304 *map, s32 index) {
    /* local-arithmetic-qualification: &map->rooms[index] folds 0x404 into
       the index, and pointer arithmetic always places the object base first;
       the original adds the room offset to the object base, then adds the
       rooms[] offset as a separate step. The result denotes one complete
       room in map->rooms[17]. */
    s32 address = index * (s32)sizeof(Room_800C0304) + (s32)map;
    address += 0x404;
    return (Room_800C0304 *)address;
}
/* Links every pair of rooms; a pair whose shared rectangle, grown by one
   tile on each side, fails the overlap test gets an extra corridor count. */
extern "C" void func_800C0304(Map_800C0304 *map) {
    for (u8 i = 0; i < map->count - 1; i++) {
        MapRect box;
        for (u8 j = i + 1; j < map->count; j++) {
            map->links[i][j] = func_800A3308(&room_at(map, i)->bounds, &room_at(map, j)->bounds);
            map->links[j][i] = 0;
            func_800A324C(&box, &map->rooms[i].bounds, &map->rooms[j].bounds);
            if (box_empty(&box)) continue;
            func_800A3180(&box);
            /* The corner operations take their operands by address; each
               operand is a scoped value copy (corner getter, unit offset). */
            MapPos lo;
            {
                MapPos corner = lower(box);
                MapPos one(1, 1);
                func_800A256C(&lo, &corner, &one);
            }
            MapPos hi;
            {
                MapPos corner = upper(box);
                MapPos one(1, 1);
                func_800A2544(&hi, &corner, &one);
            }
            box.min = lo;
            box.max = hi;
            if ((check(map, box) ^ 1) != 0) map->links[j][i]++;
        }
        map->links[i][i] = 0;
    }
    map->links[map->count - 1][map->count - 1] = 0;
}
