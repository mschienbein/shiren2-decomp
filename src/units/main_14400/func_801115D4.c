#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u8 pad0[0xC];
    u8 field_C;
} Obj;

void func_801115D4(Obj *obj, s32 delta) {
    s16 value = obj->field_C + delta;

    if (value < 0) {
        value = 0;
    } else if (value >= 100) {
        value = 99;
    }
    obj->field_C = value;
}
