#include "map_position_iterator.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u16 cells[0x4C]; } Row800B2DEC;
extern Row800B2DEC D_80143450[];
s32 func_80049CB4(s32 id, ...);
void func_800B2DEC(MapRect *rect) {
    MapPositionIterator it;
    MapPositionIterator *iter = &it;
    MapPos pos;
    pos.x = rect->min.x;
    pos.y = rect->min.y;
    it.start = pos;
    it.cur = it.start;
    pos.x = rect->max.x;
    pos.y = rect->max.y;
    it.end = pos;
    while (1) {
        s32 more = it.cur.x <= iter->end.x;
        if (!more) break;
        func_800A3610(&pos, iter);
        D_80143450[pos.x].cells[pos.y] |= 0x400;
    }
    func_80049CB4(0xDC, rect);
}
