#include "common.h"

typedef struct VTable VTable;
typedef struct { unsigned char pad00[0x24]; VTable *vtable24; } Obj;
extern VTable D_80149778;
extern void func_800E016C(Obj *obj, s32 flags);
extern void func_800A3918(Obj *obj);

void func_80136548(Obj *obj, s32 flags)
{
    obj->vtable24 = &D_80149778;
    func_800E016C(obj, 0);
    if (flags & 1) {
        func_800A3918(obj);
    }
}
