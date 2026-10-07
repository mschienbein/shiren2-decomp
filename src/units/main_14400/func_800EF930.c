#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
/* 0x34-byte embedded subobject constructed by overlay func_801F2AC0, which initialises
 * fields at +0x00..+0x2D (opaque here) and its vtable word at +0x30. */
typedef struct { u8 opaque[0x30]; void *vtable; } Sub801F2AC0;
typedef struct {
    u8 pad0[0xA];
    u8 unkA;
    u8 padB[0x11];
    u16 unk1C;
    u8 unk1E;
    u8 unk1F;
    u8 pad20[4];
    void *vtable;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x46];
    Sub801F2AC0 unk78;
} Obj;
extern u8 D_80159300[];
extern u8 D_80159320[];
extern u32 D_8013960C;
void *func_800E0120(Obj *);
void func_801F2AC0(Sub801F2AC0 *);
s32 func_800A3934(Obj *);
void func_800E039C(Obj *, s32);

Obj *func_800EF930(Obj *obj, u8 kind)
{
    func_800E0120(obj);
    func_801F2AC0(&obj->unk78);
    obj->unk78.vtable = D_80159300;
    obj->vtable = D_80159320;
    if (func_800A3934(obj) == 0) {
        obj->unk28 = 1;
        obj->unk2A = 1;
        obj->unk2C = 1;
        obj->unk2E = 1;
        obj->unk30 = 1;
        D_8013960C <<= 1;
        obj->unk1E = 0x40;
        obj->unkA = kind;
        obj->unk1F = kind;
        func_800E039C(obj, 1);
        obj->unk1C |= 1;
        D_8013960C >>= 1;
    }
    return obj;
}
