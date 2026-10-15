#include "map_rect.h"
static inline s32 absolute(s32 value)
{
    if (value < 0) value = -value;
    return value;
}
s32 func_800A2424(MapPos *arg)
{
    s32 y = absolute(arg->y);
    s32 x = absolute(arg->x);
    if (x < y) x = y;
    return x;
}
