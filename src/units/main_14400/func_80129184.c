#include "common.h"
typedef struct VTable VTable;
typedef struct Obj { unsigned char pad_00[8]; const VTable *vtable_08; } Obj;
extern const VTable D_80153AA0;
extern void func_800AC68C(void *a);
void func_80129184(Obj *self, s32 flags) {
    self->vtable_08 = &D_80153AA0;
    if (flags & 1) func_800AC68C(self);
}
