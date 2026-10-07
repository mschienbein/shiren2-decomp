#include "common.h"

typedef struct { char pad0[0x8]; void *field_8; } Obj;

extern char D_80160108[];
Obj *func_80115690(Obj *obj, s32 kind);

Obj *func_80125050(Obj *obj) {
    func_80115690(obj, 0xDC);
    obj->field_8 = D_80160108;
    return obj;
}
