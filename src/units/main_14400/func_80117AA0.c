#include "common.h"

typedef struct { char pad0[0x8]; void *field_8; } Obj;

extern char D_8015DCE0[];
Obj *func_80116D50(Obj *obj, s32 kind);

Obj *func_80117AA0(Obj *obj) {
    func_80116D50(obj, 0x5);
    obj->field_8 = D_8015DCE0;
    return obj;
}
