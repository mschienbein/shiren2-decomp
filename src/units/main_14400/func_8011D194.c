#include "common.h"

typedef unsigned char u8;

typedef struct { s32 field_0; s32 field_4; void *handler; } Obj8011D194;

extern u8 D_80153AA0[];
void func_800AC68C(Obj8011D194 *obj);

void func_8011D194(Obj8011D194 *obj, s32 flags) {
    obj->handler = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
