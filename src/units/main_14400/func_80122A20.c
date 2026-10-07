#include "common.h"

typedef struct { s32 pad0[2]; void *vtbl; } Obj;
extern Obj *func_80114060(Obj *, s32);
extern s32 D_8015F9E0[];

Obj *func_80122A20(Obj *o) {
    func_80114060(o, 0xAE);
    o->vtbl = D_8015F9E0;
    return o;
}
