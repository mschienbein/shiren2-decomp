#include "common.h"
#include "map_edge_iterator.h"
MapPos *func_800A3834(MapPos *out, MapEdgeIterator *obj) {
    out->x = obj->last.x;
    out->y = obj->last.y;
    return out;
}
