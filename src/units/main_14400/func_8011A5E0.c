#include "common.h"

typedef struct { s32 pad0[2]; void *vtbl; } Obj;
/* Returns the input object through this TU's partial view. */
extern Obj *func_80112D20(Obj *, s32);
extern s32 D_8015E550[];

Obj *func_8011A5E0(Obj *o) {
    func_80112D20(o, 0x1B);
    o->vtbl = D_8015E550;
    return o;
}
