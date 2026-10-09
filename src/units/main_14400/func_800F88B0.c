#include "common.h"
typedef struct Object Object;
extern void func_800E016C(Object *obj, s32 flags);
extern void func_800A3918(Object *obj);
void func_800F88B0(Object *object, s32 flags)
{
    func_800E016C(object, 0);
    if (flags & 1) {
        func_800A3918(object);
    }
}
