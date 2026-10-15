#include "map_line_iterator.h"


void func_800C25D0(MapLineIterator *obj, const MapPos *position,
                   const unsigned char *direction, s32 length)
{
    obj->current = *position;
    obj->direction = *direction;
    obj->length = length;
    obj->index = 0;
}
