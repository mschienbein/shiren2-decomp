#include "common.h"

typedef struct {
    unsigned char pad0[8];
    void *vtable;
} Obj;

extern unsigned char D_8015E308[];
Obj *func_80116D50(Obj *obj, s32 kind);

Obj *func_80118EB0(Obj *obj)
{
    func_80116D50(obj, 20);
    obj->vtable = D_8015E308;
    return obj;
}
