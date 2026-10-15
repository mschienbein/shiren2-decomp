#include "map_rect.h"

typedef struct { s32 x0; s32 y0; s32 x1; s32 y1; } Rect;
extern Rect D_801429C0;

static inline void copy_coordinate(s32 *out, s32 value) {
    *out = value;
}
MapRect *func_800B1F58(MapRect *out) {
    copy_coordinate(&out->min.x, D_801429C0.x0);
    copy_coordinate(&out->min.y, D_801429C0.y0);
    copy_coordinate(&out->max.x, D_801429C0.x1);
    copy_coordinate(&out->max.y, D_801429C0.y1);
    return out;
}
