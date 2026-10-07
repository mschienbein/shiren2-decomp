#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad0[8]; void *field_8; } Obj8011FD60;
extern u8 D_8015F510[];
/* Returns the input object through this TU's partial view. */
Obj8011FD60 *func_80111530(Obj8011FD60 *obj, s32 arg1);

Obj8011FD60 *func_8011FD60(Obj8011FD60 *obj) {
    func_80111530(obj, 0x9F);
    obj->field_8 = D_8015F510;
    return obj;
}
