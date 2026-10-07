#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x8];
    void *vtable;
} Obj;

extern u8 D_8015FAD0[];
extern Obj *func_80114060(Obj *self, s32 kind);

Obj *func_80123130(Obj *self)
{
    func_80114060(self, 177);
    self->vtable = D_8015FAD0;
    return self;
}
