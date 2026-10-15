#include "map_line_iterator.h"

typedef unsigned char u8;
extern s32 func_800A3138(MapRect *span);
extern s32 func_800A315C(MapRect *range);

static inline MapPos *set_pair(MapPos *pair, s32 x, s32 y) {
    pair->x = x;
    pair->y = y;
    return pair;
}

static inline u8 *set_direction(u8 *direction, u8 value) {
    *direction = value;
    return direction;
}

void func_800C25F4(MapLineIterator *iter, MapRect *rect, s32 side, s32 extend) {
    MapPos position;
    u8 direction0, direction1, direction2, direction3;
    s32 x0 = rect->min.x;
    s32 y0 = rect->min.y;
    s32 y1 = rect->max.y;
    s32 x1 = rect->max.x;
    s32 height = func_800A3138(rect);
    s32 width = func_800A315C(rect);
    switch (side) {
    case 0:
        if (extend) { x0--; width += 2; }
        func_800C25D0(iter, set_pair(&position, x0, y1 + 1), set_direction(&direction0, 6), width);
        break;
    case 1:
        func_800C25D0(iter, set_pair(&position, x0 - 1, y0), set_direction(&direction1, 0), height);
        break;
    case 2:
        if (extend) { x0--; width += 2; }
        func_800C25D0(iter, set_pair(&position, x0, y0 - 1), set_direction(&direction2, 6), width);
        break;
    default:
        func_800C25D0(iter, set_pair(&position, x1 + 1, y0), set_direction(&direction3, 0), height);
        break;
    }
}
