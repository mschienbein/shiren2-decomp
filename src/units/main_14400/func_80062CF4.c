#include "common.h"
s32 func_80062CF4(u32 value) { s32 count = 0; while (value) { value >>= 1; count++; } return count - 1; }
