#include "map_rect.h"
extern s32 func_800A251C(MapPos *a, MapPos *b);
s32 func_800A2FFC(MapRect *left, MapRect *right) {
    return func_800A251C(&left->min, &right->min) && func_800A251C(&left->max, &right->max);
}
