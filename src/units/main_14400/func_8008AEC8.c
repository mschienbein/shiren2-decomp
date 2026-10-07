#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x4];
    s16 state;
    u8 pad6[0xE];
    s32 field_14;
    u8 pad18[0x44];
    s32 field_5C;
    u8 pad60[0x8];
    s32 field_68;
} Obj;

extern s32 func_8007BE1C(s32 a, s32 b);

void func_8008AEC8(Obj *obj) {
    obj->field_14 = func_8007BE1C(obj->field_5C, obj->field_68);
    obj->state = 4;
}
