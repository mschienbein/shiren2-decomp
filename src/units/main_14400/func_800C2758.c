#include "map_line_iterator.h"
typedef unsigned char u8;
typedef struct { u8 value; } Dir;
extern void func_800A2758(MapPos *position, Dir direction);
static inline void copy(MapPos *dest, const MapPos *source) {
    dest->x = source->x;
    dest->y = source->y;
}
MapPos *func_800C2758(MapPos *result, MapLineIterator *obj) {
    MapPos saved;
    Dir direction;
    copy(&saved, &obj->current);
    direction.value = obj->direction;
    func_800A2758(&obj->current, direction);
    obj->index++;
    copy(result, &saved);
    return result;
}
