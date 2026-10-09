#include "common.h"

typedef unsigned char u8;

typedef struct Obj800F40F0 Obj800F40F0;

typedef struct {
    u8 pad0[0xA];
    u8 kind;
    u8 padB[0x13];
    u8 flags_1E;
} Target800F40F0;

static inline s32 is_special_kind(u8 kind)
{
    return kind == 4 || kind == 0x5B;
}

/* The virtual dispatch ABI supplies obj; this query only examines target. */
s32 func_800F40F0(Obj800F40F0 *obj, Target800F40F0 *target, u8 *out)
{
    *out = 0;
    if (target == 0) {
        return 0;
    }
    if ((target->flags_1E >> 2) & 1) {
        return 2;
    }
    if (is_special_kind(target->kind)) {
        *out = 5;
        return 2;
    }
    return 0;
}
