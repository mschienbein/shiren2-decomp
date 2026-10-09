#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_00[0xB9]; u8 field_B9; } Object;
extern s32 func_8012BF6C(s32 range);
u8 *func_80129944(Object *self, u8 *cursor) { self->field_B9 = func_8012BF6C(*cursor++); self->field_B9 += *cursor++; return cursor; }
