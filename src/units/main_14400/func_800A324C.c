#include "map_rect.h"

MapRect *func_800A324C(MapRect *out, const MapRect *a, const MapRect *b) {
    MapRect r;

    s32 top;
    s32 left;
    s32 bottom;
    s32 right;

    top = b->min.y;
    if (top < a->min.y) {
        top = a->min.y;
    }
    r.min.y = top;
    left = b->min.x;
    if (left < a->min.x) {
        left = a->min.x;
    }
    r.min.x = left;
    bottom = b->max.y;
    if (a->max.y < bottom) {
        bottom = a->max.y;
    }
    r.max.y = bottom;
    right = b->max.x;
    if (a->max.x < right) {
        right = a->max.x;
    }
    r.max.x = right;
    out->min.x = r.min.x;
    out->min.y = r.min.y;
    out->max.x = r.max.x;
    out->max.y = r.max.y;
    return out;
}
