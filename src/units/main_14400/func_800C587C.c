#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

u8 func_800C57CC(void *rng, s32 limit);

s32 func_800C587C(void *rng, u8 limit) {
    return limit > func_800C57CC(rng, 0x63);
}
