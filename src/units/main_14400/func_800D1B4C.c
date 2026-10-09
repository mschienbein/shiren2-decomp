#include "common.h"

typedef unsigned char u8;

/* Clears one cell of a byte grid laid out with four cells per row. */
void func_800D1B4C(u8 *grid, s32 row, s32 col)
{
    s32 index = col + row * 4;

    grid[index] = 0;
}
