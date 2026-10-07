#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0x8]; void *vtable; } Obj8011E78C;
extern u8 D_80153AA0[];
void func_800AC68C(void *ptr);
void func_8011E78C(Obj8011E78C *obj, s32 flags) {
    obj->vtable = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
