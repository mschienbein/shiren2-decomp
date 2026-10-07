#include "common.h"

typedef unsigned short u16;

typedef struct {
    s32 x;
    s32 y;
} Pos;

extern short D_80143450[0x36][0x4C];

void func_800B1AE0(Pos *pos, u16 value) {
    s32 outside = 0;

    if (pos->y >= 0x4C || pos->x >= 0x36 || pos->y < 0 || pos->x < 0) {
        outside = 1;
    }
    if (!outside) {
        D_80143450[pos->x][pos->y] = value;
    }
}
