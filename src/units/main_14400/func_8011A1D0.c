#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x8];
    void *field_8;
} Obj8011A1D0;

extern u8 D_8015E4B0[];

/* Returns the input object through this TU's partial view. */
Obj8011A1D0 *func_80112D20(Obj8011A1D0 *obj, s32 kind);

Obj8011A1D0 *func_8011A1D0(Obj8011A1D0 *obj) {
    func_80112D20(obj, 0x19);
    obj->field_8 = D_8015E4B0;
    return obj;
}
