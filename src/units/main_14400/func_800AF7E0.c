#include "common.h"
/* Seven pool-header pointers, defined (const) by func_800AF818.c. */
extern void *const D_80153AE4[];
s32 func_800AF7E0(void *value) { s32 i; for(i=0;i<7;i++) { if(D_80153AE4[i]==value) return i; } return -1; }
