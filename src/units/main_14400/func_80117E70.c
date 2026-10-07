#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x8];
    void *field_8;
} Obj;

extern u8 D_8015DD90[];
Obj *func_80116D50(Obj *obj, s32 kind);

Obj *func_80117E70(Obj *obj) {
    func_80116D50(obj, 0x7);
    obj->field_8 = D_8015DD90;
    return obj;
}
