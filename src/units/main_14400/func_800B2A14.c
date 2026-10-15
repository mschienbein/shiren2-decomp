#include "map_position_iterator.h"
typedef unsigned char u8;
typedef struct { u8 kind; u8 pad1[0xB]; u8 flagsC; } Item;
extern void *func_800B4D80(MapPos *p);
extern void func_800B4E7C(MapPos *);
extern s32 func_80049CB4(s32 id, ...);
static inline s32 position_x(MapPos *position) { return position->x; }
static inline s32 iterator_active(MapPositionIterator *iter, MapPos *current) {
    return position_x(current) <= iter->end.x;
}
s32 func_800B2A14(MapRect *area, s32 remove) {
    MapPositionIterator iterator;
    MapPos position;
    s32 found = 0;
    MapPositionIterator *iter = &iterator;
    MapPos *pos = &position;
    position.x = area->min.x;
    position.y = area->min.y;
    iterator.start = position;
    iterator.cur = iterator.start;
    position.x = area->max.x;
    position.y = area->max.y;
    iterator.end = position;
    while (iterator_active(iter, &iterator.cur)) {
        Item *item;
        s32 eligible;
        func_800A3610(pos, iter);
        item = func_800B4D80(pos);
        eligible = 0;
        if (item && item->kind == 0x10) {
            if ((item->flagsC >> 1) & 1) eligible = 0;
            else eligible = 1;
        }
        if (eligible) {
            if (!remove) return 1;
            func_800B4E7C(pos);
            func_80049CB4(6);
            func_80049CB4(0x107, pos);
            func_80049CB4(0xD7, pos);
            func_80049CB4(7);
            found = 1;
        }
    }
    return found;
}
