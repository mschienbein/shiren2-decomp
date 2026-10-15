#include "map_rect.h"

static inline s32 x_positive(MapRect *bounds) { return bounds->min.x > bounds->max.x; }
static inline s32 y_positive(MapRect *bounds) { return bounds->min.y > bounds->max.y; }
s32 func_800A3308(const MapRect *first, const MapRect *second) {
    MapRect bounds;
    func_800A324C(&bounds, first, second);
    {
    s32 value = 0;
    if (x_positive(&bounds)) value = bounds.min.x - bounds.max.x;
    if (y_positive(&bounds)) value += bounds.min.y - bounds.max.y;
    return value;
    }
}
