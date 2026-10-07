#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0xA];
    u8 field_A;
    u8 padB[0x1F - 0xB];
    u8 field_1F;
    u8 pad20[0x4];
    void *vtable_24;
} Obj800F5258;
extern u8 D_801596F0[];
Obj800F5258 *func_800F4760(Obj800F5258 *obj);

Obj800F5258 *func_800F5258(Obj800F5258 *obj) {
    Obj800F5258 *self = obj;

    func_800F4760(obj);
    self->vtable_24 = D_801596F0;
    self->field_A = 10;
    self->field_1F = 10;
    return self;
}
