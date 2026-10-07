#include "common.h"

typedef struct { s32 pad0[2]; void *vtbl; } Obj;
extern Obj *func_80116D50(Obj *, s32);
extern s32 D_8015DDE8[];

Obj *func_80117F10(Obj *o) {
    func_80116D50(o, 8);
    o->vtbl = D_8015DDE8;
    return o;
}
