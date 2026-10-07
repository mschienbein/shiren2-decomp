#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad0[0x10]; s32 field_10; u8 field_14; u8 field_15; } Obj;
extern u8 D_801575D8[];
u8 func_800AE98C(Obj *obj);
void func_801124B4(Obj *obj) {
    u8 value;

    obj->field_10 = 6;
    value = D_801575D8[func_800AE98C(obj)];
    obj->field_15 = 0;
    obj->field_14 = value;
}
