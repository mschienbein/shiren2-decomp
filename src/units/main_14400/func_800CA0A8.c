#include "common.h"
typedef struct { unsigned char pad_00[8]; short offset_08; short field_0A; void (*method_0C)(void *); } Methods;
typedef struct { s32 field_00; unsigned char pad_04[0x14]; Methods *field_18; } Object;
void func_800CA0A8(Object *object, s32 value) {
    Methods *methods = object->field_18;
    methods->method_0C((unsigned char *)object + methods->offset_08);
    object->field_00 = value;
}
