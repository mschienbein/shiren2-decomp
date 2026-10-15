#include "func_801E72BC_1391DC/types.h"

extern Rec801F6764 D_801F6764_148684[30];
extern Unit *func_8007946C(s32 side, s32 slot);
extern void func_8007935C(s32 i);
extern s32 func_8007920C(s32 kind, s32 id, s32 x, s32 y);

void func_801E72BC_1391DC(s32 idx, s32 id, s32 arg2, s32 x, s32 z, s32 flip, s32 scale_milli) {
    Rec801F6764 *rec = &D_801F6764_148684[idx];
    Unit *unit;
    f32 scale;

    if (rec->x14 != 0) {
        func_8007935C(rec->x14 - 1);
        rec->x14 = 0;
    }
    if (id != 0) {
        rec->x14 = func_8007920C(0, id, 0, 0) + 1;
        unit = func_8007946C(4, rec->x14 - 1);
        scale = scale_milli;
        scale /= 1000.0f;
        unit->x3E = arg2;
        unit->xC = x * scale;
        unit->xE = z * scale;
        if (flip) {
            unit->x34 = -1;
            unit->x10 = -1;
        } else {
            unit->x34 = 1;
            unit->x10 = 1;
        }
        unit->x37 = 1;
        unit->x12 = 0;
        unit->x13 = idx;
        unit->x1C = scale;
        unit->x20 = scale;
        unit->x35 = 0;
        unit->x38 = 0;
    }
}
