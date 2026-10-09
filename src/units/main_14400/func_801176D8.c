#include "common.h"
typedef struct { s32 field_00[2]; void *field_08; } Object;
extern s32 D_80153AA0[];
extern void func_800AC68C(Object *obj);
void func_801176D8(Object *obj, s32 flags)
{
    obj->field_08 = D_80153AA0;
    if (flags & 1) func_800AC68C(obj);
}
