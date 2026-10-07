#include "common.h"

/* Widget vtable slot 9 (+0x48 this-adjust, +0x4C method) takes only the receiver and
 * returns a status code (func_80046CB0 tests its low byte for 0 and 2). The default
 * returns 1; self is passed by the slot contract and unused. */
s32 func_80096074(void *self) {
    return 1;
}
