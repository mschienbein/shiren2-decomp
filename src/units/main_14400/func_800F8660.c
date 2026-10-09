#include "common.h"
typedef struct Obj_800EE358 Obj_800EE358;
typedef struct Obj800A38A0 Obj800A38A0;
extern void func_800E016C(Obj_800EE358 *obj, s32 flags);
extern void func_800A3918(Obj800A38A0 *obj);
void func_800F8660(Obj_800EE358 *self, s32 flags) {
    func_800E016C(self, 0);
    if (flags & 1) func_800A3918((Obj800A38A0 *)self);
}
