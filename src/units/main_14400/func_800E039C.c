#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 pad_00[0x33]; u8 field_33; u8 pad_34[0x1C]; u16 field_50; } Object;
extern u16 D_80158C6C[];
void func_800E039C(Object *object, s32 value) {
    u16 duration = D_80158C6C[(u8)value];
    object->field_33 = value;
    object->field_50 = duration;
}
