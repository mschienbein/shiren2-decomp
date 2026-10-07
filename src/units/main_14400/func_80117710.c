#include "common.h"

typedef struct { char pad0[8]; void *vtable; } Obj;
extern char D_8015DC30[];
Obj *func_80116D50(Obj *obj, s32 kind);

Obj *func_80117710(Obj *obj) {
    func_80116D50(obj, 0x3);
    obj->vtable = D_8015DC30;
    return obj;
}
