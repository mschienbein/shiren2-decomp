#include "common.h"
extern unsigned char func_800C57CC(void *rng, s32 limit);
/* Average of two byte samples in [lo, hi]; both samples come from the same RNG object. */
s32 func_800C5A48(void *rng, s32 arg1, s32 arg2) {
    s32 range = (arg2 - arg1) & 0xFF;
    unsigned char first = func_800C57CC(rng, range);
    return (arg1 & 0xFF) + (((first & 0xFF) + (func_800C57CC(rng, range) & 0xFF)) >> 1);
}
