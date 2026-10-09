#include "common.h"

typedef struct VTable800D9E5C VTable800D9E5C;

typedef struct {
    char pad0[4];
    VTable800D9E5C *vtable;
} Obj800D9E5C;

extern VTable800D9E5C D_80157FA8;

extern void func_800D8FE8(void *object);

/* Destructor: restore this class's vtable, free the storage when bit 0 is set. */
void func_800D9E5C(Obj800D9E5C *self, s32 flags) {
    self->vtable = &D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(self);
    }
}
