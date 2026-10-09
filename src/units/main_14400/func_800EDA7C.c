#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[0x60]; short offset_60; short pad_62; void (*method_64)(void *); } VTable;
typedef struct { s32 field_0; void *vtable_4; s32 x_8; s32 y_C; } Member800EBA54;
typedef struct {
    u8 pad_0[0x24]; VTable *field_24; u8 pad_28[0x60]; s32 field_88;
    u8 pad_8C[0x58]; short field_E4; short pad_E6; s32 field_E8; s32 field_EC;
    Member800EBA54 field_F0; void *field_100; void *field_104;
    u8 field_108, field_109, field_10A, field_10B;
} S;
typedef S Obj800EBA54;
extern void func_800EA7D4(S *s);
extern void func_800CFB90(Member800EBA54 *member, Obj800EBA54 *owner);
void func_800EDA7C(S *obj) {
    func_800EA7D4(obj);
    obj->field_E4 = 0;
    obj->field_E8 = 0;
    obj->field_EC = obj->field_88;
    func_800CFB90(&obj->field_F0, obj);
    obj->field_100 = 0;
    obj->field_104 = 0;
    obj->field_108 = 0;
    obj->field_109 = 0;
    obj->field_10A = 0;
    obj->field_10B = 0;
    obj->field_24->method_64((u8 *)obj + obj->field_24->offset_60);
}
