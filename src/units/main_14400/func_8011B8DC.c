#include "common.h"

/* The query slot supplies self; this override only inspects kind. */
s32 func_8011B8DC(void *self, s32 kind) {
    s32 result = 0;

    if (kind == 6) {
        result = 1;
    } else if (kind == 11) {
        result = 1;
    }
    return result;
}
