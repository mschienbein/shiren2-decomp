#include "common.h"

typedef unsigned short u16;

typedef struct Rng Rng;

u16 func_800C58B0(Rng *rng);

/* Uniform value in [0, range]: mask the generator output to the bit width of range and retry. */
u16 func_800C58DC(Rng *rng, u16 range)
{
    s32 mask = 0;
    u16 bits = range;
    u16 value;

    while (bits != 0) {
        mask = (mask << 1) | 1;
        bits >>= 1;
    }
    do {
        bits = mask;
        bits &= func_800C58B0(rng);
        value = bits;
    } while (value > range);
    return value;
}
