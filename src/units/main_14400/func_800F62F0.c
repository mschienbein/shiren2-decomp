#include "common.h"

typedef struct {
    char pad0[0x1C];
    unsigned short flags_1C;
    char pad1E[2];
    s32 field_20;
    char pad24[0x54];
    s32 field_78;
} Obj;

extern s32 D_80148270;
s32 func_800E0F40(Obj *obj);
s32 func_800E1CC4(Obj *obj, s32 kind);
s32 func_800E1CD4(Obj *obj, s32 kind);

void func_800F62F0(Obj *obj) {
    unsigned char level = func_800E0F40(obj);

    s32 blocked;

    obj->field_78 = 0;
    obj->flags_1C |= 0x80;
    blocked = func_800E1CC4(obj, 2) == 1;
    if (!blocked) {
        if (level >= 2) {
            obj->field_78 |= 0x8000;
        }
        if (level >= 3) {
            obj->field_78 |= 0x2000;
        }
    }
    if (func_800E1CD4(obj, 0xF)) {
        obj->field_78 &= ~D_80148270;
    }
    obj->field_20 = obj->field_78;
}
