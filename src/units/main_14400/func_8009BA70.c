#include "common.h"

void func_80045A24(s32 arg);
s32 func_8009C0E4(void *p);
s32 func_8009BA70(void *p) { s32 r; func_80045A24(1); r = func_8009C0E4(p); if (r == 0) return 1; return 2; }
