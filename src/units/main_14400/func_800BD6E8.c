#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Point;
typedef struct { Point min, max; } Rect;
typedef struct { Point current, min, max; } Iterator;
typedef struct Random Random;
/* Fixed-capacity stack of pending carve coordinates (one axis). */
typedef struct { s32 at[100]; } Stack;
extern s32 D_80147570[4];
extern s32 D_80147580[4];
extern u8 D_80147590[24][4];
extern Random D_80147620;
extern u32 func_800B1C6C(Point *point);
extern u8 func_800C57CC(void *random, s32 limit);
extern s32 func_800A31C8(Rect *rectangle, Point *point);
extern void func_800B1AE0(Point *point, u16 value);
extern Point *func_800A3610(Point *out, Iterator *iterator);
extern void func_800B1BE0(Point *point, s32 mask);

/* ODD_C: copying and querying complete Point objects keeps their two
   coordinate stores grouped without introducing extra address lifetimes. */
static inline void point_copy(Point *out, Point *point)
{
    out->x = point->x;
    out->y = point->y;
}

static inline u32 point_flags(s32 x, s32 y, Point *point)
{
    point->x = x;
    point->y = y;
    return func_800B1C6C(point);
}

/* The caller forwards its floor receiver; carving here uses only the bounds. */
s32 func_800BD6E8(void *floor, Rect *bounds)
{
    Stack rows;
    Stack columns;
    Point probe;
    Point check;
    Iterator iterator;
    u8 count = 0;
    s32 left = bounds->min.x;
    s32 right = bounds->max.x;
    s32 top = bounds->min.y;
    s32 bottom = bounds->max.y;
    s32 x, y;
    for (y = top + 1;; y += 2) {
        s32 edge;
        if (y > bottom - 1) break;
        edge = left - 1;
        if (!(point_flags(edge, y, &probe) & 0x800)) {
            s32 index = count;
            ++count;
            rows.at[index] = y;
            columns.at[index] = edge;
        }
        {
            s32 far_edge = right + 1;
            if (!(point_flags(far_edge, y, &probe) & 0x800)) {
                s32 index = count;
                ++count;
                rows.at[index] = y;
                columns.at[index] = far_edge;
            }
        }
    }
    for (x = left + 1;; x += 2) {
        s32 edge;
        if (x > right - 1) break;
        edge = top - 1;
        if (!(point_flags(x, edge, &probe) & 0x800)) {
            s32 index = count;
            ++count;
            rows.at[index] = edge;
            columns.at[index] = x;
        }
        {
            s32 far_edge = bottom + 1;
            if (!(point_flags(x, far_edge, &probe) & 0x800)) {
                s32 index = count;
                ++count;
                rows.at[index] = far_edge;
                columns.at[index] = x;
            }
        }
    }
    /* ODD_C: the carve phase works through named pointers to the two pending
       stacks and the probe, initialized after the count guard; this also keeps
       their bases outside the nested random-walk loops. */
    if (count >= 101U) return 0;
    {
        Point *pending_probe;
        Stack *pending_rows = &rows;
        Stack *pending_columns = &columns;
        pending_probe = &probe;
        for (;;) {
            u8 chosen;
            if (count == 0) break;
            --count;
            chosen = count != 0 ? func_800C57CC(&D_80147620, (u8)(count - 1)) : 0;
            y = pending_rows->at[chosen];
            pending_rows->at[chosen] = pending_rows->at[count];
            x = pending_columns->at[chosen];
            pending_columns->at[chosen] = pending_columns->at[count];
            for (;;) {
                u8 *order = D_80147590[func_800C57CC(&D_80147620, 23)];
                u8 i = 0;
                s32 next_x, next_y;
                for (;;) {
                    s32 accepted = 0;
                    if (i >= 4U) break;
                    next_x = x + D_80147580[order[i]];
                    next_y = y + D_80147570[order[i]];
                    if (point_flags(next_x, next_y, &probe) & 0x1000) {
                        check.x = next_x;
                        check.y = next_y;
                        accepted = func_800A31C8(bounds, &check) != 0;
                    }
                    if (accepted) break;
                    ++i;
                }
                if (i >= 4U) break;
                probe.x = (x + next_x) / 2;
                probe.y = (y + next_y) / 2;
                func_800B1AE0(pending_probe, 0x4000);
                y = next_y;
                x = next_x;
                probe.x = x;
                probe.y = y;
                func_800B1AE0(pending_probe, 0x4000);
                {
                    s32 index = count;
                    ++count;
                    pending_rows->at[index] = y;
                    pending_columns->at[index] = x;
                }
            }
        }
    }
    point_copy(&probe, &bounds->min);
    iterator.min = probe;
    iterator.current = iterator.min;
    point_copy(&probe, &bounds->max);
    iterator.max = probe;
    for (;;) {
        s32 remaining = iterator.current.x <= iterator.max.x;
        if (!remaining) break;
        func_800A3610(&probe, &iterator);
        if (func_800B1C6C(&probe) & 0x1000) {
            func_800B1BE0(&probe, 0x100F);
        }
    }
    return 1;
}
