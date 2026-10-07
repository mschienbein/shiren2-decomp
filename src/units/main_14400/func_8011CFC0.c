#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct { s32 field_0; s32 field_4; void *vtable; } Obj8011CFC0;
extern u8 D_80153AA0[];
void func_800AC68C(void *obj);
void func_8011CFC0(Obj8011CFC0 *obj, s32 flags) {
    obj->vtable = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
