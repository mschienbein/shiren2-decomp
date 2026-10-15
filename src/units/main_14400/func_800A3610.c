#include "map_position_iterator.h"
static inline void copy_position(MapPos *dst, MapPos *src) {
    dst->x = src->x;
    dst->y = src->y;
}
MapPos *func_800A3610(MapPos *out, MapPositionIterator *it) {
    MapPos tmp;
    copy_position(&tmp, &it->cur);
    it->cur.y++;
    if (it->end.y < it->cur.y) {
        it->cur.y = it->start.y;
        it->cur.x++;
    }
    copy_position(out, &tmp);
    return out;
}
