#include "common.h"

typedef struct { char pad0[0x4C]; void *vtable_4C; } Obj;

extern Obj *func_800953C0(Obj *obj);
extern char D_801521D0[];

Obj *func_8009767C(Obj *obj) {
    func_800953C0(obj);
    obj->vtable_4C = D_801521D0;
    return obj;
}
