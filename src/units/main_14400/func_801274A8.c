#include "common.h"

/* The query slot supplies self; this override only inspects arg1. */
s32 func_801274A8(void *self, s32 arg1) {
    s32 result = 0;
    if (arg1 == 29) {
        result = 1;
    } else if (arg1 == 25) {
        result = 1;
    }
    return result;
}
