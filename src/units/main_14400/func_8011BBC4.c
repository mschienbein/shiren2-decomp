#include "common.h"
typedef struct { unsigned char pad_00[8]; const void *field_08; } Object;
extern const unsigned char D_80153AA0[];
extern void func_800AC68C(void *object);
void func_8011BBC4(Object *self, s32 flags) { self->field_08 = D_80153AA0; if (flags & 1) func_800AC68C(self); }
