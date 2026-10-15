#ifndef SHIREN2_MAP_EDGE_ITERATOR_H
#define SHIREN2_MAP_EDGE_ITERATOR_H

#include "map_rect.h"

/* 56-byte sloped-edge scan, distinct from the directional line iterator.
 * last is the accepted position saved before current advances, not padding. */
typedef struct MapEdgeIterator {
    MapPos current;
    MapPos last;
    MapPos minimum;
    MapPos maximum;
    double slope;
    double intercept;
    s32 lower;
    s32 upper;
} MapEdgeIterator;

#ifdef __cplusplus
extern "C" {
#endif
MapEdgeIterator *func_800A3670(MapEdgeIterator *iterator, MapPos *first,
                              MapPos *second, MapPos *third);
s32 func_800A375C(MapEdgeIterator *iterator);
MapPos *func_800A3834(MapPos *output, MapEdgeIterator *iterator);
#ifdef __cplusplus
}
#endif

#endif
