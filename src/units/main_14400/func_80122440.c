#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x8];
    void *field_8;
} Obj;

extern u8 D_8015F990[];
Obj *func_80114060(Obj *obj, s32 kind);

Obj *func_80122440(Obj *obj) {
    func_80114060(obj, 0xAD);
    obj->field_8 = D_8015F990;
    return obj;
}
