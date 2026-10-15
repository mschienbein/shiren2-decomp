#include "common.h"
#include "map_types.h"

/*
 * Corridor linker of the grid-room floor generator (C++ TU, g++ module
 * 0x800B7948..0x800C2D3C). For grid cell (x, y) it joins the cell to its
 * neighbour (x + 1, y) when wall bit 0 is open and to (x, y + 1) when wall
 * bit 3 is open, unless both cells belong to the same room. Each side is
 * either a room record (D_801431F0, a random point on the facing edge,
 * clamped to the grid line spacing) or a junction cell's stored position.
 * Coordinates are kept in the corridor-local frame used by func_800B7F8C
 * (x across, y along); the vertical link transposes them when carving.
 * As in func_800B7F8C, each carve passes a MapPos by value (user copy
 * constructor: one reused temporary at sp+0x20) and a direction by const
 * reference (one byte temporary per call site, sp+0x28..0x2D).
 */

typedef unsigned char u8;
typedef signed char s8;

/* One-byte direction, same storage as MapDir (map_types.h); a distinct class because its
   constructor narrows without masking to 0..7. */
struct Direction_800B98F4 {
    u8 value;
    Direction_800B98F4(s32 v) : value(v) {}
};

/* 0x14-byte room record of D_801431F0 viewed as two corners (func_800D37E8's Room_800D37E8
   views the same 16 bytes as one rectangle). */
struct Room_800B98F4 {
    MapPos min;
    MapPos max;
    u8 pad10[4];
};

/* Generator grid cell (same 4 bytes as the floor generator's cells[11][8] at +0x13). */
struct Cell_800B98F4 { u8 kind, walls, x, y; };

/* Partial view of the 0x9CC-byte floor generator (layout of func_800BA558.cpp). */
struct MapView_800B98F4 {
    char pad0[0x13];
    Cell_800B98F4 cells[11][8];
    char pad173[0x2D4 - 0x173];
    s8 *row_edges;
    s8 *col_edges;
};

extern "C" {
extern Room_800B98F4 D_801431F0[];
extern u8 D_80147620[];
s32 func_800C5844(void *rng, u8 base, u8 top);
void func_800B8138(void *object, MapPos *position, u8 *direction, s32 count);
void func_800B7F8C(void *map, MapPos *from, MapPos *to, s32 vertical);
}

/* Clamp helpers: lo = max(grid line + 10, room edge), hi = min(next line + 9, room edge). */
static inline s32 max(s32 a, s32 b) { return a < b ? b : a; }
static inline s32 min(s32 a, s32 b) { return a < b ? a : b; }

static inline void carve(MapView_800B98F4 *map, MapPos start, const Direction_800B98F4 &dir, s32 count)
{
    func_800B8138(map, &start, (u8 *)&dir.value, count);
}

static inline void bend(MapView_800B98F4 *map, MapPos from, MapPos *to, s32 vertical)
{
    func_800B7F8C(map, &from, to, vertical);
}

