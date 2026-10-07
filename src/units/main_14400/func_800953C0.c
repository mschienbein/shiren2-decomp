#include "common.h"

typedef unsigned char u8;
typedef struct {
    u8 pad0[0xC];
    s32 field_C;
    void *field_10;
    u8 pad14[0x46 - 0x14];
    u8 field_46;
    u8 pad47;
    s32 field_48;
    void *field_4C;
} Obj;
extern u8 D_80151E38[];
extern u8 D_80151EC8[];
Obj *func_800953C0(Obj *o) {
    o->field_4C = D_80151E38;
    o->field_C = -1;
    o->field_10 = D_80151EC8;
    o->field_46 = 1;
    o->field_48 = -1;
    return o;
}
