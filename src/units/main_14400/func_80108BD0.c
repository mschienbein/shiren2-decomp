#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct VTable VTable;
typedef struct Obj80108BD0 Obj80108BD0;

extern Obj80108BD0 *func_800EFC70(Obj80108BD0 *obj, s32 arg1, u8 arg2);
extern VTable D_8015C5D0;

/* Kind 0x56 actor: base actor (func_800EFC70) with extra state flags set. */
struct Obj80108BD0 {
    char pad00[0x20];
    u32 flags20;
    VTable *vtable24;
    char pad28[0x90 - 0x28];
    u32 flags90;
    char pad94[0x9A - 0x94];
    u16 flags9A;
};

Obj80108BD0 *func_80108BD0(Obj80108BD0 *self, u8 value) {
    func_800EFC70(self, 0x56, value);
    self->vtable24 = &D_8015C5D0;
    self->flags9A |= 1;
    self->flags90 |= 0x2000000;
    self->flags20 = self->flags90;
    return self;
}
