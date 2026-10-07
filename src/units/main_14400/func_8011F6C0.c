#include "common.h"

typedef struct { s32 pad0[2]; void *vtbl; } Obj;
/* Returns the input object through this TU's partial view. */
extern Obj *func_80111530(Obj *, s32);
extern s32 D_8015F308[];

Obj *func_8011F6C0(Obj *o) {
    func_80111530(o, 0x9B);
    o->vtbl = D_8015F308;
    return o;
}
