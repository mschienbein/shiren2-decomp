#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0xA4];
    s32 field_A4;
} Obj;

s32 func_800F837C(Obj *obj) {
    return obj->field_A4;
}
