#include "common.h"
#include "item_records.h"
typedef signed char s8;
typedef unsigned char u8;
extern void *func_800AADF8(s8 index, u8 value);
extern void func_800AE974(void *, signed char);
extern s32 func_800ADA00(Item *, void *);
s32 func_800D36E0(void *a, s32 b, s32 c) { s32 result; Item *p = func_800AADF8(-1, c); if (p) { func_800AE974(p, b); func_800ADA00(p, a); result = 1; } else result = 0; return result; }
