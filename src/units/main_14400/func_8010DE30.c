#include "common.h"

typedef struct VTable8010DE30 VTable8010DE30;

typedef struct {
    char pad0[8];
    VTable8010DE30 *vtable;
} Obj8010DE30;

extern VTable8010DE30 D_8015D2B0;

extern void *func_800AC0C0(void *self, s32 a, s32 b);

/* Constructor: base init with kind 10, then install this class's vtable. */
Obj8010DE30 *func_8010DE30(Obj8010DE30 *self, s32 arg) {
    func_800AC0C0(self, 0xA, arg);
    self->vtable = &D_8015D2B0;
    return self;
}
