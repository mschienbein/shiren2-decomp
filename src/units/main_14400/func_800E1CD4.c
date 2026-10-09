#include "common.h"
typedef struct { unsigned char field_00[0x42]; unsigned char field_42; } Object;
extern u32 func_800E10D0(const Object *obj);
s32 func_800E1CD4(Object *obj, s32 value)
{
    s32 result = 0;
    if (obj->field_42) result = func_800E10D0(obj) == value;
    return result;
}
