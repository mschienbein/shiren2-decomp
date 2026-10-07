#include "common.h"

u32 func_800C59B4(void *rng, u32 range);

s32 func_800C5A1C(void *rng, s32 base, s32 top) {
    return (s32)((u32)base + func_800C59B4(rng, (u32)top - (u32)base));
}
