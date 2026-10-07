#include "common.h"
typedef struct { char pad[0x45]; signed char x45; } S;
s32 func_800957B4(S *p) { return p->x45 != 0; }
