#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
extern u16 D_80157308[], D_801573A8[], D_801571AC[], D_80157280[], D_801575EC[], D_80157734[], D_80157670[];
extern u8 D_80157448[];
s32 func_8010C174(void *obj, u8 kind, u16 *base_values, u8 *modifier_scales, u16 *values_1, u16 *values_2, u16 *values_3, u16 *values_4, u16 *values_5, u16 *values_6);
s32 func_8010EEF4(u8 *s) { return func_8010C174(s, s[1], D_80157308, D_80157448, D_801573A8, D_801571AC, D_80157280, D_801575EC, D_80157734, D_80157670); }
