#include "common.h"
typedef unsigned char u8;
typedef struct { char pad[0x24]; void *vtbl; } Obj800F5F60;
extern s32 D_801599F8[];
void *func_800F3CF0(Obj800F5F60 *, s32, u8);
s32 func_800A3934(Obj800F5F60 *);
void func_800F5FB4(Obj800F5F60 *);
Obj800F5F60 *func_800F5F60(Obj800F5F60 *self, u8 arg1) {
    func_800F3CF0(self, 0x57, arg1);
    self->vtbl = D_801599F8;
    if (func_800A3934(self) == 0) {
        func_800F5FB4(self);
    }
    return self;
}
