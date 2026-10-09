#include "common.h"
typedef unsigned char u8;
/* Embedded item-set member (family D_80154300, vptr at member+4): slot +0x1C is void (void *self, s32 value). */
typedef struct { u8 pad_0[0x18]; short offset_18; short pad_1A; void (*method_1C)(void *self, s32 value); } MemberVTable;
typedef struct { void *field_0; MemberVTable *field_4; u8 pad_8[0x10]; } Member;
typedef struct { u8 pad_0[0xA0]; short offset_A0; short pad_A2; void *(*method_A4)(void *, u8); } VTable;
typedef struct {
    u8 pad_0[0x24]; VTable *field_24; u8 pad_28[0x50]; s32 field_78;
    u8 pad_7C[0x3C]; short field_B8; u8 pad_BA[0xA]; Member field_C4; u8 field_DC;
} Obj;
extern void func_800CD468(void *list);
extern void func_800E946C(void *obj, short count);
void func_8010A0F4(void *arg, s32 value) {
    Obj *obj = arg;
    Member *member = &obj->field_C4;
    member->field_4->method_1C((u8 *)member + member->field_4->offset_18, 4);
    func_800CD468(member);
    obj->field_DC = 0;
    obj->field_B8 = 0x16A;
    obj->field_78 = *(s32 *)obj->field_24->method_A4((u8 *)obj + obj->field_24->offset_A0, (u8)value);
    func_800E946C(obj, (u8)value - 1);
}
