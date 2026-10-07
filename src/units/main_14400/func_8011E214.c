#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

extern s32 D_80153AA0;
typedef struct { u8 pad0[0x8]; s32 *vtable; } Obj8011E214;
void func_800AC68C(Obj8011E214 *obj);
void func_8011E214(Obj8011E214 *obj, s32 flags) {
    obj->vtable = &D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
