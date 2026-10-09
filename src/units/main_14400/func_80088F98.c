#include "common.h"
typedef unsigned char u8;
typedef struct {
    u8 field_00[4]; short field_04; u8 field_06[8]; short field_0E;
    u8 field_10[4]; s32 field_14; u8 field_18[12]; s32 field_24;
} Object;
extern void func_800766A8(s32 index, s32 value);
void func_80088F98(Object *object) {
    func_800766A8(object->field_14, object->field_24);
    object->field_0E = 1;
    object->field_04 = 4;
}
