#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad0[8]; void *field_8; } Obj;
extern u8 D_8015F5C8[];
Obj *func_8010E290(Obj *o, s32 kind);
Obj *func_8011FEF0(Obj *o) {
    func_8010E290(o, 0xA2);
    o->field_8 = D_8015F5C8;
    return o;
}
