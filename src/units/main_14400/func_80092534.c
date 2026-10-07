#include "common.h"

/* +0x4 holds a descriptor pointer (descriptor+0 row count, descriptor+4 array of
 * 0x14-byte rows) read by the base operations func_80091A1C/func_80091B30;
 * the descriptor stays opaque in this constructor. */
typedef struct {
    s32 field_0;
    void *descriptor;
    s32 field_8;
    void *vtable;
} Obj;

extern u32 D_80151350[];
extern u32 D_80151498[];

Obj *func_80092534(Obj *self, void *descriptor) {
    self->vtable = D_80151350;
    self->descriptor = descriptor;
    self->vtable = D_80151498;
    return self;
}
