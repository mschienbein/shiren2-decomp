#include "common.h"

/* RNG object (D_80154088 class): 0x10-byte header (state/saved pointers, depth, vtable),
 * then the three-word Tausworthe state. */
typedef struct {
    unsigned char header_00[0x10];
    u32 state_10;
    u32 state_14;
    u32 state_18;
} RandomState;

/* Slot +0x0C target (0x80154094), sharing the signed-word contract of func_800C5CA4 and the
 * s32 (*)(void *) slot declarations of its wrappers; arithmetic stays unsigned. */
s32 func_800C5E10(RandomState *random) {
    u32 mixed;
    mixed = ((random->state_10 << 13) ^ random->state_10) >> 19;
    random->state_10 = ((random->state_10 & ~1U) << 12) ^ mixed;
    mixed = ((random->state_14 << 2) ^ random->state_14) >> 25;
    random->state_14 = ((random->state_14 & ~7U) << 4) ^ mixed;
    mixed = ((random->state_18 << 3) ^ random->state_18) >> 11;
    random->state_18 = ((random->state_18 & ~15U) << 17) ^ mixed;
    return (s32)(random->state_10 ^ random->state_14 ^ random->state_18);
}
