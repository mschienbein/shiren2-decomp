#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x8];
    void *field_8;
} Obj8011CE30;

extern u8 D_8015EC00[];

/* Returns the input object through this TU's partial view. */
Obj8011CE30 *func_80112D20(Obj8011CE30 *obj, s32 kind);

Obj8011CE30 *func_8011CE30(Obj8011CE30 *obj) {
    func_80112D20(obj, 0x2F);
    obj->field_8 = D_8015EC00;
    return obj;
}
