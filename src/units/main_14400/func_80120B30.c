#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x8];
    void *field_8;
} Obj80120B30;

extern u8 D_8015F848[];

Obj80120B30 *func_80114060(Obj80120B30 *obj, s32 kind);

Obj80120B30 *func_80120B30(Obj80120B30 *obj) {
    func_80114060(obj, 0xAA);
    obj->field_8 = D_8015F848;
    return obj;
}
