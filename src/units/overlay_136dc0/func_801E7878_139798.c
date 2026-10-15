#include "func_801E7878_139798/types.h"

extern Rec801F6764 D_801F6764_148684[30];
/* Vertical offset per step, indexed by step - 1 (lb at 0x801E78D4); 80 ends the sequence.
 * No other code in either overlay or the main image reads or writes it. */
s8 D_801F5160_147080[14] = { -1, 5, 4, 3, 1, 1, 0, 0, 0, -1, -4, -4, -4, 80 };
extern Unit *func_8007946C(s32 side, s32 slot);

void func_801E7878_139798(void) {
    Rec801F6764 *rec = &D_801F6764_148684[30];
    Unit *unit;
    s32 i;

    for (i = 29; i != -1; i--) {
        rec--;
        if (rec->x17 != 0) {
            unit = func_8007946C(0, i);
            if (D_801F5160_147080[rec->x17 - 1] == 80) {
                rec->x17 = 0;
                unit->x7 = 1;
            } else {
                unit->xE += D_801F5160_147080[rec->x17 - 1] * 8;
                rec->x17++;
            }
        }
    }
}
