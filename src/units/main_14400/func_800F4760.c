#include "common.h"

typedef unsigned char u8;
typedef struct { u8 reserved_00[0x1E]; u8 field_1E; u8 reserved_1F[5]; void *field_24; } Object;
extern u8 D_801595F0[];
extern Object *func_800A3850(Object *self);
Object *func_800F4760(Object *self) {
    func_800A3850(self);
    self->field_24 = D_801595F0;
    self->field_1E = 2;
    return self;
}
