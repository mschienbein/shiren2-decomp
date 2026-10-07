#include "common.h"

typedef struct {
    s32 x;
    s32 y;
} Pair;

typedef struct {
    char pad0[0x84];
    Pair first;
    Pair second;
} Obj;

void func_800F61B4(Obj *obj, Pair *first, Pair *second)
{
    obj->first = *first;
    obj->second = *second;
}
