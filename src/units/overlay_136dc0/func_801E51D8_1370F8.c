#include "func_801E51D8_1370F8/types.h"

/* One scalar owner for the getter/resetter pair; both ROM accesses place
 * the load/store in their return delay slot. Ownership remains proposed. */
s32 D_801F513C_14705C = -1;

s32 func_801E51D8_1370F8(void) {
    return D_801F513C_14705C;
}

void func_801E51E4_137104(void) {
    D_801F513C_14705C = -1;
}
