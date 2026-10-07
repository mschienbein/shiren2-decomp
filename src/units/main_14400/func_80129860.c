#include "common.h"

typedef unsigned char u8;

/* D_801487D0 passes state, intentionally unused here; skip the two-byte operand. */
u8 *func_80129860(void *state, u8 *cursor) { return cursor + 2; }
