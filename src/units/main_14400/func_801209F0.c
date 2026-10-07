#include "common.h"

typedef struct { char pad0[0x8]; void *field_8; } Obj;

extern char D_8015F7F8[];
Obj *func_80114060(Obj *obj, s32 kind);

Obj *func_801209F0(Obj *obj) {
    func_80114060(obj, 0xA9);
    obj->field_8 = D_8015F7F8;
    return obj;
}
