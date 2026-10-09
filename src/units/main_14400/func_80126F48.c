#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

/* Vtable slot 3 of D_80160568: s32 (*)(void *self, s32 kind), like its peers.
 * self is supplied by the virtual call contract and unused here. */
s32 func_80126F48(void *self, s32 kind) {
    s32 result = 0;

    if (kind == 4 || kind == 0x1D || kind == 0xB) {
        result = 1;
    }
    return result;
}
