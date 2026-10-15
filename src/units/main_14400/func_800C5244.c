#include "map_ray_iterator.h"


void func_800C5244(MapRayIterator *obj) {
    func_800C25D0(&obj->line, &obj->origin, &obj->line.direction, obj->line.length);
    obj->available = 1;
}
