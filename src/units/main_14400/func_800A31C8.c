#include "map_rect.h"
s32 func_800A31C8(MapRect *a, MapPos *b) { return b->y >= a->min.y && b->y <= a->max.y && b->x >= a->min.x && b->x <= a->max.x; }
