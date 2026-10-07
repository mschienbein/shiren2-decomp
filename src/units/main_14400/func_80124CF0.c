#include "common.h"

typedef struct {
    unsigned char pad0[8];
    void *vtable;
} Obj;

extern unsigned char D_8015FFC0[];
Obj *func_80115690(Obj *obj, s32 kind);

Obj *func_80124CF0(Obj *obj)
{
    func_80115690(obj, 217);
    obj->vtable = D_8015FFC0;
    return obj;
}
