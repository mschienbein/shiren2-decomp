#include "common.h"

typedef struct { char pad0[8]; void *vtable; } Obj;
extern char D_8015F758[];
Obj *func_80114060(Obj *obj, s32 kind);

Obj *func_80120830(Obj *obj) {
    func_80114060(obj, 0xA7);
    obj->vtable = D_8015F758;
    return obj;
}
