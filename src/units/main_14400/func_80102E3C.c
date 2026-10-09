#include "common.h"

typedef unsigned char u8;

typedef struct VTable VTable;
typedef struct Obj80102E3C Obj80102E3C;

extern Obj80102E3C *func_800EFC70(Obj80102E3C *obj, s32 arg1, u8 arg2);
extern VTable D_8015B8F8;

/* Kind 0x41 actor: base actor (func_800EFC70) plus two cleared state words. */
struct Obj80102E3C {
    char pad00[0x24];
    VTable *vtable24;
    char pad28[0xA0 - 0x28];
    s32 fieldA0;
    s32 fieldA4;
};

Obj80102E3C *func_80102E3C(Obj80102E3C *self, u8 value) {
    func_800EFC70(self, 0x41, value);
    self->vtable24 = &D_8015B8F8;
    self->fieldA0 = 0;
    self->fieldA4 = 0;
    return self;
}
