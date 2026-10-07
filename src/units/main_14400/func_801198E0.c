#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[0x8]; void *vtable_8; } Obj801198E0;
extern u8 D_8015E3B8[];
Obj801198E0 *func_80116D50(Obj801198E0 *obj, s32 kind);
void func_800ACF34(Obj801198E0 *obj);

Obj801198E0 *func_801198E0(Obj801198E0 *obj) {
    func_80116D50(obj, 0x16);
    obj->vtable_8 = D_8015E3B8;
    func_800ACF34(obj);
    return obj;
}
