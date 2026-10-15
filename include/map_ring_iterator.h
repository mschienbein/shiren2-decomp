#ifndef SHIREN2_MAP_RING_ITERATOR_H
#define SHIREN2_MAP_RING_ITERATOR_H

#include "map_rect.h"

/* Expanding square-ring scan, distinct from the position and line iterators.
 * Original providers access center@0, radius@8, limit@10, signed offsets@12/14,
 * direction@16 and steps@18. Natural target extent is 20 bytes; old callers'
 * 24-byte opaque buffers do not establish an additional field. */
typedef struct MapRingIterator {
    MapPos center;
    unsigned short radius;
    unsigned short limit;
    short offsetY;
    short offsetX;
    unsigned char direction;
    unsigned short steps;
} MapRingIterator;

#ifdef __cplusplus
extern "C" {
#endif
MapRingIterator *func_800C5280(MapRingIterator *iterator, MapPos *center, unsigned short limit);
void func_800C52C0(MapRingIterator *iterator, MapPos *center, unsigned short limit);
MapPos *func_800C532C(MapPos *output, MapRingIterator *iterator);
s32 func_800C559C(MapRingIterator *iterator);
#ifdef __cplusplus
}
#endif

#endif
