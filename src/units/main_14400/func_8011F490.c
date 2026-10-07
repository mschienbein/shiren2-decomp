#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x8];
    void *field_8;
} Obj;

extern u8 D_8015F290[];
/* Returns the input object through this TU's partial view. */
Obj *func_80111530(Obj *obj, s32 kind);

Obj *func_8011F490(Obj *obj) {
    func_80111530(obj, 0x9A);
    obj->field_8 = D_8015F290;
    return obj;
}
