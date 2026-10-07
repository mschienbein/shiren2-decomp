#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0xC0];
    s32 field_C0;
    s32 field_C4;
    s32 field_C8;
    s32 field_CC;
    s32 field_D0;
} Obj_8010AC5C;

extern void func_800EEED8(Obj_8010AC5C *obj);

void func_8010AC5C(Obj_8010AC5C *obj) {
    func_800EEED8(obj);
    obj->field_C0 = 0;
    obj->field_C4 = 0;
    obj->field_C8 = 0;
    obj->field_CC = 0;
    obj->field_D0 = 0;
}
