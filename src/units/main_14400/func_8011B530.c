#include "common.h"
typedef struct { unsigned char field_00[8]; const void *field_08; } Object;
extern const unsigned char D_80153AA0[];
extern void func_800AC68C(void *a);
void func_8011B530(Object *object, s32 flags) {
    object->field_08 = D_80153AA0;
    if (flags & 1) func_800AC68C(object);
}
