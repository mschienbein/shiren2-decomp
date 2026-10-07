#include "common.h"

typedef struct { s32 pad0[2]; void *vtbl; } Obj;
extern Obj *func_80115690(Obj *, s32);
extern s32 D_80160298[];

Obj *func_80125820(Obj *o) {
    func_80115690(o, 0xE1);
    o->vtbl = D_80160298;
    return o;
}
