#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0xA4];
    s32 field_A4;
} Obj;

void func_80108AF4(Obj *obj, s32 value) {
    obj->field_A4 = value;
}
