#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef struct { s32 field_0; s32 field_4; void *vtable_8; } Obj8011D9E0;
extern u8 D_80153AA0[];
void func_800AC68C(Obj8011D9E0 *obj);

void func_8011D9E0(Obj8011D9E0 *obj, s32 flags) {
    obj->vtable_8 = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
