#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 x;
    u8 y;
    u8 pad2[2];
} Cell;

typedef struct {
    u8 pad0[0x15];
    Cell cells[21][8];
    u8 pad2B5[0x1F];
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
