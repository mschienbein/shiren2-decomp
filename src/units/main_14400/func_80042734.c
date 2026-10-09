#include "common.h"
typedef struct { unsigned char pad00[9]; unsigned char field09; } Cell;
extern void *func_800A8CB0(s32 cell);
s32 func_80042734(s32 cell) { return ((Cell *)func_800A8CB0(cell & 0xFF))->field09 & 0xF; }
