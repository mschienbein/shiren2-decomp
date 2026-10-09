#include "common.h"
typedef struct { unsigned char pad0[0x1E]; unsigned char field_1E; unsigned char pad1F[0xD]; unsigned short field_2C, field_2E; } Obj;
/* Explicit signed-halfword result and word argument follow the original arithmetic. */
short func_800E0BD0(Obj *self, s32 amount) {
    s32 limit = 9999;
    s32 before;
    if (self->field_1E & 12) limit = 99;
    amount += self->field_2E;
    if (amount <= 0) amount = 1;
    else if (limit < amount) amount = limit;
    before = self->field_2E;
    self->field_2E = amount;
    before -= amount;
    if (amount < self->field_2C) self->field_2C = amount;
    return before;
}
