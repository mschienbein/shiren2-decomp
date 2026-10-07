#include "common.h"

typedef struct {
    unsigned char pad0[8];
    void *vtable;
} Obj;

extern unsigned char D_8015F708[];
Obj *func_80114060(Obj *obj, s32 kind);

Obj *func_80120430(Obj *obj)
{
    func_80114060(obj, 166);
    obj->vtable = D_8015F708;
    return obj;
}
