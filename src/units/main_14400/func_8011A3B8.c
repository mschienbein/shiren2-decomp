#include "common.h"

/* The query slot supplies self; this override only inspects arg1. */
s32 func_8011A3B8(void *self, s32 arg1) {
    s32 result = 0;
    if (arg1 == 6) {
        result = 1;
    } else if (arg1 == 11) {
        result = 1;
    }
    return result;
}
