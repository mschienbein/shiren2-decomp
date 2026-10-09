#include "common.h"
/* The virtual-call contract supplies self even though this override only tests kind. */
s32 func_8010E238(void *self, s32 kind) {
    return kind == 11;
}
