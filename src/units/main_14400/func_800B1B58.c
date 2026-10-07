#include "common.h"

typedef unsigned short u16;

typedef struct {
    s32 x;
    s32 y;
} Pos;

extern u16 D_80143450[][76];

void func_800B1B58(Pos *pos, u16 flags) {
    s32 outside = 0;

    if (pos->y >= 76 || pos->x >= 54 || pos->y < 0 || pos->x < 0) {
        outside = 1;
    }
    if (!outside) {
        D_80143450[pos->x][pos->y] |= flags;
    }
}
