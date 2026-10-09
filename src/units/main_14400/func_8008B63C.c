#include "common.h"
typedef struct { unsigned char pad_0[4]; short field_4; unsigned char pad_6[0x56]; s32 field_5C; unsigned char pad_60[8]; s32 field_68; } Object;
extern void func_80062304(s32 x0, s32 y0, s32 x1, s32 y1);
void func_8008B63C(Object *p) { s32 x=p->field_5C; s32 y=p->field_68; func_80062304(x, y, x, y); p->field_4 = 4; }
