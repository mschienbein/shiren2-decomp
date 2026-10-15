#include "common.h"
#include "map_position_iterator.h"
typedef unsigned char u8;
typedef struct { char pad[0x1E]; u8 flags; } Ent;
extern MapRect D_801429C0;
Ent *func_800B4928(MapPos *);
void func_800B4A1C(MapPos *);
static inline s32 Range_valid(MapPositionIterator *r) { return r->cur.x <= r->end.x; }
static inline void Range_setBegin(MapPositionIterator *r) {
    MapPos p;
    p.x = D_801429C0.min.x;
    p.y = D_801429C0.min.y;
    r->start = p;
    r->cur = r->start;
}
static inline void Range_setEnd(MapPositionIterator *r) {
    MapPos p;
    p.x = D_801429C0.max.x;
    p.y = D_801429C0.max.y;
    r->end = p;
}
static inline void Range_init(MapPositionIterator *r) {
    Range_setBegin(r);
    Range_setEnd(r);
}
void func_800B4B88(void) {
    MapPositionIterator r;
    Range_init(&r);
    while (1) {
        MapPos c;
        Ent *e;
        s32 ok = Range_valid(&r);
        if (!ok) break;
        func_800A3610(&c, &r);
        e = func_800B4928(&c);
        if (e && (e->flags & 0xC)) continue;
        func_800B4A1C(&c);
    }
}