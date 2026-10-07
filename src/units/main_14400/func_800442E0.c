#include "common.h"
extern s32 D_80138B10;
extern unsigned char D_80160A70[];
/* The size argument is not read: the static buffer is always returned. */
void *func_800442E0(s32 unusedSize) { D_80138B10=1; return D_80160A70; }
