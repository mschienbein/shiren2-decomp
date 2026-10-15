#include "func_801E7610_139530/types.h"

extern Rec801F6764 D_801F6764_148684[30];
extern Unit *func_8007946C(s32 side, s32 slot);

void func_801E7610_139530(s32 idx, s32 amp_x, s32 amp_z, s32 period) {
    Rec801F6764 *rec = &D_801F6764_148684[idx];
    Unit *unit = func_8007946C(0, idx);

    if (unit != 0) {
        if (period != 0) {
            rec->x8 = amp_x;
            rec->x9 = amp_z;
            rec->xC = 0;
            rec->xE = unit->xC;
            rec->x10 = unit->x10;
        } else if (rec->xA != 0) {
            unit->xC = rec->xE;
            unit->x10 = rec->x10;
        }
        rec->xA = period;
    }
}
