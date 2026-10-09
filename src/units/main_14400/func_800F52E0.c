#include "common.h"

typedef unsigned char u8;
typedef struct VTable800F52E0 VTable800F52E0;

typedef struct {
    char pad0[0xA];
    u8 field_A;
    char padB[0x1F - 0xB];
    u8 field_1F;
    char pad20[4];
    VTable800F52E0 *vtable;
    char pad28[4];
} Obj800F52E0;

extern VTable800F52E0 D_80159760;

extern void *func_800A38FC(s32 size);
extern void *func_800F4760(Obj800F52E0 *obj);

/* Factory: allocate and construct a 0x2C-byte object of type 0xB. */
Obj800F52E0 *func_800F52E0(void) {
    Obj800F52E0 *self = func_800A38FC(sizeof(Obj800F52E0));
    func_800F4760(self);
    self->vtable = &D_80159760;
    self->field_A = 0xB;
    self->field_1F = 0xB;
    return self;
}
