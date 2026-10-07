#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x8];
    void *handler;
    u8 padC[0x2C - 0xC];
    u8 field_2C;
} Obj801216D0;

extern u8 D_8015F938[];
void func_801140C0(Obj801216D0 *obj, s32 kind, s32 arg);
void func_800ACF34(Obj801216D0 *obj);

Obj801216D0 *func_801216D0(Obj801216D0 *obj) {
    func_801140C0(obj, 0xAC, 1);
    obj->handler = D_8015F938;
    obj->field_2C = 0xFF;
    func_800ACF34(obj);
    return obj;
}
