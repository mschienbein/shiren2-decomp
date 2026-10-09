#include "common.h"
typedef struct VTable VTable;
typedef struct { unsigned char pad_00[8]; const VTable *field_08; } Object;
extern const VTable D_80153AA0;
extern void func_800AC68C(void *object);
void func_8011C524(Object *object, s32 flags) {
    object->field_08 = &D_80153AA0;
    if (flags & 1) func_800AC68C(object);
}
