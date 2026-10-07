#include "common.h"
typedef struct { unsigned char field_00[2]; short field_02; unsigned char field_04[0x42]; unsigned char field_46; unsigned char field_47[0x2C]; unsigned char field_73; unsigned char field_74[3]; unsigned char field_77; } Child;
typedef struct { unsigned char field_00[4]; short field_04; short field_06; unsigned short field_08; unsigned char field_0A[0xA]; s32 field_14; s32 field_18; s32 field_1C; unsigned char field_20[0x3C]; s32 field_5C; unsigned char field_60[8]; s32 field_68; } Object;
extern s32 func_80084014(s32, s32);
extern Child *func_8007946C(s32, s32);
extern void func_80079560(s32, s32, s32);
void func_8008A7B0(Object *object) {
    Child *child;
    if (!func_80084014(object->field_5C, object->field_68)) { object->field_04 = 4; return; }
    if (!object->field_08) object->field_14 = (object->field_68 % 10) * 11 + object->field_5C % 11;
    child = func_8007946C(3, object->field_14);
    switch (object->field_08) {
    case 0: func_80079560(3, object->field_14, 0); object->field_1C = 1; object->field_08++; break;
    case 1: if (--object->field_1C == 0) { child->field_77 = 0x40; child->field_46 = 0xFF; object->field_1C = 15; object->field_08++; } break;
    case 2: if (--object->field_1C != 0) { child->field_46 -= 8; child->field_73 -= 4; child->field_77 -= 4; } else { func_80079560(3, object->field_14, 1); child->field_02 = -1; object->field_04 = 4; } break;
    }
}
