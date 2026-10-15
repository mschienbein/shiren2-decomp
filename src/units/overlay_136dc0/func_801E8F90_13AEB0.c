#include "func_801E8F90_13AEB0/types.h"
extern u8 D_80147620[];
extern u8 func_800C57CC(void *rng, s32 range);
u32 func_801E8F90_13AEB0(u8 *slots, u8 value) {
    u8 tries;
    u8 index;
    for (tries = 0; tries < 20; tries++) {
        index = func_800C57CC(D_80147620, 3);
        if (slots[index] == value) {
            slots[index] = 1;
            return 1 << index;
        }
    }
    /* ODD_C: the ROM repeats the same last-index test four times; neither
     * index nor slots advances. Preserve that original ineffective fallback,
     * rather than repairing it into a scan. A single-test alternative gives
     * 144 bytes / 30 differing words (original: 212 bytes). */
    for (tries = 0; tries < 4; tries++) {
        if (slots[index] == value) {
            slots[index] = 1;
            return 1 << index;
        }
    }
    return 1;
}
