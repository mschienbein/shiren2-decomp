#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 field0;
    s32 field4;
    void *vtable8;
    u8 flagsC;
} Obj80124820;

extern u8 D_8015FED0[];
extern Obj80124820 *func_80115690(Obj80124820 *obj, s32 arg1);

Obj80124820 *func_80124820(Obj80124820 *obj) {
    Obj80124820 *self = obj;

    func_80115690(obj, 0xD6);
    self->vtable8 = D_8015FED0;
    self->flagsC |= 8;
    return self;
}
