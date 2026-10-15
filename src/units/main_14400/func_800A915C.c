#include "map_unit_iterator.h"

/* Geometry prefix of the supplied foreign unit; this view does not establish
 * the unit's allocation or lifetime. */
typedef struct {
    MapPos position;
    s32 unk8;
    MapRect area;
} Source;

MapUnitIterator *func_800A915C(MapUnitIterator *dest, void *source) {
    Source *src = source;
    dest->excluded = src;
    dest->center = src->position;
    dest->area = src->area;
    dest->cursor = 0;
    return dest;
}
