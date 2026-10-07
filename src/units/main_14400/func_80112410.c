#include "common.h"

/* Trap predicate slot +0x1C supplies self; this default ignores it. */
s32 func_80112410(void *arg0, s32 kind)
{
    s32 result = 0;
    if (kind == 7) {
        result = 1;
    } else if (kind == 11) {
        result = 1;
    }
    return result;
}
