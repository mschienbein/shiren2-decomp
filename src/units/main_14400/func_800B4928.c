#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    s32 x;
    s32 y;
} Point;

extern u8 D_80146468[54][76];

void *func_800A8CB0(s32 cell);

void *func_800B4928(Point *pos) {
    s32 outside = 0;
    u8 cell;

    if (pos->y >= 76 || pos->x >= 54 || pos->y < 0 || pos->x < 0) {
        outside = 1;
    }
    if (outside) {
        return 0;
    }
    cell = D_80146468[pos->x][pos->y];
    if (cell == 0xFF) {
        return 0;
    }
    return func_800A8CB0(cell);
}
