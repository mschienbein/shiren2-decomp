#include "common.h"

typedef struct { char pad0[8]; void *vtable; } Obj;
extern char D_8015EB10[];
Obj *func_80112D20(Obj *obj, s32 kind);

Obj *func_8011C790(Obj *obj) {
    func_80112D20(obj, 0x2C);
    obj->vtable = D_8015EB10;
    return obj;
}
