#include "common.h"
typedef struct { unsigned char pad0[8]; void *vtable; } Obj8011F27C;
extern s32 D_80153AA0;
void func_800AC68C(void *obj);
void func_8011F27C(Obj8011F27C *obj, s32 flags) {
    obj->vtable = &D_80153AA0;
    if (flags & 1) func_800AC68C(obj);
}
