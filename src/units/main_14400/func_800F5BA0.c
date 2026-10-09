#include "common.h"

typedef unsigned char u8;
typedef struct VTable VTable;
typedef struct { u8 pad00[0xA]; u8 field0A; u8 pad0B[0x14]; u8 field1F; u8 pad20[4]; const VTable *vtable24; u8 pad28[4]; } Object;
extern const VTable D_80149540;
extern void *func_800A38FC(s32 size);
extern void *func_800F4760(Object *);

Object *func_800F5BA0(void) {
    Object *self = func_800A38FC(0x2C);
    func_800F4760(self);
    self->vtable24 = &D_80149540;
    self->field0A = 0x16;
    self->field1F = 0x16;
    return self;
}
