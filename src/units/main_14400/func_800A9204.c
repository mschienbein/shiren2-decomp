#include "map_unit_iterator.h"

MapUnitIterator *func_800A9204(MapUnitIterator *iterator, MapRect *area, void *source) {
    MapPos *position = source;
    iterator->excluded = source;
    iterator->center = *position;
    iterator->area = *area;
    iterator->cursor = 0;
    return iterator;
}
