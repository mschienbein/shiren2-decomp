#include "common.h"

typedef unsigned char u8;
typedef struct VTable VTable;

typedef struct {
    u8 pad00[0x24];
    VTable *vtable24;
} Obj_800EE358;

extern void func_800E016C(Obj_800EE358 *obj, s32 flags);
extern void func_800A3918(Obj_800EE358 *obj);
/* Opaque mixed adjustment/function-pointer table; only its address is used here. */
extern VTable D_801496E0;

/* Destructor: restore this class's vtable, run the base destructor, free on the delete bit. */
void func_801364F4(Obj_800EE358 *self, s32 flags)
{
    self->vtable24 = &D_801496E0;
    func_800E016C(self, 0);
    if (flags & 1) {
        func_800A3918(self);
    }
}
