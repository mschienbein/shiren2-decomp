#include "func_801F212C_14404C/types.h"

void func_801F212C_14404C(u16 id, u8 mode) {
    if (D_801F5D0E_147C2E == 0) {
        D_801F5D0E_147C2E = id;
        D_801F5D0C_147C2C = 0;
        if (mode) {
            D_801F5D0C_147C2C = 2;
        }
        D_801F5D08_147C28 = 0;
    }
}
