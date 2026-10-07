#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x8];
    void *field_8;
} Obj80125600;

extern u8 D_801601F8[];

Obj80125600 *func_80115690(Obj80125600 *obj, s32 kind);

Obj80125600 *func_80125600(Obj80125600 *obj) {
    func_80115690(obj, 0xDF);
    obj->field_8 = D_801601F8;
    return obj;
}
