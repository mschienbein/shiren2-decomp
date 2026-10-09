#include "common.h"

typedef unsigned char u8;
typedef struct {
    u8 pad_00[0x28];
    u8 state_28[0x1E];
    u8 pad_46[2];
    u8 state_48[0x10];
    s32 field_58, field_5C, field_60, field_64, field_68, field_6C;
    u8 field_70, field_71, flags_72, field_73, field_74, field_75, field_76;
} Object;
extern u32 D_8013960C;
extern void func_800E039C(Object *object, s32 value);
extern void func_800E0508(Object *object, s32 a, s32 b);

void func_800E01F0(void *self)
{
    Object *object = self;
    u8 *p = object->state_28;
    s32 count = 0x1D;
    D_8013960C <<= 1;
    do {
        *p++ = 0;
    } while (--count != -1);
    p = object->state_48;
    count = 0xF;
    do {
        *p++ = 0;
    } while (--count != -1);
    object->field_58 = 0;
    object->field_5C = 0;
    object->field_60 = 0;
    object->field_64 = 0;
    object->field_68 = 0;
    object->field_6C = 0;
    object->field_71 = 0;
    object->field_70 = 0;
    object->flags_72 = 0;
    object->field_73 = 3;
    object->field_74 = 0;
    object->field_75 = 0;
    object->field_76 = 0;
    func_800E039C(object, 1);
    func_800E0508(object, 1, 1);
    object->flags_72 |= 8;
    D_8013960C >>= 1;
}
