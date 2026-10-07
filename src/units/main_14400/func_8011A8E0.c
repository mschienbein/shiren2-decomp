#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x8];
    void *vtable;
} Obj;

extern u8 D_8015E5F0[];
extern Obj *func_80112D20(Obj *self, s32 kind);

Obj *func_8011A8E0(Obj *self)
{
    func_80112D20(self, 29);
    self->vtable = D_8015E5F0;
    return self;
}
