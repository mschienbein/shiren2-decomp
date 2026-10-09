#include "common.h"

typedef struct { s32 field_0; void *vtable_4; } Obj;

extern Obj *func_800DDAD0(Obj *obj, s32 size);
extern char D_801589E8[];

Obj *func_800DEBC0(Obj *obj) {
    func_800DDAD0(obj, 0x2C);
    obj->vtable_4 = D_801589E8;
    return obj;
}
