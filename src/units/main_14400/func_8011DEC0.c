#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad0[8]; void *field_8; } Obj8011DEC0;
extern u8 D_8015EDF8[];
/* Returns the input object through this TU's partial view. */
Obj8011DEC0 *func_80111530(Obj8011DEC0 *obj, s32 arg1);

Obj8011DEC0 *func_8011DEC0(Obj8011DEC0 *obj) {
    func_80111530(obj, 0x8E);
    obj->field_8 = D_8015EDF8;
    return obj;
}
