#include "func_801F21E4_144104/types.h"

/* Same test as main func_80041EB0, visible inline in this TU. */
static inline s32 flag4_is_one(void) {
    return D_80140160[4] == 1;
}

void func_801F21E4_144104(void) {
    if (D_801F5D10_147C30 != 0) {
        if (D_801F5D0E_147C2E == 0) {
            s32 skip = D_80140160[6] == 1 && !flag4_is_one();

            if (!skip) {
                func_801F212C_14404C(D_801F5D10_147C30, 0);
            }
        }
        D_801F5D10_147C30 = 0;
    }
}
