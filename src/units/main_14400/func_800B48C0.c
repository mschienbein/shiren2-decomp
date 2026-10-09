#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 x;
    s32 y;
} Point;

extern u8 D_80146468[54][76];

u8 func_800A8C00(void *actor);

s32 func_800B48C0(Point *pos, void *actor)
{
    if (actor == 0) {
        return 0;
    }
    D_80146468[pos->x][pos->y] = func_800A8C00(actor);
    return 1;
}
