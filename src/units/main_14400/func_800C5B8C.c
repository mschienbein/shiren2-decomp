#include "common.h"
/* RNG object (as in func_800C5BE8): +0 state buffer pointer, vtable at +0x0C. Slot +0x2C is
 * the state size getter s32 (void *self) (func_800C5C9C/func_800C5DBC). */
typedef struct { unsigned char pad[0x28]; short field28; s32 (*field2C)(void *self); } FirstMethods;
typedef struct { void *field0; unsigned char pad4[8]; FirstMethods *fieldC; } First;
/* Stream write slot +0x18/+0x1C: void (receiver, s32 size, source pointer). */
typedef struct { unsigned char pad[0x18]; short field18; void (*field1C)(void *self, s32 size, void *source); } SecondMethods;
typedef struct { unsigned char pad[0x18]; SecondMethods *field18; } Second;
void func_800C5B8C(First *first, Second *second) {
    FirstMethods *methods = first->fieldC;
    s32 value = methods->field2C((unsigned char *)first + methods->field28);
    SecondMethods *other = second->field18;
    other->field1C((unsigned char *)second + other->field18, value, first->field0);
}
