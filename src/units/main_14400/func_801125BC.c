#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { char pad[0xC]; u16 unkC; } Obj;
extern u16 D_801575C0[];
u8 func_800AE98C(Obj *);
s32 func_800AC584(u16);
s32 func_801125BC(Obj *self) {
    return func_800AC584(D_801575C0[func_800AE98C(self)]) * self->unkC;
}
