#include "common.h"

typedef struct VTable VTable;
typedef struct { s32 field00; const VTable *vtable04; } Object;
extern const VTable D_80157FA8;
extern void func_800D8FE8(void *object);

void func_800DA3B0(Object *self, s32 flags) {
    self->vtable04 = &D_80157FA8;
    if (flags & 1) func_800D8FE8(self);
}
