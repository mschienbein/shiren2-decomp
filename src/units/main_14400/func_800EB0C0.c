#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x7C];
    s32 field_7C;
} Obj;

void func_800EB0C0(Obj *obj, s32 value) {
    obj->field_7C = value;
}
