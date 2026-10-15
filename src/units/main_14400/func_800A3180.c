#include "map_rect.h"

void func_800A3180(MapRect *rect) {
    s32 tmp;
    if (rect->min.y > rect->max.y) {
        tmp = rect->min.y - 1;
        rect->min.y = rect->max.y + 1;
        rect->max.y = tmp;
    }
    if (rect->min.x > rect->max.x) {
        tmp = rect->min.x - 1;
        rect->min.x = rect->max.x + 1;
        rect->max.x = tmp;
    }
}
