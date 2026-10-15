#include "common.h"
#include "map_position_iterator.h"
typedef unsigned short u16;
extern MapRect D_801429C0;
extern u16 D_80143450[][76];
extern u32 func_800B1C6C(MapPos *pos);
extern void func_800B2884(void);
static __inline__ void set_start(MapPositionIterator *it, s32 x, s32 y) {
    MapPos p;
    p.x = x; p.y = y;
    it->start = p;
    it->cur = it->start;
}
static __inline__ void set_end(MapPositionIterator *it, s32 x, s32 y) {
    MapPos p;
    p.x = x; p.y = y;
    it->end = p;
}
static __inline__ s32 valid(MapPositionIterator *it) { return it->cur.x <= it->end.x; }
s32 func_800B274C(s32 kind) {
    MapPositionIterator it;
    s32 changed = 0;
    set_start(&it, D_801429C0.min.x, D_801429C0.min.y);
    set_end(&it, D_801429C0.max.x, D_801429C0.max.y);
    while (valid(&it)) {
        MapPos pos;
        func_800A3610(&pos, &it);
        if (func_800B1C6C(&pos) & 0x2000) {
            D_80143450[pos.x][pos.y] = kind | (D_80143450[pos.x][pos.y] & 0xDFFF);
            changed = 1;
        }
    }
    if (changed) func_800B2884();
    return changed;
}
