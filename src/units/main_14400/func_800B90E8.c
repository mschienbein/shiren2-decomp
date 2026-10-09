#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 state;
    u8 walls;
    u8 x;
    u8 y;
} Cell;

/* The generator has eleven rows of eight four-byte cells at +0x13.
 * Its separate word-valued availability array starts at +0x174; the
 * axis-edge pointers remain at +0x2D4/+0x2D8 (see func_800B8628). */
typedef struct {
    u8 pad0[0x10];
    u8 rows;
    u8 columns;
    u8 wanted;
    Cell cells[11][8];
    u8 pad173;
    s32 available[11][8];
    u8 *rowEdges;
    u8 *colEdges;
} Grid;

extern u8 D_80147620[];
extern s32 func_800C5844(void *rng, u8 base, u8 top);

void func_800B90E8(Grid *grid, u8 row, u8 col)
{
    grid->cells[row][col].x = func_800C5844(D_80147620, grid->rowEdges[row - 1] + 3, grid->rowEdges[row] - 4) + 10;
    grid->cells[row][col].y = func_800C5844(D_80147620, grid->colEdges[col - 1] + 3, grid->colEdges[col] - 4) + 10;
}
