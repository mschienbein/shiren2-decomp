#include "common.h"

typedef unsigned char u8;
typedef struct VTable800FB2C0 VTable800FB2C0;

typedef struct {
    char pad0[0x24];
    VTable800FB2C0 *vtable;
} Obj800FB2C0;

extern VTable800FB2C0 D_8015A130;

extern void *func_800A38FC(s32 size);
extern Obj800FB2C0 *func_800EFC70(Obj800FB2C0 *obj, s32 arg1, u8 arg2);

/* Factory/constructor: kind 0x21 object in the supplied storage or a new 0xA0 block. */
Obj800FB2C0 *func_800FB2C0(u8 kind, Obj800FB2C0 *self) {
    if (self == 0) {
        self = func_800A38FC(0xA0);
    }
    func_800EFC70(self, 0x21, kind);
    self->vtable = &D_8015A130;
    return self;
}
