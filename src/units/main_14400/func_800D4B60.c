#include "common.h"
#include "map_rect.h"

typedef signed char s8;
typedef struct { unsigned char pad00[8]; s8 index08; } S;
extern MapRect D_80147F80;
extern s32 func_800A315C(MapRect *range);

MapPos *func_800D4B60(MapPos *out, S *self, s32 index) {
    MapPos point;
    MapRect *ranges = &D_80147F80;
    s32 width = func_800A315C(&ranges[self->index08]);
    s32 row = index / width;
    MapPos *pos = &point;
    MapRect *range;
    pos->x = index - row * width;
    pos->y = row;
    range = &ranges[self->index08];
    pos->x += range->min.x;
    pos->y += range->min.y;
    out->x = pos->x;
    out->y = pos->y;
    return out;
}
