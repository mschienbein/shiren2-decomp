#include "common.h"

typedef unsigned char u8;

extern u8 D_8015A3A8[];
typedef struct { u8 pad0[0x24]; void *x24; } Obj;
void func_800EFD28(Obj *obj, s32 arg);
void func_800A3918(Obj *obj);
void func_800FC398(Obj *obj, s32 flags) {
    obj->x24 = D_8015A3A8;
    func_800EFD28(obj, 0);
    if (flags & 1) {
        func_800A3918(obj);
    }
}
