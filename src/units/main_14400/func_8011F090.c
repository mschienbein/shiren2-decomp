#include "common.h"

typedef struct { char pad0[8]; void *vtable; } Obj;
extern char D_8015F158[];
Obj *func_80111530(Obj *obj, s32 kind);

Obj *func_8011F090(Obj *obj) {
    func_80111530(obj, 0x96);
    obj->vtable = D_8015F158;
    return obj;
}
