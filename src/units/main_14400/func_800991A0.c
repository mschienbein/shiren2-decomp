#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct {
    s32 x;
    s32 y;
} Pos;

typedef struct Grid Grid;

typedef struct {
    u8 pad0[0x80];
    s16 delta_80;
    s16 pad82;
    void (*func_84)(void *self, Pos *cursor);
} GridVTable;

/* Item grid menu: `cols` x `rows` cells, cursor at (col, row). */
struct Grid {
    u8 pad0[0x20];
    s32 cols;              /* 0x20 */
    s32 rows;              /* 0x24 */
    u8 pad28[0xC];
    Pos cursor;            /* 0x34: col, row */
    u8 pad3C[0x10];
    GridVTable *vtable_4C;
    u8 pad50[0x2FC - 0x50];
    s32 mode;              /* 0x2FC */
};

s32 func_80098E34(Grid *grid, s32 pos);
s32 func_80098DD4(Grid *grid, s32 id);
s32 func_80099028(Grid *grid, s32 id);
s32 func_8009910C(Grid *grid, s32 limit);
s32 func_80099084(Grid *grid, s32 key);
s32 func_80098F54(Grid *grid, s32 id);
void func_80045A24(s32 sound);

/* Move the grid cursor one step in direction `dir` (0..3). */
void func_800991A0(Grid *grid, s32 dir)
{
    Pos cursor;
    Pos *p;
    s32 index;
    s32 cell;
    s32 start;  /* first selection, before moving */
    s32 next;
    s32 prev;

    if (grid->mode == 8) {
        return;
    }
    p = &cursor;
    *p = grid->cursor;
    start = func_80098E34(grid, grid->cursor.x + grid->cols * grid->cursor.y);
    func_80098DD4(grid, start);
    index = start;
    switch (dir) {
    case 0:
        if (grid->mode == 0x400) {
            break;
        }
        next = index + 1;
        if (func_80099028(grid, next) != grid->cursor.y) {
            index = func_80099084(grid, grid->cursor.y);
        } else {
            index = next;
        }
        break;
    case 1:
        if (grid->mode == 0x400) {
            break;
        }
        prev = index - 1;
        if (func_80099028(grid, prev) != grid->cursor.y) {
            index = func_8009910C(grid, grid->cursor.y);
        } else {
            index = prev;
        }
        break;
    case 2:
        if (grid->rows < 2) {
            break;
        }
        if (++cursor.y >= grid->rows) {
            cursor.y = 0;
        }
        index = func_80098E34(grid, cursor.x + grid->cols * p->y);
        if (index < 0) {
            index = func_8009910C(grid, cursor.y);
            break;
        }
        if (func_80099028(grid, index) != cursor.y) {
            index = func_80099084(grid, cursor.y);
        }
        break;
    case 3:
        if (grid->rows < 2) {
            break;
        }
        if (--cursor.y < 0) {
            cursor.y = grid->rows - 1;
        }
        index = func_80098E34(grid, cursor.x + grid->cols * p->y);
        if (index < 0) {
            index = func_8009910C(grid, cursor.y);
            break;
        }
        if (func_80099028(grid, index) != cursor.y) {
            index = func_80099084(grid, cursor.y);
        }
        break;
    }
    if (index >= 0) {
        cell = func_80098F54(grid, index);
        cursor.y = cell / grid->cols;
        cursor.x = cell % grid->cols;
    } else {
        cursor.x = 0;
    }
    if (cursor.y != grid->cursor.y || cursor.x != grid->cursor.x) {
        grid->vtable_4C->func_84((u8 *)grid + grid->vtable_4C->delta_80, &cursor);
        func_80045A24(2);
    }
}
