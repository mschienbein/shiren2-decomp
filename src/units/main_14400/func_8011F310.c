#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x8];
    void *field_8;
} Obj8011F310;

extern u8 D_8015F228[];

/* Returns the input object through this TU's partial view. */
Obj8011F310 *func_8011DA10(Obj8011F310 *obj, s32 kind);

Obj8011F310 *func_8011F310(Obj8011F310 *obj) {
    func_8011DA10(obj, 0x99);
    obj->field_8 = D_8015F228;
    return obj;
}
