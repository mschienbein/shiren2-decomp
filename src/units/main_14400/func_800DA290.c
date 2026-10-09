#include "common.h"

typedef struct VTable VTable;
typedef struct { unsigned char pad0[4]; VTable *field04; } Object;
extern VTable D_80157FA8;
extern void func_800D8FE8(void *object);

void func_800DA290(Object *self, s32 flags)
{
    self->field04 = &D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(self);
    }
}
