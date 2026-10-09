#include "common.h"

/* Trap +0x44 supplies self; this constant predicate does not read it. */
s32 func_8011F67C(void *self) {
    return 1;
}
