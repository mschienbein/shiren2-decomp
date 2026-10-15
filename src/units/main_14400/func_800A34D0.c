#include "map_rect.h"

s32 func_800A34D0(const MapRect *bounds) {
    s32 valid = bounds->min.y <= bounds->max.y;
    return valid && bounds->min.x <= bounds->max.x;
}
