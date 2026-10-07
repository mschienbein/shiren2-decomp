#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x8];
    void *field_8;
} Obj;

extern u8 D_80160248[];
Obj *func_80115690(Obj *obj, s32 kind);

Obj *func_80125710(Obj *obj) {
    func_80115690(obj, 0xE0);
    obj->field_8 = D_80160248;
    return obj;
}
