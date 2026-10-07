#include "common.h"
static inline s32 absolute(s32 value)
{
    if (value < 0) value = -value;
    return value;
}
s32 func_800A2424(s32 *arg)
{
    s32 y = absolute(arg[1]);
    s32 x = absolute(arg[0]);
    if (x < y) x = y;
    return x;
}
