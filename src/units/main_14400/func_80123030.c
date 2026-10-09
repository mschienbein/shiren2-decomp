#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[4];
    u8 kind;
    u8 pad5[3];
    const void *vtable;
    u8 padC[0x1C];
    u8 field_28;
} Obj80123030;

extern const unsigned char D_8015FA80[];

Obj80123030 *func_801140C0(Obj80123030 *obj, s32 kind, s32 arg);
void func_800ACF34(Obj80123030 *obj);

Obj80123030 *func_80123030(Obj80123030 *obj)
{
    func_801140C0(obj, 0xB0, 0);
    obj->vtable = D_8015FA80;
    obj->field_28 = 0;
    obj->kind = 3;
    func_800ACF34(obj);
    return obj;
}
