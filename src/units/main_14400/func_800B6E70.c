#include "common.h"

extern unsigned char D_801C91D8[];
/* The arena allocator receives a size, but always returns this shared backing object. */
void *func_800B6E70(s32 size) { return D_801C91D8; }
