#include "common.h"
typedef struct { s32 field_0, field_4; const void *field_8; } Object;
extern const s32 D_80153AA0[];
extern void func_800AC68C(Object *);
void func_801256DC(Object *self, s32 flags) { self->field_8 = &D_80153AA0; if (flags & 1) func_800AC68C(self); }
