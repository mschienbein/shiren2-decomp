#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

/* Vtable slot 3 of D_801606D0: s32 (*)(void *self, s32 kind), like its peers.
 * self is supplied by the virtual call contract and unused here. */
s32 func_80128244(void *self, s32 kind) {
    return kind == 0x1F;
}
