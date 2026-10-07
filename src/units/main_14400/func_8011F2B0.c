#include "common.h"

typedef struct { char pad0[0x8]; void *field_8; } Obj;

extern char D_80149050[];
Obj *func_8011DA10(Obj *obj, s32 kind);

Obj *func_8011F2B0(Obj *obj) {
    func_8011DA10(obj, 0x98);
    obj->field_8 = D_80149050;
    return obj;
}
