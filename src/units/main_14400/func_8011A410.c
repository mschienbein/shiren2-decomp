#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x8];
    void *field_8;
} Obj;

extern u8 D_8015E500[];
/* Returns the input object through this TU's partial view. */
Obj *func_80112D20(Obj *obj, s32 kind);

Obj *func_8011A410(Obj *obj) {
    func_80112D20(obj, 0x1A);
    obj->field_8 = D_8015E500;
    return obj;
}
