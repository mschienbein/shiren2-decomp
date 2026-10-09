#include "common.h"
/* Slot 18 targets func_800E115C / derived dispatchers; its receiver adjustment is zero. */
typedef struct { short offset; short field_2; s32 (*method)(void *, s32, s32, unsigned char, s32); } Method;
typedef struct { unsigned char field_0[0x24]; Method *field_24; } Object;
void func_800E27FC(Object *self, signed char value) {
    Method *method = &self->field_24[18];
    void *target = (unsigned char *)self + method->offset;
    s32 code = 0x11;
    if (value > 0) code = 0x12;
    method->method(target, 0, code, 0xFE, value);
}
