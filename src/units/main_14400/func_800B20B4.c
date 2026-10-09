#include "common.h"
typedef struct { s32 x; s32 y; } Pair;
extern s32 func_800B2068(void *obj, Pair *pair);
extern s32 func_800A23E8(void *obj, Pair *pair);

s32 func_800B20B4(void *obj, Pair *pair)
{
    Pair local;
    s32 different = func_800B2068(obj, pair) ^ 1;
    if (different) {
        Pair *copy = &local;
        copy->x = pair->x;
        copy->y = pair->y;
        if (func_800A23E8(obj, copy) >= 2) return 0;
    }
    return 1;
}
