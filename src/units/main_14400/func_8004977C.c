#include "common.h"
typedef struct { unsigned char field_00[0xC]; short field_0C, field_0E, field_10; unsigned char field_12, field_13; unsigned char field_14[0x20]; unsigned char field_34, field_35, field_36, field_37; s32 field_38; } Object;
extern Object *func_8007946C(s32 type, s32 index);
extern void func_80079560(s32 type, s32 index, s32 mode);
void func_8004977C(s32 index, s32 value)
{
    Object *obj = func_8007946C(4, index);
    obj->field_0C = obj->field_0E = obj->field_10 = 0;
    func_80079560(4, index, 1);
    obj->field_34 = 1;
    obj->field_37 = 1;
    obj->field_12 = 0;
    obj->field_13 = value;
    obj->field_35 = 0;
    obj->field_38 = 0;
}
