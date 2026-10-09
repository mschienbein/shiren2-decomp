#include "common.h"

/* RNG state-size getter: slot +0x2C of D_80154088 (s32 (void *self), called by
 * func_800C5B8C/func_800C5BE8 with the adjusted receiver). The receiver is part of the
 * slot contract and is unused here. */
s32 func_800C5DBC(void *self) {
    return 12;
}
