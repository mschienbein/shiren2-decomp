#include "func_801E7C88_139BA8/types.h"

extern Unit *func_8007946C(s32 side, s32 slot);
extern void func_801E6BA0_138AC0(s32 idx);
extern s32 func_801E6C44_138B64(s32 a0, s32 a1, s32 a2, s32 a3, f32 *pos);
extern Rec801F6EAA D_801F6EAA_148DCA[];

void func_801E7C88_139BA8(s32 a0, s32 a1, s32 idx, s32 a3, s32 arg4, f32 *pos) {
    Unit *unit;
    Rec801F6EAA *e;
    f32 v[3];

    if (a0 == 0) {
        func_801E6BA0_138AC0(idx);
        return;
    }
    unit = func_8007946C(0, idx);
    v[0] = *pos + (unit->xC >> 2);
    v[1] = *pos + (unit->xE >> 2);
    v[2] = *pos + (unit->x10 >> 2);
    e = &D_801F6EAA_148DCA[func_801E6C44_138B64(a0, a1, a3, arg4, v)];
    e->x1 = idx;
    e->x4 = 1;
    e->x6 = pos[0];
    e->x8 = pos[1];
    e->xA = pos[2];
}
