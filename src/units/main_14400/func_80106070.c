#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct VTable VTable;
typedef struct Obj80106070 Obj80106070;

extern Obj80106070 *func_800EFC70(Obj80106070 *obj, s32 arg1, u8 arg2);
extern void func_800A5A88(Obj80106070 *obj, s32 value);
extern VTable D_8015BEF8;

/* Kind 0x49 actor: base actor (func_800EFC70), func_800A5A88 mode 2, flag bit 0. */
struct Obj80106070 {
    char pad00[0x24];
    VTable *vtable24;
    char pad28[0x9A - 0x28];
    u16 flags9A;
};

Obj80106070 *func_80106070(Obj80106070 *self, u8 value) {
    func_800EFC70(self, 0x49, value);
    self->vtable24 = &D_8015BEF8;
    func_800A5A88(self, 2);
    self->flags9A |= 1;
    return self;
}
