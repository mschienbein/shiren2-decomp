#include "common.h"
/* Virtual predicate: a0 is the supplied, unused receiver; a1 is the command identifier. */
s32 func_80129158(void *self, s32 command) { return (u32)(command - 0x23) < 2; }
