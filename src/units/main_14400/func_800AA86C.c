#include "common.h"
typedef unsigned char u8;
extern unsigned char D_80147620[];
extern u8 func_800C57A0(void *);
extern s32 func_800AA48C(u8, u8);
extern void *func_800A8640(s32, s32);
void *func_800AA86C(void) { s32 value; if (func_800C57A0(D_80147620) & 1) value = 0x58; else value = 0x59; if (func_800AA48C((u8)value, 1)) value = value == 0x58 ? 0x59 : 0x58; return func_800A8640((u8)value, 1); }
