#include "common.h"
typedef unsigned short u16;
typedef struct { unsigned char pad_00[4]; u16 field_04; unsigned char pad_06[8]; u16 field_0E; unsigned char pad_10[0x4C]; s32 field_5C; unsigned char pad_60[8]; s32 field_68; } Object;
extern void func_80062658(s32 a, s32 b);
void func_8008B72C(Object *self) { func_80062658(self->field_5C, self->field_68); self->field_0E = 1; self->field_04 = 4; }
