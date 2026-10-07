#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { s32 field_0; s32 field_4; void *vtable; } Obj;
extern u8 D_80153AA0[];
void func_800AC68C(Obj *obj);
void func_8011D62C(Obj *obj, s32 flags) {
    obj->vtable = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
