#include "common.h"
extern s32 func_800A39F0(s32);
s32 func_800A3A00(s32 value) { return func_800A39F0(value & 0xFF) ^ 1; }
