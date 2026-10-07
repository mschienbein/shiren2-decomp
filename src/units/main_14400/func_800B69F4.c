#include "common.h"

typedef struct { s32 x0; s32 y0; s32 x1; s32 y1; } Rect;
typedef struct { s32 x; s32 y; } Point;
s32 func_800B69F4(Rect *r, Point *p) {
    s32 inside;

    inside = r->x0 <= p->x && p->x <= r->x1;
    if (inside) {
        if (p->y == r->y0 - 1) {
            return 2;
        }
        if (p->y == r->y1 + 1) {
            return 0;
        }
    }
    inside = r->y0 <= p->y && p->y <= r->y1;
    if (inside) {
        if (p->x == r->x0 - 1) {
            return 1;
        }
        if (p->x == r->x1 + 1) {
            return 3;
        }
    }
    return -1;
}
