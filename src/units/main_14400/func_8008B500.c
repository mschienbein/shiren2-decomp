#include "common.h"
typedef unsigned short u16;
typedef struct { unsigned char pad_00[4]; u16 field_04; unsigned char pad_06[8]; u16 field_0E; unsigned char pad_10[0x14]; s32 field_24; s32 field_28; unsigned char pad_2C[0x30]; s32 field_5C; s32 field_60; s32 field_64; s32 field_68; s32 field_6C; s32 field_70; } Object;
extern void func_80061DD0(s32 x0, s32 y0, s32 x1, s32 y1);
extern void func_8007D824(s32 a, s32 b, s32 c, s32 d);
void func_8008B500(Object *self) { func_80061DD0(self->field_5C, self->field_68, self->field_5C, self->field_68); func_8007D824(self->field_64, self->field_70, self->field_24, self->field_28); self->field_0E = 1; self->field_04 = 4; }
