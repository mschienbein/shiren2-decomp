#include "common.h"

typedef struct {
    char pad0[0x48];
    s32 field_48;
    s32 field_4C;
    char pad50[2];
    unsigned char field_52;
    char pad53;
    unsigned char field_54;
    char pad55;
    unsigned char field_56;
} Obj;

void func_800E4DA8(Obj *obj) {
    obj->field_56 = 0;
    obj->field_48 = 0;
    obj->field_4C = 0;
    obj->field_54 = 0;
    if (obj->field_52 == 2) {
        obj->field_52 = 3;
    } else if (obj->field_52 == 4) {
        obj->field_52 = 1;
    }
}
