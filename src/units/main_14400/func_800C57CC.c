#include "common.h"
typedef unsigned char u8;
extern u8 func_800C57A0(void *);
/* Uniform random byte in [0, range]: mask the generator's byte to the range's bit width and
 * reject samples above the range. */
u8 func_800C57CC(void *rng, s32 range) {
    u8 mask = 0;
    u8 bits = range;
    u8 limit;
    while (bits) {
        mask = (mask << 1) | 1;
        bits >>= 1;
    }
    limit = range;
    do {
        bits = mask & func_800C57A0(rng);
        range = bits;
    } while ((u32)range > limit);
    return range;
}
