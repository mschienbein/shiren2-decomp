#include "common.h"
s32 func_800D19C0(unsigned char *p, s32 row) { s32 i; for (i = 0; i < 4; i++) { unsigned char *q = p + (i + row * 4); if (q[0] == 0 && q[0x43] == 0) break; } return i; }
