#include "map_rect.h"
s32 func_800A2854(void *, MapPos *, MapPos *);
s32 func_800A24DC(void *self, MapRect *r) {
    MapRect t;
    t.min.x = r->min.x;
    t.min.y = r->min.y;
    t.max.x = r->max.x;
    t.max.y = r->max.y;
    return func_800A2854(self, &t.min, &t.max);
}
