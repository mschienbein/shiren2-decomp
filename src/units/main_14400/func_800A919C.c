#include "map_unit_iterator.h"

/* Iterator over the units in the area around a position, with no excluded unit. */
MapUnitIterator *func_800A919C(MapUnitIterator *it, MapPos *pos) {
    it->excluded = 0;
    it->center = *pos;
    it->area = func_800B3080(&it->center);
    it->cursor = 0;
    return it;
}
