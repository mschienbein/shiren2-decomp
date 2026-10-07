#include "common.h"

typedef struct {
    char pad0[4];
    short field_4;
    char pad6[0x1E];
    s32 field_24;
    s32 field_28;
    s32 field_2C;
} Obj;

extern void func_80077BE4(s32 a, s32 b, s32 c);

void func_8008876C(Obj *obj) {
    func_80077BE4(obj->field_24, obj->field_28, obj->field_2C);
    obj->field_4 = 4;
}
