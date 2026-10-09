#include "common.h"
typedef struct { unsigned char pad00[8]; const void *vtable; } Object;
extern const unsigned char D_80153AA0[];
extern void func_800AC68C(void *object);
void func_8011C400(Object *object, s32 flags) {
    object->vtable = D_80153AA0;
    if (flags & 1) func_800AC68C(object);
}
