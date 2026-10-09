#include "common.h"
typedef struct { s32 field_00[2]; const void *field_08; } Object;
extern const s32 D_80153AA0[];
extern void func_800AC68C(void *);
void func_80126C90(Object *self, s32 flags) {
    self->field_08 = &D_80153AA0;
    if (flags & 1) func_800AC68C(self);
}
