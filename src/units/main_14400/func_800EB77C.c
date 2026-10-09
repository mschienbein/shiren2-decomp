#include "common.h"
typedef struct { unsigned char field_00[0x84]; s32 field_84; } Object;
void func_800EB77C(Object *obj, unsigned short percent)
{
    u32 value = (obj->field_84 * percent) / 100U;
    if (value > 999999U) value = 999999;
    obj->field_84 = value;
}
