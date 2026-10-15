#include "map_rect.h"
static __inline__ s32 less_equal(s32 left, s32 right) {
    return left <= right;
}
static __inline__ s32 bounds_nonempty(MapRect *bounds) {
    return less_equal(bounds->min.y, bounds->max.y) || less_equal(bounds->min.x, bounds->max.x);
}
s32 func_800A3378(const MapRect *a, const MapRect *b) {
    MapRect bounds;
    func_800A324C(&bounds, a, b);
    return bounds_nonempty(&bounds);
}
