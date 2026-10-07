#include "common.h"

typedef short s16;

typedef struct {
    s32 x;
    s32 y;
} Cell;

/* s16 grid[0x36][0x4C], indexed [x][y] by byte offset. */
extern unsigned char D_80143450[];

void func_800B1CEC(s32 mode, Cell *cell);

void func_800B5AD0(Cell *cell, s16 value) {
    s32 outside = 0;

    if (cell->y >= 0x4C || cell->x >= 0x36 || cell->y < 0 || cell->x < 0) {
        outside = 1;
    }
    if (!outside) {
        *(s16 *)(D_80143450 + cell->y * 2 + cell->x * 0x98) = value;
        func_800B1CEC(1, cell);
    }
}
