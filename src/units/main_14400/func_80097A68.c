#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x4C];
    void *vtable;
} Obj;

extern u8 D_80152260[];
extern Obj *func_800953C0(Obj *self);

Obj *func_80097A68(Obj *self)
{
    func_800953C0(self);
    self->vtable = D_80152260;
    return self;
}
