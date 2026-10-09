#include "common.h"
typedef unsigned char u8;

/* Grid coordinate and rectangle value types of the floor generator (g++
   module 0x800B7948..0x800C2D3C): copy construction is member-wise, plain
   assignment is the implicit bitwise copy. */
struct Pair {
    s32 row, column;
    Pair() {}
    Pair(s32 r, s32 c) : row(r), column(c) {}
    Pair(const Pair &p) : row(p.row), column(p.column) {}
};
struct Rect {
    Pair first, last;
    Rect() {}
    Rect(s32 r0, s32 c0, s32 r1, s32 c1) : first(r0, c0), last(r1, c1) {}
    Rect(const Pair &a, const Pair &b) { first = a; last = b; }
    Rect(const Rect &r) : first(r.first), last(r.last) {}
    Rect &operator=(const Rect &r) { first = r.first; last = r.last; return *this; }
    Pair start() const { return first; }
    Pair end() const { return last; }
};
/* Row-major walk over a rectangle; func_800A3610 yields the next cell. */
struct Iterator {
    Pair current, first, last;
    s32 valid() const { return current.row <= last.row; }
    void rewind() { current = first; }
};
struct Cell { u8 kind, walls, x, y; };
struct Map {
    u8 pad_00[0x10];
    /* Dimensions at +0x10 precede, rather than overlap, the +0x13 grid.
     * ROM 0x8B6C8..0x8B6D0 uses the same 32-byte row stride for both grids. */
    struct { struct { u8 width, height, wanted; } dimensions; Cell grid[11][8]; } map_10;
    u8 pad_173; s32 allowed_174[11][8]; u8 pad_2D4[8];
    Rect rooms_2DC[16]; s32 room_count_3DC; u8 pad_3E0[8]; s32 extra_3E8; Pair point_3EC;
};
extern "C" {
extern u8 D_80147620[];
extern u8 D_8014344C;
s32 func_800C5A48(void *rng, s32 minimum, s32 maximum);
s32 func_800C5844(void *rng, u8 minimum, u8 maximum);
s32 func_800B9FE0(Map *map, void *rectangle);
Pair *func_800A3610(Pair *point, Iterator *iterator);
}
/* The overlap test takes the candidate rectangle by value (a native copy). */
static inline s32 overlaps(Map *map, Rect r) { return func_800B9FE0(map, &r); }

/* Places random rooms until the wanted count (at least two) is reached, then
   adds the optional fixed one-cell room. */
extern "C" void func_800B91AC(Map *map) {
    u8 min_width = 1;
    if (map->map_10.dimensions.width >= 10) min_width = 2;
    s32 min_height = 1;
    if (map->map_10.dimensions.height >= 7) min_height = 2;
    map->room_count_3DC = 0;
    do {
        for (s32 desired = map->room_count_3DC; desired < map->map_10.dimensions.wanted; desired++) {
            for (s32 attempt = 0; attempt < 20; attempt++) {
                Rect room;
                s32 width = func_800C5A48(D_80147620, min_width, map->map_10.dimensions.width >> 1);
                s32 height = func_800C5A48(D_80147620, (u8)min_height, map->map_10.dimensions.height >> 1);
                s32 column = (u8)func_800C5844(D_80147620, 1, map->map_10.dimensions.width - width + 1);
                s32 row = (u8)func_800C5844(D_80147620, 1, map->map_10.dimensions.height - height + 1);
                s32 valid = 1;
                for (s32 x = column; x <= column + (u8)width - 1; x++) {
                    for (s32 y = row; y <= row + (u8)height - 1; y++) {
                        if (map->allowed_174[x][y] == 0) { valid = 0; break; }
                    }
                    if (!valid) break;
                }
                if (!valid) continue;
                room = Rect(row, column, row + (u8)height - 1, column + (u8)width - 1);
                if ((overlaps(map, room) ^ 1) == 0) continue;
                Iterator it;
                it.current = it.first = room.start();
                it.last = room.end();
                s32 clear = 1;
                while (it.valid()) {
                    Pair p;
                    func_800A3610(&p, &it);
                    if (map->map_10.grid[p.column][p.row].kind != 0x10) { clear = 0; break; }
                }
                if (!clear) continue;
                map->rooms_2DC[map->room_count_3DC] = room;
                it.rewind();
                while (it.valid()) {
                    Pair p;
                    func_800A3610(&p, &it);
                    map->map_10.grid[p.column][p.row].kind = (u8)map->room_count_3DC | 0x20;
                }
                map->room_count_3DC++;
                break;
            }
        }
    } while (map->room_count_3DC < 2);
    if (map->extra_3E8) {
        map->map_10.grid[map->point_3EC.column][map->point_3EC.row].kind = (u8)map->room_count_3DC | 0x20;
        map->rooms_2DC[map->room_count_3DC] = Rect(map->point_3EC, map->point_3EC);
        map->room_count_3DC++;
    }
    D_8014344C = map->room_count_3DC;
}
