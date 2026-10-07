#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x8];
    void *vtable;
} Obj;

extern u8 D_80149010[];
extern Obj *func_80112470(Obj *self, s32 kind);

Obj *func_8011D400(Obj *self)
{
    func_80112470(self, 120);
    self->vtable = D_80149010;
    return self;
}
