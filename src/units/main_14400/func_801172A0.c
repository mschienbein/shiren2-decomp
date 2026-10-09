#include "common.h"

/* Virtual kind query: the original contract supplies the unused self receiver. */
s32 func_801172A0(void *self, s32 kind) {
    return kind == 0x24;
}
