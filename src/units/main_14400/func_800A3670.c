#include "common.h"
#include "map_edge_iterator.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

MapEdgeIterator *func_800A3670(MapEdgeIterator *e, MapPos *a, MapPos *b, MapPos *c) {
    if (a->y < c->y) {
        e->minimum.y = a->y;
        e->maximum.y = c->y;
    } else {
        e->minimum.y = c->y;
        e->maximum.y = a->y;
    }
    if (a->x < b->x) {
        e->minimum.x = a->x;
        e->maximum.x = b->x;
    } else {
        e->minimum.x = b->x;
        e->maximum.x = a->x;
    }
    e->current = e->minimum;
    e->slope = (double)(c->x - b->x) / (double)(c->y - b->y);
    e->intercept = (double)b->x - e->slope * (double)b->y;
    e->lower = a->x < b->x;
    e->upper = a->x > b->x;
    return e;
}
