#include "common.h"

typedef struct { char pad0[0x8]; void *field_8; } Obj;

extern char D_8015EBB0[];
Obj *func_80112D20(Obj *obj, s32 kind);

Obj *func_8011CBD0(Obj *obj) {
    func_80112D20(obj, 0x2E);
    obj->field_8 = D_8015EBB0;
    return obj;
}
