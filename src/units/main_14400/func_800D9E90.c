#include "common.h"
typedef unsigned short u16;
extern s32 D_80157FA8[];
extern s32 D_80158158[];
typedef struct { u16 unk0; u16 pad2; void *vtbl; } Obj;
Obj *func_800D9E90(Obj *self) {
    self->vtbl = D_80157FA8;
    self->unk0 = 50;
    self->vtbl = D_80158158;
    return self;
}
