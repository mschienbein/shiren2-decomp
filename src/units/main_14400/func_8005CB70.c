#include "common.h"

/* Catalogue-adjacent setter/getter sharing one initialized word; the single
 * definition lives here so both delay-slot accesses see a defined object. */
s32 D_8013B5F0 = 0;

void func_8005CB70(s32 value) {
    D_8013B5F0 = value;
}

s32 func_8005CB7C(void) {
    return D_8013B5F0;
}
