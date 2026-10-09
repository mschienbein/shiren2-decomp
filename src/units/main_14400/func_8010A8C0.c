#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef struct { u8 pad_00[0xA0]; s16 delta_A0; s16 index_A2; void *(*method_A4)(void *, u8); } Methods;
typedef struct { u8 pad_00[0x1C]; unsigned short field_1C; u8 pad_1E[6]; Methods *field_24; u8 pad_28[0x50]; s32 field_78; u8 pad_7C[0x3C]; unsigned short field_B8; u8 pad_BA[6]; s32 field_C0, field_C4, field_C8, field_CC, field_D0; } Obj;
extern void func_800E946C(void *obj, s16 count);
void func_8010A8C0(void *self, u8 value)
{
    Obj *obj = self;
    obj->field_B8 = 0x173;
    obj->field_1C |= 4;
    obj->field_78 = *(s32 *)obj->field_24->method_A4((u8 *)obj + obj->field_24->delta_A0, value);
    func_800E946C(obj, value - 1);
    obj->field_C0 = 0;
    obj->field_C4 = 0;
    obj->field_C8 = 0;
    obj->field_CC = 0;
    obj->field_D0 = 0;
}
