#include "common.h"

typedef unsigned char u8;
typedef struct VTable VTable;
typedef struct { u8 pad00[0x18]; const VTable *vtable18; } Object;
extern const VTable D_80149F80;
extern void func_800D8FA8(void *object);

void func_801369D8(Object *self, s32 flags) {
    self->vtable18 = &D_80149F80;
    if (flags & 1) func_800D8FA8(self);
}
