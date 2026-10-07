#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x8];
    void *vtable;
} Obj;

extern u8 D_80160338[];
extern Obj *func_80115690(Obj *self, s32 kind);

Obj *func_80125B20(Obj *self)
{
    func_80115690(self, 227);
    self->vtable = D_80160338;
    return self;
}
