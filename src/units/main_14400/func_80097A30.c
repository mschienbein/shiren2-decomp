#include "common.h"

/* Widget vtable slot 11 (+0x58 this-adjust, +0x5C method): writes the text of a flattened
 * item index into buf. This class has no item text; self and index are passed by the
 * slot contract and unused. */
void func_80097A30(void *self, s32 index, char *buf) {
    *buf = 0;
}
