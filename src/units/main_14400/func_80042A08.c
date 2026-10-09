#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[0x1C]; unsigned short field_1C; } Cell;
extern void *func_800A8CB0(s32);
/* Callers pass un-narrowed indices (0x8008AC60/0x8008ADD4 loop counters, 0x800772D4):
 * full-width parameter, narrowed to the byte cell id here (0x80042A14). */
s32 func_80042A08(s32 value) { Cell *cell = func_800A8CB0((u8)value); return cell->field_1C & 1; }
