#include "common.h"

typedef unsigned char u8;

typedef unsigned short u16;

typedef struct {
    u8 pad0[0x24];
    void *vtable24;
    u8 pad28[0x72];
    u16 flags9A;
    u8 pad9C[4];
    s32 activeA0;
    s32 valueA4;
    s32 valueA8;
} Obj801073B0;

extern u8 D_8015C2C8[];
extern void *func_800EFC70(Obj801073B0 *obj, s32 kind, u8 arg2);
extern u8 func_801E9F70(s32 id);
extern void func_800E4D88(Obj801073B0 *obj, s32 arg1);
extern void func_800E4D90(Obj801073B0 *obj, s32 arg1);

Obj801073B0 *func_801073B0(Obj801073B0 *obj, u8 arg1) {
    Obj801073B0 *self = obj;

    func_800EFC70(obj, 0x51, arg1);
    self->vtable24 = D_8015C2C8;
    if (func_801E9F70(0xE)) {
        self->activeA0 = 1;
    } else {
        self->activeA0 = 0;
    }
    self->valueA4 = 0;
    self->valueA8 = 0;
    func_800E4D88(self, 3);
    func_800E4D90(self, 2);
    self->flags9A |= 1;
    return self;
}
