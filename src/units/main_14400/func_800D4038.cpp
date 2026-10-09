#include "common.h"

/*
 * Room carving: clear every cell of the room rectangle, then cut the door
 * corridors.  g++ 2.8.1 translation unit: block-scoped locals (the corridor
 * iterators and the per-step points) share stack slots once their scope ends,
 * which only the C++ front end does, and the Rect by-value argument is copied
 * member-wise through Point's copy constructor.
 */

typedef unsigned char u8;
typedef unsigned short u16;

struct Point {
    s32 x;
    s32 y;

    Point() {}
    Point(const Point &other) : x(other.x), y(other.y) {}
};

struct Rect {
    Point begin;
    Point end;
};

struct RectIter {
    Point current;
    Point begin;
    Point end;
};

struct Dir {
    u8 value;
};

struct LineIter {
    Point current;
    u8 dir;
    s32 length;
    s32 index;
};

struct Room {
    Rect *bounds;
};

extern "C" {
extern u8 D_80147620[];

Point *func_800A3610(Point *out, RectIter *it);
void func_800B1B58(Point *pos, u16 flags);
s32 func_800B68B0(Rect *rect);
void *func_800B6A98(void *out, void *room, s32 index);
s32 func_800B69F4(Rect *r, Point *p);
void func_800A2758(Point *p, Dir d);
u32 func_800B1C6C(Point *pos);
void func_800B1BE0(Point *pos, s32 mask);
s32 func_800A3138(Rect *rect);
s32 func_800A315C(Rect *rect);
s32 func_800C5844(void *rng, u8 base, u8 top);
void func_800C25D0(LineIter *it, Point *start, u8 *dir, s32 length);
Point *func_800C2758(Point *result, LineIter *it);
void func_800D4038(Room *room);
}

static inline Point *copy_point(Point *dst, Point *src)
{
    dst->x = src->x;
    dst->y = src->y;
    return dst;
}

static inline void rect_iter_init(RectIter *it, Rect bounds)
{
    Point tmp;

    it->begin = *copy_point(&tmp, &bounds.begin);
    it->current = it->begin;
    it->end = *copy_point(&tmp, &bounds.end);
}

static inline u8 random_between(s32 low, s32 high)
{
    return func_800C5844(D_80147620, low, high);
}

/* 8-way direction for a room wall side index. */
static inline void set_side_dir(Dir *dir, s32 side)
{
    dir->value = (side * 2 + 4) & 7;
}

static inline s32 rect_iter_valid(RectIter *it)
{
    return it->current.x <= it->end.x;
}

static inline s32 line_iter_valid(LineIter *it)
{
    return it->index < it->length;
}

extern "C" void func_800D4038(Room *room)
{
    RectIter cells;
    rect_iter_init(&cells, *room->bounds);
    Point point;
    while (rect_iter_valid(&cells)) {
        func_800A3610(&point, &cells);
        func_800B1B58(&point, 0x2000);
    }

    s32 count = func_800B68B0(room->bounds);
    if (count == 1) {
        func_800B6A98(&point, room->bounds, 0);
        Dir dir;
        set_side_dir(&dir, func_800B69F4(room->bounds, &point));
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
            s32 side = func_800B69F4(room->bounds, &point);
            if (!(side & 1)) {
                horizontal++;
            } else {
                vertical++;
            }
        }
        s32 width = func_800A3138(room->bounds);
        s32 height = func_800A315C(room->bounds);
        if (!horizontal) {
            LineIter line;

            copy_point(&point, &room->bounds->begin);
            point.x = random_between(room->bounds->begin.x + 1, room->bounds->end.x - 1);
            u8 dir = 0;
            func_800C25D0(&line, &point, &dir, width);
            while (line_iter_valid(&line)) {
                Point current;

                func_800C2758(&current, &line);
                func_800B1BE0(&current, 0x2000);
            }
        }
        if (!vertical) {
            LineIter line;

            copy_point(&point, &room->bounds->begin);
            point.y = random_between(room->bounds->begin.y + 1, room->bounds->end.y - 1);
            u8 dir = 6;
            func_800C25D0(&line, &point, &dir, height);
            while (line_iter_valid(&line)) {
                Point current;

                func_800C2758(&current, &line);
                func_800B1BE0(&current, 0x2000);
            }
        }
        for (s32 i = 0; i < count; i++) {
            LineIter line;

            func_800B6A98(&point, room->bounds, i);
            s32 side = func_800B69F4(room->bounds, &point);
            Dir dir;
            set_side_dir(&dir, side);
            func_800A2758(&point, dir);
            s32 length = !(side & 1) ? width : height;
            func_800C25D0(&line, &point, &dir.value, length);
            while (line_iter_valid(&line)) {
                Point current;

                func_800C2758(&current, &line);
                func_800B1BE0(&current, 0x2000);
            }
        }
    }
}
