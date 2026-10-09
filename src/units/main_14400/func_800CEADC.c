#include "common.h"
struct VTable;
typedef struct { void *pool_0; const struct VTable *field_4; } Object;
extern const struct VTable D_80154390;
Object *func_800CEADC(Object *self) { self->field_4 = &D_80154390; return self; }
