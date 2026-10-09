#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Pair;
typedef Pair Pos;
typedef Pair Position;
typedef struct { Pair current, start, end; } Iter;
typedef struct { Pair start, end; } Area;
typedef struct { u8 kind; u8 pad1[0xB]; u8 flagsC; } Item;
extern Pair *func_800A3610(Pair *out, Iter *it);
extern void *func_800B4D80(Pos *p);
extern void func_800B4E7C(Position *);
extern s32 func_80049CB4(s32 id, ...);
static inline s32 position_x(Pair *position) { return position->x; }
static inline s32 iterator_active(Iter *iter, Pair *current) {
    return position_x(current) <= iter->end.x;
}
s32 func_800B2A14(Area *area, s32 remove) {
    Iter iterator;
    Pair position;
    s32 found = 0;
    Iter *iter = &iterator;
    Pair *pos = &position;
    position.x = area->start.x;
    position.y = area->start.y;
    iterator.start = position;
    iterator.current = iterator.start;
    position.x = area->end.x;
    position.y = area->end.y;
    iterator.end = position;
    while (iterator_active(iter, &iterator.current)) {
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
