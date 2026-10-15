#include "map_rect.h"

s32 func_800A2424(MapPos *delta);

s32 func_800A23E8(MapPos *origin, MapPos *vec) {
    vec->x -= origin->x;
    vec->y -= origin->y;
    return func_800A2424(vec);
}
