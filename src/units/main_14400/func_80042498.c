#include "common.h"

typedef unsigned char u8;

extern u8 D_80147600[];

u8 func_800C57CC(void *rng, s32 limit);

u8 func_80042498(s32 kind) {
    return func_800C57CC(D_80147600, (u8)kind);
}
