#include "common.h"
struct VTable;
typedef struct { unsigned char pad0[8]; const struct VTable *field_8; } Object;
extern const struct VTable D_80153AA0;
void func_800AC68C(void *self);
void func_80128F1C(Object *self, s32 flags) {
    self->field_8 = &D_80153AA0;
    if (flags & 1) func_800AC68C(self);
}
