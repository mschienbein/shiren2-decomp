#include "common.h"

/* Widget vtable slot 12 (+0x60 this-adjust, +0x64 method): text attribute of a flattened
 * item index, passed to func_80048870. This class uses one attribute for every item;
 * self and index are passed by the slot contract and unused. */
s32 func_8009DCD8(void *self, s32 index) {
    return 0x78000000;
}
