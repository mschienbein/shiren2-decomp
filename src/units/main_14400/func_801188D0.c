#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad0[8]; void *field_8; } Obj;
extern u8 D_8015E0A8[];
Obj *func_80116D50(Obj *o, s32 kind);
Obj *func_801188D0(Obj *o) {
    func_80116D50(o, 0x10);
    o->field_8 = D_8015E0A8;
    return o;
}
