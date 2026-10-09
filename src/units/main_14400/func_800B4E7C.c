#include "common.h"
typedef struct { short offset; short field_2; void (*method)(void *, s32); } Method;
typedef struct { s32 field_0[2]; Method *field_8; } Object;
extern Object *func_800B4E18(void *);
void func_800B4E7C(void *position) { Object *self = func_800B4E18(position); if (self) self->field_8[1].method((unsigned char *)self + self->field_8[1].offset, 3); }
