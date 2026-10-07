#include "common.h"

typedef struct { s32 a; s32 b; } Pair;
typedef struct {
    s32 count;
    char pad4[2];
    unsigned short field_6;
    s32 field_8;
    s32 field_C;
} Desc;
typedef struct {
    char pad0[0x10];
    char sub_10[0x10];
    s32 field_20;
    s32 field_24;
    s32 field_28;
    s32 field_2C;
    unsigned short field_30;
    char pad32[2];
    s32 field_34;
    s32 field_38;
    s32 field_3C;
    s32 field_40;
    unsigned char field_44;
    char pad45;
    unsigned char field_46;
    char pad47;
    s32 field_48;
} Obj;

extern Pair D_80151E2C;
void func_800957AC(Obj *obj, unsigned char value);
void func_80095E58(void *sub, Obj *obj, Pair pair);

void func_8009543C(Obj *obj, Desc *desc) {
    obj->field_20 = desc->count;
    if (obj->field_20 <= 0) {
        obj->field_20 = 1;
    }
    obj->field_24 = 1;
    obj->field_30 = desc->field_6;
    obj->field_28 = desc->field_8;
    obj->field_2C = desc->field_C;
    obj->field_34 = 0;
    obj->field_38 = 0;
    obj->field_3C = 0;
    obj->field_40 = 0;
    obj->field_44 = 1;
    func_800957AC(obj, 0);
    func_80095E58(obj->sub_10, obj, D_80151E2C);
    obj->field_46 = 1;
    obj->field_48 = -1;
}
