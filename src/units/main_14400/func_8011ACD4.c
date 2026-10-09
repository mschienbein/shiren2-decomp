#include "common.h"

typedef struct VTable VTable;
typedef struct { unsigned char pad0[8]; VTable *field08; } Object;
extern VTable D_80153AA0;
extern void func_800AC68C(void *a);

void func_8011ACD4(Object *self, s32 flags)
{
    self->field08 = &D_80153AA0;
    if (flags & 1) {
        func_800AC68C(self);
    }
}
