#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[0x4C]; void *vtable_4C; u8 pad50[0xC]; } Part8009CA0C;
typedef struct {
    Part8009CA0C base_0;
    Part8009CA0C part_5C;
    u8 padB8[0xC];
    s32 field_C4;
    void *vtable_C8;
} Obj8009CA0C;
extern u8 D_80152968[];
extern u8 D_80152AE8[];
extern u8 D_80151EC8[];
Part8009CA0C *func_800953C0(Part8009CA0C *part);

Obj8009CA0C *func_8009CA0C(Obj8009CA0C *obj) {
    Obj8009CA0C *self = obj;
    Part8009CA0C *part;

    func_800953C0(&obj->base_0);
    obj->base_0.vtable_4C = D_80152968;
    part = &obj->part_5C;
    func_800953C0(part);
    part->vtable_4C = D_80152AE8;
    self->field_C4 = -1;
    self->vtable_C8 = D_80151EC8;
    return self;
}
