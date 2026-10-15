#include "map_rect.h"


s32 func_800A3138(MapRect *span)
{
    s32 diff = span->max.y - span->min.y;

    if (diff < 0) {
        return diff - 1;
    }
    return diff + 1;
}
