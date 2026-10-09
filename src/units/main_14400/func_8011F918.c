#include "common.h"

/* Trap +0x44 supplies self; this constant predicate does not read it. */
s32 func_8011F918(void *self) { return 1; }
