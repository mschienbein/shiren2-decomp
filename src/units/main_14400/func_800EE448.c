#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct {
    u8 pad0[0xA];
    u8 field_A;
    u8 padB[0x14];
    u8 field_1F;
    u32 field_20;
    u8 pad24[4];
    u16 field_28;
    u16 field_2A;
    u16 field_2C;
    u16 field_2E;
    u16 field_30;
    u8 field_32;
    u8 pad33[0x45];
    s32 field_78;
    u8 pad7C[4];
    u32 field_80;
    u8 pad84[0x34];
    u16 field_B8;
    u8 field_BA;
    u8 field_BB;
    u8 field_BC;
} Obj;
extern u32 D_8013960C;
void func_800E039C(Obj *obj, s32 arg1);
void func_800E4D88(Obj *obj, s32 arg1);
void func_800E4D90(Obj *obj, s32 arg1);

void func_800EE448(Obj *obj, s32 arg1, u8 arg2) {
    s32 kind = arg1 & 0xFF;

    obj->field_A = kind;
    obj->field_1F = kind;
    D_8013960C <<= 1;
    obj->field_28 = 15;
    obj->field_2A = 15;
    obj->field_2C = 8;
    obj->field_2E = 8;
    obj->field_30 = 0;
    obj->field_78 = 0;
    obj->field_32 = arg2;
    func_800E039C(obj, 1);
    func_800E4D88(obj, 2);
    func_800E4D90(obj, 1);
    obj->field_BA = 0;
    obj->field_BB = 0;
    obj->field_BC = 0;
    obj->field_B8 = 0;
    obj->field_80 |= 0x4000000;
    obj->field_20 = obj->field_80;
    D_8013960C >>= 1;
}
