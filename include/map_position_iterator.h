#ifndef SHIREN2_MAP_POSITION_ITERATOR_H
#define SHIREN2_MAP_POSITION_ITERATOR_H

#include "map_rect.h"

/* The 24-byte position walk is separate from the 32-byte unit iterator.
 * cur/start/end occupy +0/+8/+0x10 on the pinned target. Foreign backing
 * storage and C++ lifetime still require their own evidence. */
typedef struct MapPositionIterator {
    MapPos cur;
    MapPos start;
    MapPos end;
} MapPositionIterator;

#ifdef __cplusplus
extern "C" {
#endif
MapPos *func_800A3610(MapPos *output, MapPositionIterator *iterator);
#ifdef __cplusplus
}
#endif

#endif
