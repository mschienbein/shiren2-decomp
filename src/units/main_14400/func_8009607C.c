#include "common.h"

/* Widget slot 7 (+0x38 this-adjust, +0x3C method), bound in every widget table:
 * "may this level be left with this result?". The default always agrees; self
 * and result are passed by the slot contract (func_800957C0 loads both) and unused. */
s32 func_8009607C(void *self, s32 *result) {
    return 1;
}
