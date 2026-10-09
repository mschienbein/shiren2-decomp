#include "common.h"
typedef unsigned char u8;
typedef struct { s32 y, x; } Position;
typedef struct { u8 field00, kind01; } Cell;
extern void *func_800B4D80(Position *);
extern s32 func_800B5728(void *);
u8 func_800416F8(s32 x, s32 y)
{
    Position position;
    Position *p = &position;
    Cell *cell;
    s32 result;
    position.y = y;
    p->x = x;
    cell = func_800B4D80(p);
    if (cell == 0) {
        position.y = y;
        p->x = x;
        result = (u8)func_800B5728(p);
    } else {
        result = cell->kind01;
    }
    return result;
}
