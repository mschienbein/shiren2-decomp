#include "common.h"

typedef struct { char pad0[8]; void *vtable; } Obj;
extern char D_80160010[];
Obj *func_80115690(Obj *obj, s32 kind);

Obj *func_80124DF0(Obj *obj) {
    func_80115690(obj, 0xDA);
    obj->vtable = D_80160010;
    return obj;
}
