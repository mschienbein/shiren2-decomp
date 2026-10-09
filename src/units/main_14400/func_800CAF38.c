#include "common.h"
typedef struct { s32 field_0[7]; s32 *field_1C; } Object;
extern void func_800CAEF4(Object *);
extern void func_800CA0A8(s32 *, s32);
void func_800CAF38(Object *self) { s32 value = self->field_1C[0]; func_800CAEF4(self); func_800CA0A8(self->field_1C, value); }
