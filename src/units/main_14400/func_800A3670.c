#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { s32 x; s32 y; } Point;
typedef struct {
    Point min0;
    u8 pad8[8];
    Point min;
    Point max;
    double slope;
    double intercept;
    s32 f_30;
    s32 f_34;
} Edge;
Edge *func_800A3670(Edge *e, Point *a, Point *b, Point *c) {
    if (a->y < c->y) {
        e->min.y = a->y;
        e->max.y = c->y;
    } else {
        e->min.y = c->y;
        e->max.y = a->y;
    }
    if (a->x < b->x) {
        e->min.x = a->x;
        e->max.x = b->x;
    } else {
        e->min.x = b->x;
        e->max.x = a->x;
    }
    e->min0 = e->min;
    e->slope = (double)(c->x - b->x) / (double)(c->y - b->y);
    e->intercept = (double)b->x - e->slope * (double)b->y;
    e->f_30 = a->x < b->x;
    e->f_34 = a->x > b->x;
    return e;
}
