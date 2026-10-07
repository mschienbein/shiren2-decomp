#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
extern u8 D_80153B40[];
extern u8 D_8014A7A8[];
typedef struct {
    u8 kind;
    u8 pad1[3];
    s32 unk4;
    void *vtable;
} Obj;
void func_800C2440(Obj *, u8, s8, s8);

Obj *func_800435F8(Obj *obj, u8 kind, s8 a, s8 b)
{
    obj->vtable = D_80153B40;
    obj->kind = kind;
    obj->unk4 = 0;
    obj->vtable = D_8014A7A8;
    func_800C2440(obj, kind, a, b);
    return obj;
}
