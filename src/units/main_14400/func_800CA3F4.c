#include "common.h"

typedef struct { s32 field_00; s32 field_04; } Pair;
typedef struct { s32 field_00; Pair field_04; } Obj;
void func_800CA3F4(Obj *obj, Pair *output)
{
    output->field_00 = obj->field_04.field_00;
    output->field_04 = obj->field_04.field_04;
}
