#include "common.h"
typedef struct { unsigned char field_0[0x1C]; float field_1C, field_20; } Target;
typedef struct { s32 field_0; unsigned short field_4, field_6, field_8; unsigned char field_A[10]; s32 field_14, field_18, field_1C, field_20, field_24; unsigned char field_28[0x14]; float field_3C; unsigned char field_40[8]; float field_48; } Object;
extern Target *func_8007946C(s32, s32);
void func_8008A680(Object *arg) { Target *target = func_8007946C(0, arg->field_14); switch (arg->field_8) { case 0: arg->field_1C = 8; arg->field_3C = target->field_1C; arg->field_48 = target->field_20; arg->field_8++; case 1: if (arg->field_1C--) { float sign = -1.0f; if (arg->field_1C & 1) sign = 1.0f; if (arg->field_24 == 1) sign -= 0.2f; target->field_1C += sign * (arg->field_3C * 0.2f); target->field_20 += sign * (arg->field_48 * 0.2f); } else { target->field_1C = arg->field_3C; target->field_20 = arg->field_48; arg->field_4 = 4; } break; } }
