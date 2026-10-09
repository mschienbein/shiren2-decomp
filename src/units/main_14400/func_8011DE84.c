#include "common.h"

typedef struct { s32 field_0; s32 field_4; const void *vtable_8; } Obj8011DE84;
extern const s32 D_80153AA0[];
void func_800AC68C(Obj8011DE84 *obj);

void func_8011DE84(Obj8011DE84 *obj, s32 flags) {
    obj->vtable_8 = &D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
