#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad0[8]; void *field_8; } Obj;
extern u8 D_8015EF40[];
/* Returns the input object through this TU's partial view. */
Obj *func_80111530(Obj *o, s32 kind);
Obj *func_8011E250(Obj *o) {
    func_80111530(o, 0x91);
    o->field_8 = D_8015EF40;
    return o;
}
