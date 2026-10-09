#include "common.h"
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
typedef struct { u8 lo, hi, min, max, field_4, kind, chance, field_7; } Range;
extern const Range D_80156CD0[8];  extern s32 func_800A99A8(void);
u8 func_800A9958(void){ u8 r; if (func_800A99A8()==0) r = D_80142F24.index; else r = D_80156CD0[D_80142F24.index-12].kind; return r; }
