#include "common.h"

extern s32 func_800AC670(void *key);

/* Forwards the incoming key (a0, untouched before the call at 0x800AF490) and negates the result. */
s32 func_800AF488(void *key) {
    return func_800AC670(key) ^ 1;
}
