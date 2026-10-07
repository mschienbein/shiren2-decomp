#include "common.h"

typedef struct {
    s32 x;
    s32 y;
} Vec2;

Vec2 *func_800A256C(Vec2 *out, Vec2 *a, Vec2 *b)
{
    s32 x = a->x - b->x;
    s32 y = a->y - b->y;

    out->x = x;
    out->y = y;
    return out;
}
