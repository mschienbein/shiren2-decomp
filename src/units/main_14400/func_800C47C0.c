#include "common.h"
typedef struct { s32 x; s32 y; } Pair;
typedef struct { unsigned char field_00[0x34]; s32 field_34; } Object;
extern s32 func_800C3114(Object *obj, Pair *x, Pair *y, Pair *first, Pair *second);
extern s32 func_80049CB4(s32 value, ...);
void func_800C47C0(Object *obj, Pair *x, Pair *y)
{
    Pair first;
    Pair second;
    if (func_800C3114(obj, x, y, &first, &second))
        func_80049CB4(obj->field_34, &first, &second);
}
