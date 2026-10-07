#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x8];
    void *vtable;
} Obj;

extern u8 D_8015DE98[];
extern Obj *func_80116D50(Obj *self, s32 kind);

Obj *func_80118130(Obj *self)
{
    func_80116D50(self, 10);
    self->vtable = D_8015DE98;
    return self;
}
