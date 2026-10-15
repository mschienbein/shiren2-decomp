#include "func_801E51F4_137114/types.h"

extern u8 D_801F5110_147030[24];

s32 func_801E51F4_137114(u16 code) {
    u32 i;

    for (i = 0; i < 24; i += 2) {
        if ((D_801F5110_147030[i + 1] | (D_801F5110_147030[i] << 8)) == code) {
            return 1;
        }
    }
    return 0;
}
