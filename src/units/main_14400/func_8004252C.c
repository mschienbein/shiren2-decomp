#include "common.h"
typedef unsigned char u8;
extern s32 func_800610A8(void);
extern u8 func_801E976C(u32 index);
/* Interface correction for the canonical unit: the only caller (func_800659B8) passes the
 * integer (cell flags >> 12) - 1 (< 7) and stores the result without narrowing, so the
 * parameter is an index and the result is full width; func_801E976C's u8 result is
 * narrowed here (andi 0xFF). */
s32 func_8004252C(u32 index) { if (!func_800610A8()) return 0; return func_801E976C(index); }
