#include "common.h"
typedef struct VTable VTable;
typedef struct { s32 field_00; void *field_04; s32 field_08; const VTable *field_0C; } Iterator;
extern const VTable D_80151350;
Iterator *func_80091BF8(Iterator *self, void *container) {
    self->field_0C = &D_80151350;
    self->field_04 = container;
    return self;
}