extern "C" void func_800B98F4(MapView_800B98F4 *map, u8 x, u8 y)
{
    if (map->cells[x][y].walls & 1) {
        u8 a = map->cells[x][y].kind;
        u8 b = map->cells[x + 1][y].kind;
        if (!((a & b & 0x20) && (a & 0xF) == (b & 0xF))) {
            MapPos from, to;
            s32 has_from = 0, from_lo = 0, from_hi = 0;
            if (a & 0x20) {
                Room_800B98F4 *room = &D_801431F0[a & 0xF];
                has_from = 1;
                from.y = room->max.y + 1;
                from_lo = room->min.x;
                from_hi = room->max.x;
                s32 lo = max(map->col_edges[y - 1] + 10, from_lo);
                s32 hi = min(map->col_edges[y] + 9, from_hi);
                from.x = (u8)func_800C5844(D_80147620, lo, hi);
            } else {
                from.y = map->cells[x][y].x;
                from.x = map->cells[x][y].y;
            }
            s32 has_to = 0, to_lo = 0, to_hi = 0;
            if (map->cells[x + 1][y].kind & 0x20) {
                Room_800B98F4 *room = &D_801431F0[map->cells[x + 1][y].kind & 0xF];
                has_to = 1;
                to.y = room->min.y - 1;
                to_lo = room->min.x;
                to_hi = room->max.x;
                s32 lo = max(map->col_edges[y - 1] + 10, to_lo);
                s32 hi = min(map->col_edges[y] + 9, to_hi);
                to.x = (u8)func_800C5844(D_80147620, lo, hi);
            } else {
                to.y = map->cells[x + 1][y].x;
                to.x = map->cells[x + 1][y].y;
            }
            s32 length = to.y - from.y + 1;
            if (has_from && has_to && length < 3) {
                s32 lo = max(from_lo, to_lo);
                s32 hi = min(from_hi, to_hi);
                if (lo <= hi) {
                    from.x = (u8)func_800C5844(D_80147620, lo, hi);
                    carve(map, from, Direction_800B98F4(0), length);
                } else if (from_hi < to_lo) {
                    from.x = from_hi;
                    for (s32 i = 0; i < length; i++, from.y++) {
                        carve(map, from, Direction_800B98F4(6), to_lo - from_hi + 1);
                    }
                } else {
                    from.x = to_hi;
                    for (s32 i = 0; i < length; i++, from.y++) {
                        carve(map, from, Direction_800B98F4(6), from_lo - to_hi + 1);
                    }
                }
            } else {
                bend(map, from, &to, 0);
            }
        }
    }
    if (map->cells[x][y].walls & 8) {
        u8 a = map->cells[x][y].kind;
        u8 b = map->cells[x][y + 1].kind;
        if (!((a & b & 0x20) && (a & 0xF) == (b & 0xF))) {
            MapPos from, to;
            s32 has_from = 0, from_lo = 0, from_hi = 0;
            if (a & 0x20) {
                Room_800B98F4 *room = &D_801431F0[a & 0xF];
                has_from = 1;
                from.y = room->max.x + 1;
                from_lo = room->min.y;
                from_hi = room->max.y;
                s32 lo = max(map->row_edges[x - 1] + 10, from_lo);
                s32 hi = min(map->row_edges[x] + 9, from_hi);
                from.x = (u8)func_800C5844(D_80147620, lo, hi);
            } else {
                from.x = map->cells[x][y].x;
                from.y = map->cells[x][y].y;
            }
            s32 has_to = 0, to_lo = 0, to_hi = 0;
            if (map->cells[x][y + 1].kind & 0x20) {
                Room_800B98F4 *room = &D_801431F0[map->cells[x][y + 1].kind & 0xF];
                has_to = 1;
                to.y = room->min.x - 1;
                to_lo = room->min.y;
                to_hi = room->max.y;
                s32 lo = max(map->row_edges[x - 1] + 10, to_lo);
                s32 hi = min(map->row_edges[x] + 9, to_hi);
                to.x = (u8)func_800C5844(D_80147620, lo, hi);
            } else {
                to.x = map->cells[x][y + 1].x;
                to.y = map->cells[x][y + 1].y;
            }
            s32 length = to.y - from.y + 1;
            if (has_from && has_to && length < 3) {
                s32 lo = max(from_lo, to_lo);
                s32 hi = min(from_hi, to_hi);
                if (lo <= hi) {
                    from.x = (u8)func_800C5844(D_80147620, lo, hi);
                    carve(map, from.transposed(), Direction_800B98F4(6), length);
                } else if (from_hi < to_lo) {
                    from.x = from_hi;
                    for (s32 i = 0; i < length; i++, from.y++) {
                        carve(map, from.transposed(), Direction_800B98F4(0), to_lo - from_hi + 1);
                    }
                } else {
                    from.x = to_hi;
                    for (s32 i = 0; i < length; i++, from.y++) {
                        carve(map, from.transposed(), Direction_800B98F4(0), from_lo - to_hi + 1);
                    }
                }
            } else {
                bend(map, from, &to, 1);
            }
        }
    }
}
