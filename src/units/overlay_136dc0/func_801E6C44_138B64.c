#include "func_801E6C44_138B64/types.h"

s32 func_801E6C44_138B64(s32 model, s32 animation, s32 code, s32 mode, Vec3 *position)
{
    s32 i;
    if (model == 0) {
        func_801E6BA0_138AC0(255);
        return -1;
    }
    for (i = 0; i < 8; i++) {
        OvlEffect *p = &D_801F6EAA_148DCA[i];
        if (!p->b02) {
            s32 handle = func_8008BF14(model, animation, code, 1, 0, (float *)position, 1.0f);
            p->b02 = handle;
            func_8008C334(handle);
            p->b03 = 1;
            p->b01 = 255;
            p->b00 = mode;
            p->b04 = 0;
            p->b02++;
            return i;
        }
    }
    return -1;
}
