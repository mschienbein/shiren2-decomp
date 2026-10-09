#include "common.h"
typedef struct { unsigned char pad0[8]; void *vtable8; } Object;
extern unsigned char D_80153AA0[];
extern void func_800AC68C(void *object);
void func_8011B7B8(Object *object, s32 flags) {
    object->vtable8 = D_80153AA0;
    if (flags & 1) func_800AC68C(object);
}
