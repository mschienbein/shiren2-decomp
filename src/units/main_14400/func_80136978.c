#include "common.h"

typedef struct VTable VTable;
typedef struct { unsigned char pad0[0xC]; VTable *field0C; } Object;
extern VTable D_80151350;
extern void func_800D8FA8(void *object);

void func_80136978(Object *self, s32 flags)
{
    self->field0C = &D_80151350;
    if (flags & 1) {
        func_800D8FA8(self);
    }
}
