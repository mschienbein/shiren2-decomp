#include "map_position_iterator.h"

/*
 * Dungeon-room painter (C++ TU). The original constructs each by-value MapPos
 * argument with a member-wise copy constructor into one reused temporary slot
 * (sp+0x28) before the bitwise assignment into the iterator; C spellings of the
 * same copies leave the loop invariants allocated/scheduled differently.
 */

typedef unsigned short u16;

/* Preserve the native by-value corner copies when initializing shared storage. */
static inline void set_start(MapPositionIterator *it, MapPos p) {
    it->start = p;
    it->cur = it->start;
}
static inline void set_end(MapPositionIterator *it, MapPos p) {
    it->end = p;
}
static inline int valid(MapPositionIterator *it) { return it->cur.x <= it->end.x; }

/* 0x14-byte room record: bounds rectangle plus four per-side bytes (unused here). */
struct RoomRecord_800B7948 {
    MapRect rect;
    unsigned char pad10[4];
};

extern "C" {
extern RoomRecord_800B7948 D_801431F0[];
void func_800B1BE0(MapPos *pos, s32 mask);
void func_800B1B58(MapPos *pos, u16 flags);
}

/* self: caller-supplied object pointer, unused here. */
extern "C" void func_800B7948(void *self, s32 index, MapRect *rect)
{
    MapPositionIterator iter;

    set_start(&iter, rect->min);
    set_end(&iter, rect->max);
    while (valid(&iter)) {
        MapPos pos;
        func_800A3610(&pos, &iter);
        func_800B1BE0(&pos, 0x4000);
        func_800B1B58(&pos, (index & 0xF) | 0x1200);
    }
    func_800B6728(&D_801431F0[index], rect);
}
