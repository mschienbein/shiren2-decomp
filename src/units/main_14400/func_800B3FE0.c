#include "common.h"
typedef unsigned char u8;
extern u8 D_8014344C;
s32 func_800B3DF0(u8 a, u8 b);
static inline u8 count(void) { return D_8014344C; }
s32 func_800B3FE0(u8 *out_a, u8 *out_b) { u8 i, j; for (i = 0; i <= count() - 2; ++i) { for (j = i + 1; j <= D_8014344C - 1; ++j) { if (func_800B3DF0(i, j)) { *out_a = i; *out_b = j; return 1; } } } return 0; }
