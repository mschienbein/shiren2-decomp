#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x8];
    void *field_8;
} Obj;

extern u8 D_8015EC50[];
/* Returns the input object through this TU's partial view. */
Obj *func_80112D20(Obj *obj, s32 kind);

Obj *func_8011CFF0(Obj *obj) {
    func_80112D20(obj, 0x30);
    obj->field_8 = D_8015EC50;
    return obj;
}
