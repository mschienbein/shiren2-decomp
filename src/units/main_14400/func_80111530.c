#include "common.h"

typedef unsigned char u8;
typedef struct VTable VTable;
typedef struct Obj8011DEC0 { u8 pad_00[8]; VTable *field_08; u8 field_0C; u8 field_0D; } Obj8011DEC0;
typedef Obj8011DEC0 S;
extern VTable D_8015D5E0;
extern void *func_800AC0C0(S *self, s32 a, s32 b);
extern void func_80111608(Obj8011DEC0 *object);

Obj8011DEC0 *func_80111530(Obj8011DEC0 *object, s32 arg1) {
    func_800AC0C0(object, 7, arg1);
    object->field_08 = &D_8015D5E0;
    object->field_0D = 0;
    func_80111608(object);
    return object;
}
