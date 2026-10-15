#include "common.h"
#include "map_position_iterator.h"

typedef unsigned char u8;

static inline void setStart(MapPositionIterator &iter, MapPos p)
{
    iter.start = p;
    iter.cur = iter.start;
}

static inline void setEnd(MapPositionIterator &iter, MapPos p)
{
    iter.end = p;
}

static inline int valid(MapPositionIterator &iter)
{
    return iter.cur.x <= iter.end.x;
}

struct Tile_800B37E8 {
    u8 kind;
    u8 feature;
};

extern "C" {
extern MapRect D_801429C0;
s32 func_800B4F74(MapPos *pos);
void *func_800B4D80(MapPos *pos);
}

extern "C" MapPos func_800B37E8(void)
{
    MapPositionIterator iter;

    setStart(iter, D_801429C0.min);
    setEnd(iter, D_801429C0.max);
    while (valid(iter)) {
        MapPos pos;
        s32 found;
        func_800A3610(&pos, &iter);
        found = 0;
        if (func_800B4F74(&pos)) {
            found = ((Tile_800B37E8 *)func_800B4D80(&pos))->feature == 0xCE;
        }
        if (found) {
            return pos;
        }
    }
    return MapPos(0, 0);
}
