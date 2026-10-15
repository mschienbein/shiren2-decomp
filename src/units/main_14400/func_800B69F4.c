#include "map_rect.h"

s32 func_800B69F4(MapRect *r, MapPos *p) {
    s32 inside;

    inside = r->min.x <= p->x && p->x <= r->max.x;
    if (inside) {
        if (p->y == r->min.y - 1) {
            return 2;
        }
        if (p->y == r->max.y + 1) {
            return 0;
        }
    }
    inside = r->min.y <= p->y && p->y <= r->max.y;
    if (inside) {
        if (p->x == r->min.x - 1) {
            return 1;
        }
        if (p->x == r->max.x + 1) {
            return 3;
        }
    }
    return -1;
}
