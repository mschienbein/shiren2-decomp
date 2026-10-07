#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[0x24]; void *vtbl; } Obj;
extern s32 D_8015B9B8;
void func_800EFD28(Obj *obj, s32 flags);
void func_800A3918(void *ptr);
void func_80103A6C(Obj *obj, s32 flags) {
    obj->vtbl = &D_8015B9B8;
    func_800EFD28(obj, 0);
    if (flags & 1) {
        func_800A3918(obj);
    }
}
