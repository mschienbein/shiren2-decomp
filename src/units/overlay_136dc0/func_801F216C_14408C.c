#include "func_801F216C_14408C/types.h"

/* Same test as main func_80041EB0, visible inline in this TU. */
static inline s32 flag4_is_one(void) {
    return D_80140160[4] == 1;
}

void func_801F216C_14408C(u16 id, void *object) {
    if (D_801F5D0E_147C2E == 0) {
        if (!flag4_is_one() || !func_801F23AC_1442CC(id)) {
            D_801F5D0E_147C2E = id;
            D_801F5D0C_147C2C = 2;
            D_801F5D08_147C28 = object;
        }
    }
}
