#include "common.h"

typedef short s16;

/* Slot 18 targets func_800E115C / derived dispatchers; its receiver adjustment is zero. */
typedef struct {
    s16 offset;
    s16 field_2;
    s32 (*method)(void *, s32, s32, unsigned char, s32);
} Method;

typedef struct {
    unsigned char field_0[0x24];
    Method *field_24;
} Object;

/* Virtual slot 18 with (0, 0xB, 0xFE, 0). */
void func_800E2C0C(Object *self) {
    self->field_24[18].method((unsigned char *)self + self->field_24[18].offset, 0, 0xB, 0xFE, 0);
}
