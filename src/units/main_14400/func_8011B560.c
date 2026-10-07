#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad0[8]; void *field_8; } Obj8011B560;
extern u8 D_8015E7F0[];
/* Returns the input object through this TU's partial view. */
Obj8011B560 *func_80112D20(Obj8011B560 *obj, s32 arg1);

Obj8011B560 *func_8011B560(Obj8011B560 *obj) {
    func_80112D20(obj, 0x22);
    obj->field_8 = D_8015E7F0;
    return obj;
}
