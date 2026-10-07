#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad0[8]; void *field_8; } Obj;
extern u8 D_8015FD88[];
Obj *func_80115690(Obj *o, s32 kind);
Obj *func_80123E90(Obj *o) {
    func_80115690(o, 0xD2);
    o->field_8 = D_8015FD88;
    return o;
}
