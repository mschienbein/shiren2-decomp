#ifndef SHIREN2_MAP_UNIT_ITERATOR_H
#define SHIREN2_MAP_UNIT_ITERATOR_H

#include "map_rect.h"

/* The 32-byte area-unit iterator on the target ABI. The separate 24-byte
 * position/rectangle walker is not this type. Foreign unit storage and its
 * lifetime remain separate from the iterator's actual declared subobjects. */
typedef struct MapUnitIterator {
    s32 cursor;
    void *excluded;
    MapPos center;
    MapRect area;
} MapUnitIterator;

#ifdef __cplusplus
extern "C" {
#endif
MapUnitIterator *func_800A915C(MapUnitIterator *iterator, void *source);
MapUnitIterator *func_800A919C(MapUnitIterator *iterator, MapPos *position);
MapUnitIterator *func_800A9204(MapUnitIterator *iterator, MapRect *area, void *source);
MapUnitIterator *func_800A9244(MapUnitIterator *iterator, MapRect *area, MapPos *position);
s32 func_800A9284(MapUnitIterator *iterator, s32 filter);
void *func_800A942C(MapUnitIterator *iterator);
#ifdef __cplusplus
}
#endif

#endif
