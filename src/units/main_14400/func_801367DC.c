#include "common.h"



typedef struct {
    char pad0[0x4C];
    const void *vtable;
} Obj801367DC;

extern const unsigned char D_80151E38[144];

extern void func_800D8FA8(void *object);

/* Destructor: restore this class's vtable, free the storage when bit 0 is set. */
void func_801367DC(Obj801367DC *self, s32 flags) {
    self->vtable = D_80151E38;
    if (flags & 1) {
        func_800D8FA8(self);
    }
}
