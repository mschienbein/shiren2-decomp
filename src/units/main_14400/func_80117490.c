#include "common.h"

typedef struct {
    unsigned char pad0[8];
    void *vtable;
} Obj;

extern unsigned char D_8015DBD8[];
Obj *func_80116D50(Obj *obj, s32 kind);

Obj *func_80117490(Obj *obj)
{
    func_80116D50(obj, 2);
    obj->vtable = D_8015DBD8;
    return obj;
}
