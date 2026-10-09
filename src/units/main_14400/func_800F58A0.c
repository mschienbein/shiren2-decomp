#include "common.h"


typedef struct {
    unsigned char pad0[0xA];
    unsigned char field0A;
    unsigned char pad0B[0x14];
    unsigned char field1F;
    unsigned char pad20[4];
    const void *field24;
} Object;
extern const unsigned char D_80159850[112];
extern void *func_800A38FC(s32 size);
extern void *func_800F4760(Object *);

Object *func_800F58A0(void)
{
    Object *self = func_800A38FC(0x2C);
    func_800F4760(self);
    self->field24 = D_80159850;
    self->field0A = 0x10;
    self->field1F = 0x10;
    return self;
}
