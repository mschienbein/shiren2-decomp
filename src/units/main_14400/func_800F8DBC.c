#include "common.h"

typedef struct Obj Obj;
extern void func_800E016C(Obj *obj, s32 flags);
extern void func_800A3918(Obj *obj);

void func_800F8DBC(Obj *obj, s32 flags)
{
    func_800E016C(obj, 0);
    if (flags & 1) {
        func_800A3918(obj);
    }
}
