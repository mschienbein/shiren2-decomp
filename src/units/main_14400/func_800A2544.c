#include "common.h"

typedef struct {
    s32 x;
    s32 y;
} Vec2_800A2544;

Vec2_800A2544 *func_800A2544(Vec2_800A2544 *out, Vec2_800A2544 *a, Vec2_800A2544 *b)
{
    s32 x = a->x + b->x;
    s32 y = a->y + b->y;

    out->x = x;
    out->y = y;
    return out;
}
