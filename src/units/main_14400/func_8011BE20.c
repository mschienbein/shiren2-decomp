#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad0[8]; void *field_8; } Obj;
extern u8 D_8015E930[];
/* Returns the input object through this TU's partial view. */
Obj *func_80112D20(Obj *o, s32 kind);
Obj *func_8011BE20(Obj *o) {
    func_80112D20(o, 0x26);
    o->field_8 = D_8015E930;
    return o;
}
