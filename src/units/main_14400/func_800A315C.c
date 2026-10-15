#include "map_rect.h"


s32 func_800A315C(MapRect *range) {
    s32 diff = range->max.x - range->min.x;

    if (diff < 0) {
        return diff - 1;
    }
    return diff + 1;
}
