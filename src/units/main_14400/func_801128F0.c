#include "common.h"

typedef struct VTable VTable;
typedef struct { unsigned char pad0[8]; VTable *field08; } Object;
extern VTable D_8015D6A0;
extern void *func_800AC0C0(Object *self, s32 a, s32 b);

Object *func_801128F0(Object *self, s32 value)
{
    func_800AC0C0(self, 0x11, value);
    self->field08 = &D_8015D6A0;
    return self;
}
