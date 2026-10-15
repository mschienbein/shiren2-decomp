#include "func_801F1974_143894/types.h"

s32 func_801F1974_143894(u8 id, u8 low, u8 high) {
    s32 lo = low;
    s32 hi = high;
    s32 level;

    if (id == 0xFF) {
        return 1;
    }
    if (id & 0x80) {
        if (hi == func_801E9F70_13BE90(((id << 8) & 0x7F00) | lo)) {
            return 1;
        }
    } else {
        level = D_801F5228_147148[id];
        if (level >= lo && level <= hi) {
            return 1;
        }
    }
    return 0;
}
