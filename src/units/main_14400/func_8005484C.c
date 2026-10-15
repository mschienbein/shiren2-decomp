#include "common.h"
typedef unsigned char u8;

/* Flag byte (.data, 0x801398B2); this file owns it: set by func_8005484C, read by func_8005485C. */
u8 D_801398B2 = 0;

/* Reads no argument (0x8005484C..0x80054858); no original caller supplies one. */
void func_8005484C(void) { D_801398B2 = 1; }

s32 func_8005485C(void) { return D_801398B2; }
