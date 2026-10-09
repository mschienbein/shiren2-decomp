#include "common.h"

/* The original virtual kind-query contract supplies the unused object receiver. */
s32 func_801171DC(void *object, s32 kind) {
    return kind == 11;
}
