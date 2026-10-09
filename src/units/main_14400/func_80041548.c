#include "common.h"
typedef struct { s32 x; s32 y; } Pos;
u32 func_800B1C6C(Pos *pos);
s32 func_80041548(s32 y, s32 x) { Pos pos; s32 result = 0; pos.y = y; pos.x = x; if (func_800B1C6C(&pos) & 0x400) result = 1; return result; }
