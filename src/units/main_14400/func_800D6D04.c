#include "common.h"

typedef struct { s32 x; s32 y; } Pos800D6784;

s32 func_800D6D04(void *map, Pos800D6784 *pos, Pos800D6784 *best)
{
    Pos800D6784 *origin = map;
    s32 a = pos->y - origin->y;
    s32 b;
    if (a < 0) a = origin->y - pos->y;
    a += pos->x - origin->x >= 0 ? pos->x - origin->x : origin->x - pos->x;
    b = best->y - origin->y;
    if (b < 0) b = origin->y - best->y;
    b += best->x - origin->x >= 0 ? best->x - origin->x : origin->x - best->x;
    return a <= b;
}
