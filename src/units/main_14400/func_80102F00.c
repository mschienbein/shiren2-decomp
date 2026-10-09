#include "common.h"

typedef unsigned char u8;

typedef struct VTable VTable;
typedef struct Obj80102F00 Obj80102F00;

extern Obj80102F00 *func_800EFC70(Obj80102F00 *obj, s32 arg1, u8 arg2);
extern void func_800E4D90(Obj80102F00 *obj, s32 value);
extern VTable D_8015B9B8;

/* Kind 0x42 actor: base actor (func_800EFC70), mode 3, two cleared state words. */
struct Obj80102F00 {
    char pad00[0x24];
    VTable *vtable24;
    char pad28[0xA0 - 0x28];
    s32 fieldA0;
    s32 fieldA4;
};

Obj80102F00 *func_80102F00(Obj80102F00 *self, u8 value) {
    func_800EFC70(self, 0x42, value);
    self->vtable24 = &D_8015B9B8;
    func_800E4D90(self, 3);
    self->fieldA0 = 0;
    self->fieldA4 = 0;
    return self;
}
