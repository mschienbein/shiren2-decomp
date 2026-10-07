#include "common.h"

typedef unsigned char u8;
typedef struct {
    u8 pad0[0xA];
    u8 unkA;
    u8 padB[0x13];
    u8 unk1E;
    u8 unk1F;
    u8 pad20[4];
    void *vtable;
    u8 pad28[0x50];
    u32 unk78;
} Obj;
extern u8 D_80159500[];
void *func_800E0120(Obj *);
void func_80136908(u32 *);
s32 func_800A3934(Obj *);
void func_800F3D88(Obj *, u8);
void func_800E4D88(Obj *, s32);
void func_800E4D90(Obj *, s32);

Obj *func_800F3CF0(Obj *obj, s32 kind, u8 arg)
{
    func_800E0120(obj);
    obj->vtable = D_80159500;
    func_80136908(&obj->unk78);
    if (func_800A3934(obj) == 0) {
        obj->unk1E = 0x20;
        obj->unkA = kind;
        obj->unk1F = kind;
        func_800F3D88(obj, arg);
        func_800E4D88(obj, 1);
        func_800E4D90(obj, 0);
    }
    return obj;
}
