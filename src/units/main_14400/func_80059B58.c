#include "common.h"

/* The producer constructs these addresses from BSS blocks and a 64-byte stride.
 * Original pointee structure and API names remain unresolved. */
typedef struct Block_80059B58 Block_80059B58;

extern Block_80059B58 *D_801658A8;
extern Block_80059B58 *D_801658AC;
extern Block_80059B58 *D_801658B0;

void func_80059B58(Block_80059B58 **out0, Block_80059B58 **out1,
                   Block_80059B58 **out2) {
    if (out0 != 0) {
        *out0 = D_801658A8;
    }
    if (out1 != 0) {
        *out1 = D_801658AC;
    }
    if (out2 != 0) {
        *out2 = D_801658B0;
    }
}
