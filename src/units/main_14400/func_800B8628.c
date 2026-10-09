#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef struct { s32 x, y; } Point;
typedef struct { Point first, second; } Rectangle;
typedef struct { u8 state, walls, x, y; } Cell;
/* Dimensions precede the 11-by-8 cell array; the parallel availability array
 * starts at +0x174 and ends immediately before the axis-edge pointers. */
typedef struct {
    u8 pad0[0x10];
    u8 rows, columns, wanted;
    Cell cells[11][8];
    u8 pad173;
    s32 available[11][8];
    s8 *rowEdges, *columnEdges;
    Rectangle rooms[16];
    s32 count;
    u8 mode, top, left, bottom, right;
    u8 pad3E5[3];
    s32 hasLast, lastColumn, lastRow;
} Object;
extern Rectangle D_801474A0, D_80147510[];
extern u8 D_80147620[];
extern u8 func_800C57CC(void *, s32);
extern u8 func_800C57A0(void *);
extern s32 func_800C587C(void *, u8);
extern s32 func_800A32D8(Rectangle *, Rectangle *);
/* ODD_C: copying and the first bounds comparison share the temporary's scope. */
static inline s32 compare_bounds(Rectangle *out, Rectangle *source) {
    out->first = source->first;
    out->second = source->second;
    return func_800A32D8(out, &D_801474A0);
}
void func_800B8628(Object *p, u8 mode) {
    Rectangle first, second;
    s32 row, column;
    s32 first_enabled, second_enabled;
    p->mode = mode;
    {
        s32 initial_row = 0;
        for (;;) {
            s32 limit = p->rows + 1;
            s32 initial_column;
            if (initial_row > limit) break;
            for (initial_column = 0; initial_column <= p->columns + 1; initial_column++) {
                p->cells[initial_row][initial_column].state = 0x40;
                p->cells[initial_row][initial_column].walls = 0;
                p->available[initial_row][initial_column] = 1;
            }
            initial_row++;
        }
    }
    p->top = 1;
    p->left = 1;
    p->bottom = p->rows;
    p->right = p->columns;
    if (p->mode == 1 && p->rows >= 5 && p->columns >= 5) {
        u8 sides = func_800C57CC(D_80147620, 14);
        sides++;
        if (sides & 1) p->top++;
        if (sides & 2) p->bottom--;
        if (sides & 4) p->left++;
        if (sides & 8) p->right--;
    }
    {
        Rectangle *source = &D_80147510[func_800C57CC(D_80147620, 5) & 0xFF];
        first.first.x = source->first.x;
        first.first.y = source->first.y;
        first.second.x = source->second.x;
        first.second.y = source->second.y;
    }
    {
        Rectangle *source = &D_80147510[2 + (func_800C57A0(D_80147620) & 3)];
        second.first.x = source->first.x;
        second.first.y = source->first.y;
        second.second.x = source->second.x;
        second.second.y = source->second.y;
    }
    first_enabled = 0;
    second_enabled = 0;
    if (!p->mode) {
        if (func_800C587C(D_80147620, 10)) first_enabled = 1;
        else if (func_800C587C(D_80147620, 10)) second_enabled = 1;
    }
    row = p->top;
    for (;;) {
        s32 limit = p->bottom;
        if (row > limit) break;
        column = p->left;
        for (;;) {
            s32 limit = p->right;
            Rectangle bounds, copy;
            if (column > limit) break;
            p->cells[row][column].state = 0x80;
            {
                s32 x = p->columnEdges[column - 1], y = p->rowEdges[row - 1];
                bounds.first.x = x;
                bounds.first.y = y;
            }
            {
                s32 x = p->columnEdges[column], y = p->rowEdges[row];
                bounds.second.x = x;
                bounds.second.y = y;
            }
            if (compare_bounds(&copy, &bounds)) p->available[row][column] = 0;
            if (first_enabled && func_800A32D8(&copy, &first)) p->available[row][column] = 0;
            if (second_enabled && func_800A32D8(&copy, &second)) p->cells[row][column].state = 0x40;
            column++;
        }
        row++;
    }
    if (p->hasLast) {
        s32 edge;
        p->lastRow = (func_800C57A0(D_80147620) & 1) ? 1 : p->rows;
        edge = (func_800C57A0(D_80147620) & 1) ? 1 : p->columns;
        p->lastColumn = edge;
        p->cells[p->lastRow][edge].state = 0x40;
        p->cells[p->lastRow][p->lastColumn].walls = 0;
    }
}
