#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct {
    u8 pad0[0x3C];
    s32 field_3C;
    s32 field_40;
    u8 pad44[0x10];
    s32 field_54;
    u8 pad58[0x42];
    u16 field_9A;
    u8 pad9C[0x16];
    u16 field_B2;
    u16 field_B4;
    u8 padB6[0x9];
    u8 field_BF;
    u8 field_C0;
    u8 padC1[0x2];
    u8 field_C3;
    u8 field_C4;
    u8 field_C5;
} Obj8012B640;
void func_8012B640(Obj8012B640 *obj) {
    if (obj->field_9A != 0x7FFF) {
        if (obj->field_B2 != 0) {
            obj->field_54 = obj->field_40 + (obj->field_B2 << 8);
        } else {
            obj->field_54 = obj->field_3C - (obj->field_B4 << 8);
        }
    } else {
        obj->field_54 = obj->field_40 + 0x7FFFFFFF;
    }
    obj->field_C3 = 1;
    obj->field_C4 = obj->field_C0;
    obj->field_C5 = obj->field_BF;
}
