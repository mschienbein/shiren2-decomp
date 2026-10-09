#include "common.h"

typedef unsigned char u8;

typedef struct VTable VTable;
typedef struct Obj800FCE80 Obj800FCE80;

extern Obj800FCE80 *func_800EFC70(Obj800FCE80 *obj, s32 arg1, u8 arg2);
extern VTable D_8015A700;

/* Kind 0x29 actor: base actor (func_800EFC70) plus two cleared state fields. */
struct Obj800FCE80 {
    char pad00[0x24];
    VTable *vtable24;
    char pad28[0xA0 - 0x28];
    s32 fieldA0;
    u8 fieldA4;
};

Obj800FCE80 *func_800FCE80(Obj800FCE80 *self, u8 value) {
    func_800EFC70(self, 0x29, value);
    self->vtable24 = &D_8015A700;
    self->fieldA0 = 0;
    self->fieldA4 = 0;
    return self;
}
