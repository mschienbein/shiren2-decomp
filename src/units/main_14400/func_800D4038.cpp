#include "common.h"
#include "map_position_iterator.h"
#include "map_line_iterator.h"

/*
 * Room carving: mark every cell of the room rectangle, then cut the door
 * corridors. Keep the MapRect argument by value, its MapPos member copies,
 * and the separately scoped line iterators and per-step positions.
 */

typedef unsigned char u8;
typedef unsigned short u16;

struct Dir_800D4038 {
    u8 value;
};


/* B68B0 and B6A98 index four side counts at +0x10..+0x13. They need the
 * complete room-area record, not a pointer to its 16-byte geometry alone.
 * Foreign backing storage and C++ lifetime remain provider/caller contracts. */
struct RoomArea_800D4038 {
    MapRect area;
    u8 side_counts[4];
};

/* The corridor state exposes its room-area pointer at +0. */
struct Room_800D4038 {
    RoomArea_800D4038 *bounds;
};

extern "C" {
extern u8 D_80147620[];

void func_800B1B58(MapPos *pos, u16 flags);
s32 func_800B68B0(RoomArea_800D4038 *room);
MapPos *func_800B6A98(MapPos *out, RoomArea_800D4038 *room, s32 index);
s32 func_800B69F4(MapRect *r, MapPos *p);
void func_800A2758(MapPos *p, Dir_800D4038 d);
u32 func_800B1C6C(MapPos *pos);
void func_800B1BE0(MapPos *pos, s32 mask);
s32 func_800A3138(MapRect *rect);
s32 func_800A315C(MapRect *rect);
s32 func_800C5844(void *rng, u8 base, u8 top);
void func_800D4038(Room_800D4038 *room);
}

static inline MapPos *copy_point(MapPos *dst, MapPos *src)
{
    dst->x = src->x;
    dst->y = src->y;
    return dst;
}

static inline void rect_iter_init(MapPositionIterator *it, MapRect bounds)
{
    MapPos tmp;

    it->start = *copy_point(&tmp, &bounds.min);
    it->cur = it->start;
    it->end = *copy_point(&tmp, &bounds.max);
}

static inline u8 random_between(s32 low, s32 high)
{
    return func_800C5844(D_80147620, low, high);
}

/* 8-way direction for a room wall side index. */
static inline void set_side_dir(Dir_800D4038 *dir, s32 side)
{
    dir->value = (side * 2 + 4) & 7;
}

static inline s32 rect_iter_valid(MapPositionIterator *it)
{
    return it->cur.x <= it->end.x;
}

static inline s32 line_iter_valid(MapLineIterator *it)
{
    return it->index < it->length;
}

extern "C" void func_800D4038(Room_800D4038 *room)
{
    MapPositionIterator cells;
    rect_iter_init(&cells, room->bounds->area);
    MapPos point;
    while (rect_iter_valid(&cells)) {
        func_800A3610(&point, &cells);
        func_800B1B58(&point, 0x2000);
    }

    s32 count = func_800B68B0(room->bounds);
    if (count == 1) {
        func_800B6A98(&point, room->bounds, 0);
        Dir_800D4038 dir;
        set_side_dir(&dir, func_800B69F4(&room->bounds->area, &point));
        for (;;) {
            func_800A2758(&point, dir);
            if (!(func_800B1C6C(&point) & 0x1000)) {
                break;
            }
            func_800B1BE0(&point, 0x2000);
        }
    } else {
        s32 horizontal = 0;
        s32 vertical = 0;

        for (s32 i = 0; i < count; i++) {
            func_800B6A98(&point, room->bounds, i);
            s32 side = func_800B69F4(&room->bounds->area, &point);
            if (!(side & 1)) {
                horizontal++;
            } else {
                vertical++;
            }
        }
        s32 width = func_800A3138(&room->bounds->area);
        s32 height = func_800A315C(&room->bounds->area);
        if (!horizontal) {
            MapLineIterator line;

            copy_point(&point, &room->bounds->area.min);
            point.x = random_between(room->bounds->area.min.x + 1, room->bounds->area.max.x - 1);
            u8 dir = 0;
            func_800C25D0(&line, &point, &dir, width);
            while (line_iter_valid(&line)) {
                MapPos current;

                func_800C2758(&current, &line);
                func_800B1BE0(&current, 0x2000);
            }
        }
        if (!vertical) {
            MapLineIterator line;

            copy_point(&point, &room->bounds->area.min);
            point.y = random_between(room->bounds->area.min.y + 1, room->bounds->area.max.y - 1);
            u8 dir = 6;
            func_800C25D0(&line, &point, &dir, height);
            while (line_iter_valid(&line)) {
                MapPos current;

                func_800C2758(&current, &line);
                func_800B1BE0(&current, 0x2000);
            }
        }
        for (s32 i = 0; i < count; i++) {
            MapLineIterator line;

            func_800B6A98(&point, room->bounds, i);
            s32 side = func_800B69F4(&room->bounds->area, &point);
            Dir_800D4038 dir;
            set_side_dir(&dir, side);
            func_800A2758(&point, dir);
            s32 length = !(side & 1) ? width : height;
            func_800C25D0(&line, &point, &dir.value, length);
            while (line_iter_valid(&line)) {
                MapPos current;

                func_800C2758(&current, &line);
                func_800B1BE0(&current, 0x2000);
            }
        }
    }
}
