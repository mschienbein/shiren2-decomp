#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct VTable VTable;
typedef struct Obj80103EC0 Obj80103EC0;

extern Obj80103EC0 *func_800EFC70(Obj80103EC0 *obj, s32 arg1, u8 arg2);
extern void func_800E4D88(Obj80103EC0 *obj, s32 value);
extern void func_800E4D90(Obj80103EC0 *obj, s32 value);
extern VTable D_8015BB38;

/* Kind 0x44 actor: base actor (func_800EFC70), modes 4/3, cleared state, flag bit 0. */
struct Obj80103EC0 {
    char pad00[0x24];
    VTable *vtable24;
    char pad28[0x9A - 0x28];
    u16 flags9A;
    char pad9C[0xA0 - 0x9C];
    s32 fieldA0;
};

Obj80103EC0 *func_80103EC0(Obj80103EC0 *self, u8 value) {
    func_800EFC70(self, 0x44, value);
    self->vtable24 = &D_8015BB38;
    func_800E4D88(self, 4);
    func_800E4D90(self, 3);
    self->fieldA0 = 0;
    self->flags9A |= 1;
    return self;
}
