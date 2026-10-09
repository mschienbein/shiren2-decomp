#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 field_0;
    u8 field_1;
    u8 flags;
    u8 pad3[0x8 - 0x3];
    void *handler;
    u8 field_C;
} Obj80115690;

extern u8 D_8015D9D8[];
void *func_800AC0C0(Obj80115690 *obj, s32 kind, s32 arg);

Obj80115690 *func_80115690(Obj80115690 *obj, s32 arg) {
    func_800AC0C0(obj, 0x10, arg);
    obj->handler = D_8015D9D8;
    obj->field_C = 0;
    obj->flags |= 0x10;
    return obj;
}
