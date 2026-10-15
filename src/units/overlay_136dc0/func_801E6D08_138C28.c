#include "func_801E6D08_138C28/types.h"

void func_801E6D08_138C28(s32 model, s32 animation, s32 code, s32 mode, Vec3 *position, s32 owner)
{
    s32 slot;
    if (!model) {
        func_801E6BA0_138AC0(owner);
    } else {
        position->y = func_80062554((s32)position->x, (s32)position->z);
        position->x = position->x * 32.0f + 16.0f;
        position->z = position->z * 32.0f + 16.0f;
        func_8007946C(0, owner);
        if (animation < 100)
            slot = func_801E6C44_138B64(model, func_80059830(animation), code, mode, position);
        else
            slot = func_801E6C44_138B64(model, animation - 100, code, mode, position);
        if (slot != -1) D_801F6EAA_148DCA[slot].b01 = owner;
    }
}
