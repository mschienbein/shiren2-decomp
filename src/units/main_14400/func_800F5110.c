#include "common.h"

typedef unsigned char u8;
typedef struct { u8 reserved_00[0xA]; u8 field_0A; u8 reserved_0B[0x14]; u8 field_1F; u8 reserved_20[4]; void *field_24; } Object;
extern u8 D_80159680[];
extern Object *func_800F4760(Object *self);
Object *func_800F5110(Object *self) {
    func_800F4760(self);
    self->field_24 = D_80159680;
    self->field_0A = 9;
    self->field_1F = 9;
    return self;
}
