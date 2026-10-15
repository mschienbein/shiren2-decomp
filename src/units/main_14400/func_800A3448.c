#include "map_rect.h"

/* Preserve the original min.y/min.x/max.y/max.x comparison/store order.
 * Read each bound after earlier stores; do not snapshot the bounds first. */
void func_800A3448(MapRect *arg0, MapRect *arg1)
{
    if (arg0->min.y < arg1->min.y) {
        arg0->min.y = arg1->min.y;
    }
    if (arg0->min.x < arg1->min.x) {
        arg0->min.x = arg1->min.x;
    }
    if (arg0->max.y > arg1->max.y) {
        arg0->max.y = arg1->max.y;
    }
    if (arg0->max.x > arg1->max.x) {
        arg0->max.x = arg1->max.x;
    }
}
