#include "common.h"

typedef unsigned char u8;

extern u8 D_8015374C[];
u8 func_800AC1AC(u8 v) { s32 lo = 1, hi = 0x15, mid; while (1) { if (hi - lo < 2) break; mid = (hi + lo) / 2; if (v < D_8015374C[mid]) hi = mid; else lo = mid; } return lo; }
