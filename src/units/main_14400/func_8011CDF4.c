#include "common.h"
typedef struct { unsigned char pad_00[8]; const void *vtable_08; } Obj8011CDF4;
extern const unsigned char D_80153AA0[];
extern void func_800AC68C(void *object);
void func_8011CDF4(Obj8011CDF4 *self, s32 flags) {
    self->vtable_08 = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(self);
    }
}
