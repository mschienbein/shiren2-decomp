#include "common.h"
typedef struct Desc Desc;
typedef struct { unsigned char pad_00[0x50]; short field_50; unsigned char pad_52[0x22]; s32 field_74; } Obj;
void func_8009543C(Obj *object, Desc *descriptor);
void func_8009C338(Obj *object, short value, Desc *descriptor)
{
    func_8009543C(object, descriptor);
    object->field_50 = value;
    object->field_74 = 0;
}
