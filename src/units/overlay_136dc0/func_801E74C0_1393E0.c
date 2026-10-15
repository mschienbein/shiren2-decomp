#include "func_801E74C0_1393E0/types.h"

extern Rec801F6764 D_801F6764_148684[30];
/* Per-step tables read only by this function (lb at 0x801E7540/0x801E756C/0x801E751C,0x801E7578);
 * no other code in either overlay or the main image reads or writes them.
 * Direction offsets for the eight unit directions (x4A): dx, then dy. */
s8 D_801F5144_147064[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
s8 D_801F514C_14706C[8] = { 0, -1, -1, -1, 0, 1, 1, 1 };
/* Distance per step, indexed by step - 1; 80 ends the sequence. */
s8 D_801F5154_147074[11] = { -2, -1, 0, 4, 4, 4, 4, -5, -5, -5, 80 };
extern Unit *func_8007946C(s32 side, s32 slot);

void func_801E74C0_1393E0(void) {
    Rec801F6764 *rec = &D_801F6764_148684[30];
    Unit *unit;
    s32 i;

    for (i = 29; i != -1; i--) {
        rec--;
        if (rec->x16 != 0) {
            unit = func_8007946C(0, i);
            if (D_801F5154_147074[rec->x16 - 1] == 80) {
                rec->x16 = 0;
                unit->x7 = 1;
            } else {
                unit->xC += D_801F5144_147064[unit->x4A] * D_801F5154_147074[rec->x16 - 1] * 8;
                unit->x10 += D_801F514C_14706C[unit->x4A] * D_801F5154_147074[rec->x16 - 1] * 8;
                rec->x16++;
            }
        }
    }
}
