#include "common.h"
#include "map_edge_iterator.h"

s32 func_800A375C(MapEdgeIterator *self)
{
    s32 found = 0;
    if (self->current.x > self->maximum.x) {
        return 0;
    }
    do {
        double boundary = self->slope * self->current.y + self->intercept;
        double x = self->current.x;
        if ((self->lower && x <= boundary) ||
            (self->upper && x >= boundary)) {
            found = 1;
            self->last = self->current;
        }
        ++self->current.y;
        if (self->current.y > self->maximum.y) {
            self->current.y = self->minimum.y;
            ++self->current.x;
        }
        if (found) {
            return 1;
        }
    } while (self->current.x <= self->maximum.x);
    return 0;
}
