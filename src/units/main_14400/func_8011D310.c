#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[0x8]; void *vtable_8; } Obj8011D310;
extern u8 D_80153AA0[];
void func_800AC68C(Obj8011D310 *obj);

void func_8011D310(Obj8011D310 *obj, s32 flags) {
    obj->vtable_8 = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
