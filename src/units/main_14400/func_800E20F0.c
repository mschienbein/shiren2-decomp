#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u8 pad0[0x72];
    u8 field_72;
} Obj;

s32 func_80049CB4(s32 id, ...);

void func_800E20F0(Obj *obj) {
    if (obj->field_72 & 1) {
        obj->field_72 &= ~1;
        func_80049CB4(0x1F, obj);
    }
}
