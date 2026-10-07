#include "common.h"

/* The query slot supplies self; this override only inspects kind. */
s32 func_8010DEFC(void *self, s32 kind) {
    s32 result = 0;

    if (kind == 0x20) {
        result = 1;
    } else if (kind == 0xB) {
        result = 1;
    }
    return result;
}
