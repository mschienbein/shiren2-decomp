#include "func_801E99C4_13B8E4/types.h"

/* A u8 slot selects three bits: all intermediate indices fit in 16 bits. */
extern "C" u8 func_801E99C4_13B8E4(u8 slot, s32 alternate) {
    s16 bit = slot * 3 + 200;
    if (alternate) {
        bit = slot * 3 + 176;
    }
    u8 value = func_801E9F70_13BE90(bit++);
    value += func_801E9F70_13BE90(bit++) << 1;
    value += func_801E9F70_13BE90(bit) << 2;
    return value;
}
