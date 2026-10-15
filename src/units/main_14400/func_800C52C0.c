#include "map_ring_iterator.h"

void func_800C52C0(MapRingIterator *p, MapPos *v, unsigned short c)
{
    p->center = *v;
    p->radius = 0;
    p->limit = c;
    p->offsetY = 0;
    p->offsetX = 0;
    p->direction = 0;
    p->steps = 0;
}
