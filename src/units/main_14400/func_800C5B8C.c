#include "common.h"
typedef struct { unsigned char pad[0x28]; short field28; s32 (*field2C)(void *); } FirstMethods;
typedef struct { s32 field0; unsigned char pad4[8]; FirstMethods *fieldC; } First;
typedef struct { unsigned char pad[0x18]; short field18; void (*field1C)(void *, s32, s32); } SecondMethods;
typedef struct { unsigned char pad[0x18]; SecondMethods *field18; } Second;
void func_800C5B8C(First *first, Second *second) {
    FirstMethods *methods = first->fieldC;
    s32 value = methods->field2C((unsigned char *)first + methods->field28);
    SecondMethods *other = second->field18;
    other->field1C((unsigned char *)second + other->field18, value, first->field0);
}
