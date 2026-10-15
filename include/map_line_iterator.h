#ifndef SHIREN2_MAP_LINE_ITERATOR_H
#define SHIREN2_MAP_LINE_ITERATOR_H

#include "map_rect.h"

/* 20-byte directional line/edge cursor, distinct from the rectangular
 * 24-byte position walker and the 32-byte unit iterator. */
typedef struct MapLineIterator {
    MapPos current;
    unsigned char direction;
    s32 length;
    s32 index;
} MapLineIterator;

#ifdef __cplusplus
extern "C" {
#endif
void func_800C25D0(MapLineIterator *iterator, const MapPos *position,
                   const unsigned char *direction, s32 length);
void func_800C25F4(MapLineIterator *iterator, MapRect *rectangle,
                   s32 side, s32 extend);
MapPos *func_800C2758(MapPos *output, MapLineIterator *iterator);
#ifdef __cplusplus
}
#endif

#endif
