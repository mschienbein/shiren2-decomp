#include "common.h"

/* Exchange setter and getter of one initialized word (initial value 1); the
 * single definition lives here. */
s32 D_8013B744 = 1;

s32 func_80061210(s32 value) {
    s32 previous = D_8013B744;
    D_8013B744 = value;
    return previous;
}

s32 func_80061224(void) {
    return D_8013B744;
}
