#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_00[0x50]; unsigned short field_50; } Object;
extern short D_80158C6C[];
extern s32 func_800E04D0(Object *object);
void func_800E4D44(Object *object) {
    s32 duration = D_80158C6C[(u8)func_800E04D0(object)];
    object->field_50 += duration;
}
