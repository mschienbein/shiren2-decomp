#include "common.h"
#include "map_rect.h"
extern MapRect D_801429B0;
extern s32 func_800A251C(MapPos *a, MapPos *b);
s32 func_800A3050(MapRect *object) {
    s32 equal = 0;
    if (func_800A251C(&object->min, &D_801429B0.min)) {
        equal = func_800A251C(&object->max, &D_801429B0.max) != 0;
    }
    return equal;
}
