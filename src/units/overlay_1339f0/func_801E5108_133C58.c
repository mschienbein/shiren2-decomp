#include "func_801E5108_133C58/types.h"

u8 *func_801E5228_133D78(void);
u32 func_801E5940_134490(void);
s32 func_801E50E4_133C34(u8 *found);
void func_801E51CC_133D1C(void);

s32 func_801E5108_133C58(void) {
    s32 count = -1;
    s32 i;
    u8 found[16];

    for (i = 0; i < 3; i++) {
        D_801E7830_136380[i] = 0xFF;
    }
    if (func_801E5228_133D78() != 0) {
        if (func_801E5940_134490() == 0) {
            count = 0;
            func_801E50E4_133C34(found);
            for (i = 0; i < 16; i++) {
                if (found[i] >= 1 && found[i] <= 3) {
                    D_801E7830_136380[found[i] - 1] = i;
                    count++;
                }
            }
        } else {
            func_801E51CC_133D1C();
        }
    }
    return count;
}
