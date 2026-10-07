#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x8];
    void *vtable;
} Obj;

extern u8 D_8015F3D8[];
extern Obj *func_80111530(Obj *self, s32 kind);

Obj *func_8011FA20(Obj *self)
{
    func_80111530(self, 157);
    self->vtable = D_8015F3D8;
    return self;
}
