#include "map_unit_iterator.h"

MapUnitIterator *func_800A9244(MapUnitIterator *iterator, MapRect *area, MapPos *position) {
    iterator->excluded = 0;
    iterator->center = *position;
    iterator->area = *area;
    iterator->cursor = 0;
    return iterator;
}
