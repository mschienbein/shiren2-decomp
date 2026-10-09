#include "common.h"
extern u32 func_800C598C(void *rng);
/* Uniform random word in [0, range] by masked rejection sampling. */
u32 func_800C59B4(void *rng, u32 range) {
    u32 mask = 0;
    u32 bits = range;
    while (bits) {
        mask = (mask << 1) | 1;
        bits >>= 1;
    }
    do { bits = func_800C598C(rng) & mask; } while (bits > range);
    return bits;
}
