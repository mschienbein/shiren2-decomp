#include "common.h"

/* Trap +0x44 supplies self; this constant predicate does not read it. */
s32 func_8011E20C(void *self) {
    return 1;
}
