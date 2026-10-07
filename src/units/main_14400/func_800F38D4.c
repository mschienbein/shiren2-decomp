#include "common.h"
typedef struct { unsigned char pad_00[0x38]; short offset_38; short field_3A; void *(*method_3C)(void *, u32); } Methods;
typedef struct { s32 field_00; Methods *field_04; } Child;
typedef struct { unsigned char pad_00[0x8C]; Child *field_8C; } Object;
void func_800F38D4(Object *object) {
    Child *child = object->field_8C;
    Methods *methods = child->field_04;
    methods->method_3C((unsigned char *)child + methods->offset_38, 0);
}
