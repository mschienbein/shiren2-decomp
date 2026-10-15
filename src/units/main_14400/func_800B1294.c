#include "common.h"
#include "map_position_iterator.h"
void func_800B1420(MapPositionIterator *r);
static inline void pair_set(MapPos *p, s32 x, s32 y) {
    p->x = x;
    p->y = y;
}
void func_800B1294(void) {
    MapPositionIterator r;
    MapPos a, b;
    pair_set(&a, 0, 0);
    pair_set(&b, 9, 75);
    r.start = a;
    r.cur = r.start;
    r.end = b;
    func_800B1420(&r);
    pair_set(&a, 10, 0);
    pair_set(&b, 43, 9);
    r.start = a;
    r.cur = r.start;
    r.end = b;
    func_800B1420(&r);
    pair_set(&a, 10, 66);
    pair_set(&b, 43, 75);
    r.start = a;
    r.cur = r.start;
    r.end = b;
    func_800B1420(&r);
    pair_set(&a, 44, 0);
    pair_set(&b, 53, 75);
    r.start = a;
    r.cur = r.start;
    r.end = b;
    func_800B1420(&r);
}
