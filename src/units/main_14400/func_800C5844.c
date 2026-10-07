#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

u8 func_800C57CC(void *rng, u8 limit);

s32 func_800C5844(void *rng, u8 base, u8 top) {
    return base + func_800C57CC(rng, top - base);
}
