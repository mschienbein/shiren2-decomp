#include "common.h"
typedef struct { s32 x, y; } Pair;
typedef struct { Pair cur; Pair start; Pair end; } Range;
void func_800B1420(Range *r);
static inline void pair_set(Pair *p, s32 x, s32 y) {
    p->x = x;
    p->y = y;
}
void func_800B1294(void) {
    Range r;
    Pair a, b;
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
