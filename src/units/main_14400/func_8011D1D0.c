#include "common.h"

typedef struct { s32 field_0; s32 field_4; const void *vtable_8; } Obj8011D1D0;
extern const unsigned char D_8015ECA0[80];
Obj8011D1D0 *func_80112D20(Obj8011D1D0 *obj, s32 kind);
void func_800ACF34(Obj8011D1D0 *obj);

Obj8011D1D0 *func_8011D1D0(Obj8011D1D0 *obj) {
    func_80112D20(obj, 0x31);
    obj->vtable_8 = D_8015ECA0;
    func_800ACF34(obj);
    return obj;
}
