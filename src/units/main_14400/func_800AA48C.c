#include "common.h"
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;

s32 func_800AA48C(u8 a, u8 b){ s32 r=0; if (a==D_80142F24.previous) r = b==D_80142F24.field_03; return r; }
