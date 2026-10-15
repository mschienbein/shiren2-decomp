#include "func_801E54B8_1373D8/types.h"

void func_8006E6D8(void);

s32 func_801E54B8_1373D8(s32 count, s32 mode) {
    while (--count != -1) {
        if (D_801F5130_147050 == 0 && mode == 1) {
            return 1;
        }
        if (D_801F5130_147050 != -1 && mode == 2) {
            return 1;
        }
        D_801F5130_147050 = -1;
        func_8006E6D8();
    }
    return 0;
}
