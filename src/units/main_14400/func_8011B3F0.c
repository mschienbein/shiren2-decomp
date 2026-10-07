#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef struct { s32 field_0; s32 field_4; void *vtable_8; } Obj8011B3F0;
extern u8 D_8015E7A0[];
Obj8011B3F0 *func_80112D20(Obj8011B3F0 *obj, s32 kind);
void func_800ACF34(Obj8011B3F0 *obj);

Obj8011B3F0 *func_8011B3F0(Obj8011B3F0 *obj) {
    func_80112D20(obj, 0x21);
    obj->vtable_8 = D_8015E7A0;
    func_800ACF34(obj);
    return obj;
}
