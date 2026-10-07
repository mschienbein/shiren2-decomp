#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 x;
    s32 y;
} Pair;

typedef struct {
    Pair pair;
    u8 field_8;
    s32 field_C;
    s32 field_10;
} Obj;

void func_800C25D0(Obj *obj, Pair *pair, u8 *byte, s32 value)
{
    obj->pair = *pair;
    obj->field_8 = *byte;
    obj->field_C = value;
    obj->field_10 = 0;
}
