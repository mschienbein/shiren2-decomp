#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { char pad[0x24]; void *vtbl; char pad2[0x9A - 0x28]; u16 unk9A; } Obj;
extern s32 D_80159E98;
void *func_800EFC70(void *, s32, u8);
void func_800E4D88(Obj *, s32);
void func_800E4D90(Obj *, s32);
Obj *func_800F8EB0(Obj *self, u8 kind) {
    u16 f;
    func_800EFC70(self, 0x1E, kind);
    self->vtbl = &D_80159E98;
    func_800E4D88(self, 3);
    func_800E4D90(self, 2);
    f = self->unk9A;
    self->unk9A = f | 1;
    if (kind >= 2) self->unk9A = f | 3;
    return self;
}
