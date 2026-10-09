#include "common.h"
typedef struct VTable VTable;
typedef struct { unsigned char pad0[8]; const VTable *vtable8; } Object;
extern const VTable D_80153AA0;
extern void func_800AC68C(void *a);
void func_8011A5A8(Object *object, s32 flags) {
    object->vtable8 = &D_80153AA0;
    if (flags & 1) func_800AC68C(object);
}
