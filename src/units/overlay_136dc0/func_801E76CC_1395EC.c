#include "func_801E76CC_1395EC/types.h"

extern Rec801F6764 D_801F6764_148684[30];
extern Unit *func_8007946C(s32 side, s32 slot);
extern f32 func_80027C60(f32 x);

void func_801E76CC_1395EC(void) {
    Rec801F6764 *rec = &D_801F6764_148684[30];
    Unit *unit;
    s32 phase;
    s32 i;
    s32 dx, dz;
    f32 s;

    for (i = 29; i != -1; i--) {
        rec--;
        if (rec->xA != 0) {
            unit = func_8007946C(0, i);
            rec->xC++;
            phase = rec->xC % rec->xA;
            if (rec->xA == 1) {
                phase = (rec->xC & 1) - 1;
                dx = rec->x8 * phase;
                dz = rec->x9 * phase;
                unit->xC = rec->xE + dx;
                unit->x10 = rec->x10 + dz;
            } else {
                s = func_80027C60(phase * 2.0f * 3.141592654 / (f32)rec->xA);
                dx = rec->x8 * s;
                dz = rec->x9 * s;
                unit->xC = rec->xE + dx;
                unit->x10 = rec->x10 + dz;
            }
        }
    }
}
