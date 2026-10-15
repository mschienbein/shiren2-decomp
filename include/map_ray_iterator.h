#ifndef SHIREN2_MAP_RAY_ITERATOR_H
#define SHIREN2_MAP_RAY_ITERATOR_H

#include "map_line_iterator.h"

/* 36-byte extended ray: a real line cursor followed by reset origin,
 * availability state and the full blocking mask. */
typedef struct MapRayIterator {
    MapLineIterator line;
    MapPos origin;
    s32 available;
    u32 mask;
} MapRayIterator;

#endif
