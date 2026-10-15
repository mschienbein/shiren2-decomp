#include "map_ring_iterator.h"
static inline void copy_position(MapPos *dst, MapPos *src) {
    dst->x = src->x;
    dst->y = src->y;
}
MapRingIterator *func_800C5280(MapRingIterator *p, MapPos *src, unsigned short c) {
    MapPos tmp;
    copy_position(&tmp, src);
    func_800C52C0(p, &tmp, c);
    return p;
}
