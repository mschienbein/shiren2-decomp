#include "common.h"
typedef signed char s8;
typedef struct { char pad; unsigned char x1; char pad2[3]; s8 x5; } S;
void func_800AE974(S *p, s8 v) { if (p->x1 != 0xCC) p->x5 = v; }
