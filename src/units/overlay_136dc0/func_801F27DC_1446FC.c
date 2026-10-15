#include "func_801F27DC_1446FC/types.h"

s32 func_801F27DC_1446FC(Arg801F27DC *arg) {
    if (D_80142F24.index == 6) {
        if (arg->kind != 0x8A) {
            return 0;
        }
        /* ODD_C: all-ones test spelled as ~x: lb+nor+bnez at 0x801F2804..0x801F280C;
         * "arg->field_05 != -1" emits addiu -1 + bne (3 words differ). */
        if (~arg->field_05) {
            return 0;
        }
        if (D_8014767C & 0xC) {
            return 0;
        }
        func_801F2254_144174(0x1FA, 0);
        return 1;
    }
    return 0;
}
