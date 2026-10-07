#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x42];
    u8 field_42;
} Obj;

extern u32 func_800E10D0(Obj *obj);

s32 func_800E1DA0(Obj *obj) {
    s32 result = 0;

    if (obj->field_42 == 0xFF) {
        result = func_800E10D0(obj) == 0xF;
    }
    return result;
}
