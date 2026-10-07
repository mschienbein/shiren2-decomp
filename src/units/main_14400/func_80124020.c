#include "common.h"

typedef unsigned char u8;

extern u8 D_8015FDD8[];
void *func_80115690(void *, s32);
typedef struct {
    u8 pad0[8];
    void *vtable;
    u8 flags;
} Obj;
Obj *func_80124020(Obj *self) {
    Obj *obj = self;

    func_80115690(self, 0xD3);
    obj->vtable = D_8015FDD8;
    obj->flags |= 8;
    return obj;
}
