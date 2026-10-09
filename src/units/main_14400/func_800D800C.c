#include "common.h"
extern unsigned char D_80148190[0xA2];
s32 func_800D800C(void) { s32 count = 0; s32 i; for (i = 0xA1; i != -1; --i) { if (D_80148190[i]) ++count; } return count; }
