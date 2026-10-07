#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct {
    char pad0[0xA];
    u8 unkA;
    char padB[0x1E - 0xB];
    u8 unk1E;
    u8 unk1F;
    char pad20[0x24 - 0x20];
    void *vtable;
    char pad28[0x32 - 0x28];
    u8 unk32;
    char pad33[0x75 - 0x33];
    u8 unk75;
    char pad76[0x8C - 0x76];
    s32 unk8C;
    char unk90[0x9A - 0x90];
    s16 unk9A;
    u8 unk9C;
    u8 unk9D;
    u8 unk9E;
    u8 unk9F;
} Obj800EFC70;

extern char D_80159440[];
extern void *func_800E0120(Obj800EFC70 *obj);
extern void func_80136908(void *arg0);
extern s32 func_800A3934(Obj800EFC70 *obj);
extern void func_800E4D88(Obj800EFC70 *obj, s32 arg1);
extern void func_800E4D90(Obj800EFC70 *obj, s32 arg1);
extern void func_800EFF0C(Obj800EFC70 *obj);

Obj800EFC70 *func_800EFC70(Obj800EFC70 *obj, s32 arg1, u8 arg2) {
    func_800E0120(obj);
    obj->vtable = D_80159440;
    func_80136908(obj->unk90);
    if (func_800A3934(obj) != 0) {
        return obj;
    }
    func_800E4D88(obj, 1);
    func_800E4D90(obj, 0);
    obj->unk1E = 16;
    obj->unk8C = 0;
    obj->unk9E = 0;
    obj->unk9F = 0;
    obj->unk9C = 0xFF;
    obj->unk9D = 0;
    obj->unk9A = 0;
    obj->unkA = arg1;
    obj->unk1F = arg1;
    obj->unk32 = arg2;
    obj->unk75 = arg2;
    func_800EFF0C(obj);
    return obj;
}
