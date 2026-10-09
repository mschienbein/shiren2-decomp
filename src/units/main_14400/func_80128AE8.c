#include "common.h"

/* Vtable slot 3 of D_80160720: s32 (*)(void *self, s32 kind), like its peers.
 * self is supplied by the virtual call contract and unused here. */
s32 func_80128AE8(void *self, s32 kind) {
    return kind == 0x23;
}
