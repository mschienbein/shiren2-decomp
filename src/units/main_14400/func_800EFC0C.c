#include "common.h"
typedef struct { char pad[0x24]; void *vtbl24; char pad2[0xA8 - 0x28]; void *vtblA8; } Obj;
extern s32 D_80159300;
extern s32 D_80159320;
void func_800E016C(Obj *, s32);
void func_800A3918(Obj *);
void func_800EFC0C(Obj *self, s32 flags) {
    self->vtblA8 = &D_80159300;
    self->vtbl24 = &D_80159320;
    func_800E016C(self, 0);
    if (flags & 1) func_800A3918(self);
}
