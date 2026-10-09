#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[8];
    void *vtable8;
    u8 flagsC;
} Obj;

extern u8 D_80160158[];
extern void *func_80115690(void *obj, s32 kind);

Obj *func_80125340(Obj *obj) {
    func_80115690(obj, 0xDD);
    obj->vtable8 = D_80160158;
    obj->flagsC |= 8;
    return obj;
}
