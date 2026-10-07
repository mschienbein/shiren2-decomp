#include "common.h"

typedef unsigned char u8;
typedef struct {
    u8 pad0[0x20];
    s32 unk20;
    void *vtable;
    u8 pad28[0x50];
    s32 unk78;
} Obj;
extern u8 D_80159DF0[];
void *func_800F3CF0(Obj *, s32, u8);
s32 func_800A3934(Obj *);
void func_800E4D88(Obj *, s32);

Obj *func_800F8B30(Obj *obj, u8 kind)
{
    func_800F3CF0(obj, 0x5E, kind);
    obj->vtable = D_80159DF0;
    if (func_800A3934(obj) == 0) {
        func_800E4D88(obj, 0);
        obj->unk78 |= 0x4000000;
        obj->unk20 = obj->unk78;
    }
    return obj;
}
