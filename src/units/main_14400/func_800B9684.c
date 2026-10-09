#include "common.h"

typedef unsigned char u8;
typedef signed char s8;

typedef struct { s32 x; s32 y; } Point;
typedef struct { Point min; Point max; } Rect;
/* Grid cell bounds in cell units. */
typedef struct { Point min; Point max; } Cell;
/* Partial view of the grid generator: per-axis edge tables (D_80153BDC/D_80153BF8 rows). */
typedef struct {
    u8 pad[0x2D4];
    s8 *rowEdges;
    s8 *colEdges;
    Cell cells[16];
    s32 count;
    u8 pad3E0[8];
    s32 hasLast;
} Grid;

extern u8 D_80147620[];
s32 func_800C5844(void *rng, u8 base, u8 top);
void func_800B7948(void *self, s32 index, Rect *rect);


/* ODD_C: corner accessors perform actual Point component reads and keep
   their separate address pseudos until address folding. */
static inline s32 point_y(Point *p) { return p->y; }
static inline s32 point_x(Point *p) { return p->x; }
void func_800B9684(Grid *g, s32 idx)
{
    s32 last = 0;
    s32 top, bottom, left, right;
    s32 a, b, i;

    if (g->hasLast) {
        last = idx == g->count - 1;
    }

    i = point_y(&g->cells[idx].min);
    a = g->rowEdges[i - 1];
    if (i < point_y(&g->cells[idx].max)) {
        b = g->rowEdges[i] - 1;
    } else {
        b = g->rowEdges[i] - 5;
    }
    top = (u8)func_800C5844(D_80147620, a, b);
    i = point_y(&g->cells[idx].max);
    b = g->rowEdges[i] - 2;
    if (point_y(&g->cells[idx].min) < i) {
        a = g->rowEdges[i - 1];
    } else {
        a = top + 3;
    }
    if (last) {
        bottom = top + 3;
        if (bottom >= 0x42) {
            bottom = 0x41;
        }
    } else {
        bottom = (u8)func_800C5844(D_80147620, a, b);
        if (bottom - top + 1 < 4) {
            bottom = top + 5;
        }
    }

    i = point_x(&g->cells[idx].min);
    a = g->colEdges[i - 1];
    if (i < point_x(&g->cells[idx].max)) {
        b = g->colEdges[i] - 1;
    } else {
        b = g->colEdges[i] - 5;
    }
    left = (u8)func_800C5844(D_80147620, a, b);
    i = point_x(&g->cells[idx].max);
    b = g->colEdges[i] - 2;
    if (point_x(&g->cells[idx].min) < i) {
        a = g->colEdges[i - 1];
    } else {
        a = left + 3;
    }
    if (last) {
        right = left + 3;
        if (right >= 0x2C) {
            right = 0x2B;
        }
    } else {
        right = (u8)func_800C5844(D_80147620, a, b);
        if (right - left + 1 < 4) {
            right = left + 3;
        }
    }
    {
        Rect rc;
        Point p0, p1;
        p0.x = left + 10;
        p0.y = top + 10;
        p1.x = right + 10;
        p1.y = bottom + 10;
        rc.min = p0;
        rc.max = p1;
        func_800B7948(g, idx, &rc);
    }
}
