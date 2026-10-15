#include "common.h"
#include "map_position_iterator.h"
typedef unsigned char u8;

/* MapRect has the shared nontrivial MapPos corners. These getters retain
   the original native by-value copies rather than returning references. */
static inline MapPos start(const MapRect &r) { return r.min; }
static inline MapPos end(const MapRect &r) { return r.max; }
static inline s32 valid(const MapPositionIterator *it) { return it->cur.x <= it->end.x; }
static inline void rewind(MapPositionIterator *it) { it->cur = it->start; }
/* The original assigns a rectangle corner by corner: two 8-byte MapPos copies
   (0x800B9358..0x800B9374, 0x800B946C..0x800B9498, 0x800B9574..0x800B9590), whereas the
   implicit MapRect assignment is one 16-byte block copy. */
static inline void assign(MapRect &dst, const MapRect &src) { dst.min = src.min; dst.max = src.max; }
/* Two-corner rectangle assigned into dst: the destination index is read first (0x800B9544),
   then each corner of the stack rectangle is assigned (0x800B9548..0x800B9564). */
static inline void assign(MapRect &dst, const MapPos &a, const MapPos &b) {
    MapRect r;
    r.min = a;
    r.max = b;
    assign(dst, r);
}
struct Cell_800B91AC { u8 kind, walls, x, y; };
/* Partial foreign floor-generator view: the layout does not establish
   Map_800B91AC/MapPos/MapRect lifetime or prove grid and room indices in bounds. */
struct Map_800B91AC {
    u8 pad_00[0x10];
    /* Dimensions at +0x10 precede, rather than overlap, the +0x13 grid.
     * ROM 0x8B6C8..0x8B6D0 uses the same 32-byte row stride for both grids. */
    struct { struct { u8 width, height, wanted; } dimensions; Cell_800B91AC grid[11][8]; } map_10;
    u8 pad_173; s32 allowed_174[11][8]; u8 pad_2D4[8];
    MapRect rooms_2DC[16]; s32 room_count_3DC; u8 pad_3E0[8]; s32 extra_3E8; MapPos point_3EC;
};
extern "C" {
extern u8 D_80147620[];
extern u8 D_8014344C;
s32 func_800C5A48(void *rng, s32 minimum, s32 maximum);
s32 func_800C5844(void *rng, u8 minimum, u8 maximum);
s32 func_800B9FE0(void *object, const MapRect *bounds);
}
/* The overlap test takes the candidate rectangle by value (a native copy). */
static inline s32 overlaps(Map_800B91AC *map, MapRect r) { return func_800B9FE0(map, &r); }

/* Places random rooms until the wanted count (at least two) is reached, then
   adds the optional fixed one-cell room. */
extern "C" void func_800B91AC(Map_800B91AC *map) {
    u8 min_width = 1;
    if (map->map_10.dimensions.width >= 10) min_width = 2;
    s32 min_height = 1;
    if (map->map_10.dimensions.height >= 7) min_height = 2;
    map->room_count_3DC = 0;
    do {
        for (s32 desired = map->room_count_3DC; desired < map->map_10.dimensions.wanted; desired++) {
            for (s32 attempt = 0; attempt < 20; attempt++) {
                MapRect room;
                s32 width = func_800C5A48(D_80147620, min_width, map->map_10.dimensions.width >> 1);
                s32 height = func_800C5A48(D_80147620, (u8)min_height, map->map_10.dimensions.height >> 1);
                s32 column = (u8)func_800C5844(D_80147620, 1, map->map_10.dimensions.width - width + 1);
                s32 row = (u8)func_800C5844(D_80147620, 1, map->map_10.dimensions.height - height + 1);
                /* Not named `valid`: that is the iterator test called below. */
                s32 allowed = 1;
                for (s32 x = column; x <= column + (u8)width - 1; x++) {
                    for (s32 y = row; y <= row + (u8)height - 1; y++) {
                        if (map->allowed_174[x][y] == 0) { allowed = 0; break; }
                    }
                    if (!allowed) break;
                }
                if (!allowed) continue;
                {
                    /* Corners are stored directly (0x800B9348..0x800B9354), with no
                       memset: a brace-initialized MapRect is cleared first by g++ 2.8.1. */
                    MapRect bounds;
                    bounds.min.set(row, column);
                    bounds.max.set(row + (u8)height - 1, column + (u8)width - 1);
                    assign(room, bounds);
                }
                if ((overlaps(map, room) ^ 1) == 0) continue;
                MapPositionIterator it;
                it.cur = it.start = start(room);
                it.end = end(room);
                s32 clear = 1;
                while (valid(&it)) {
                    MapPos p;
                    func_800A3610(&p, &it);
                    if (map->map_10.grid[p.y][p.x].kind != 0x10) { clear = 0; break; }
                }
                if (!clear) continue;
                assign(map->rooms_2DC[map->room_count_3DC], room);
                rewind(&it);
                while (valid(&it)) {
                    MapPos p;
                    func_800A3610(&p, &it);
                    map->map_10.grid[p.y][p.x].kind = (u8)map->room_count_3DC | 0x20;
                }
                map->room_count_3DC++;
                break;
            }
        }
    } while (map->room_count_3DC < 2);
    if (map->extra_3E8) {
        map->map_10.grid[map->point_3EC.y][map->point_3EC.x].kind = (u8)map->room_count_3DC | 0x20;
        /* The original two-point constructor assigns each corner. */
        assign(map->rooms_2DC[map->room_count_3DC], map->point_3EC, map->point_3EC);
        map->room_count_3DC++;
    }
    D_8014344C = map->room_count_3DC;
}
