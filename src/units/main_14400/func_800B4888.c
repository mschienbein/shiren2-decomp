#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct {
    s32 row;
    s32 col;
} Cell;

extern u8 D_80146468[][76];

s32 func_800B4888(Cell *cell) {
    return D_80146468[cell->row][cell->col] != 0xFF;
}
