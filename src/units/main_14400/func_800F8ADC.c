#include "common.h"

typedef struct Obj_800EE358 Obj_800EE358;

extern void func_800E016C(Obj_800EE358 *obj, s32 flags);
extern void func_800A3918(Obj_800EE358 *obj);

/* Destructor: run the base destructor, then free when the delete bit is set. */
void func_800F8ADC(Obj_800EE358 *self, s32 flags)
{
    func_800E016C(self, 0);
    if (flags & 1) {
        func_800A3918(self);
    }
}
