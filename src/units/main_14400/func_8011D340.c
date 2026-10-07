#include "common.h"

typedef struct { s32 pad0[2]; void *vtbl; } Obj;
extern Obj *func_80112470(Obj *, s32);
extern s32 D_80148F90[];

Obj *func_8011D340(Obj *o) {
    func_80112470(o, 0x76);
    o->vtbl = D_80148F90;
    return o;
}
